import requests

from flask_restful import Resource, request, abort
from flask_restful_swagger import swagger

from hpcpm.api import log
from hpcpm.api.helpers.database import database
from hpcpm.api.helpers.utils import abort_when_not_int, COMPUTATION_NODE_PARAM, COMPUTATION_NODE_NOT_FOUND_RESPONSE, \
    COMPUTATION_NODE_FETCHED_RESPONSE


class PowerLimit(Resource):
    @swagger.operation(
        notes="This endpoint is used for setting power limit for given device.",
        nickname="/nodes/computation_node/<string:name>/<string:device_id>/power_limit",
        parameters=[
            COMPUTATION_NODE_PARAM,
            {
                'name': 'device_id',
                'description': 'Device identifier',
                'required': True,
                'allowMultiple': False,
                'dataType': 'string',
                "paramType": 'path'
            },
            {
                'name': 'power_limit',
                'description': 'Power limit',
                'required': True,
                'allowMultiple': False,
                'dataType': 'int',
                "paramType": 'query'
            },
        ],
        responseMessages=[
            {
                "code": 201,
                "message": "Power limit set successfully"
            },
            COMPUTATION_NODE_NOT_FOUND_RESPONSE
        ]
    )
    def put(self, name, device_id):
        power_limit = request.args.get('power_limit')
        abort_when_not_int(power_limit)
        computation_node = database.get_computation_node_info(name)
        if not computation_node:
            log.error('There is no such computation node: %s', name)
            abort(404)

        if not any(d['id'] == device_id for d in computation_node['backend_info']['devices']):
            log.error('There is no such device: %s', device_id)
            abort(404)

        limit_info = {
            'name': name,
            'device_id': device_id,
            'power_limit': power_limit
        }
        upsert_result = database.replace_power_limit_for_device(name, device_id, limit_info)
        if upsert_result.modified_count:
            log.info("Power limit for device %s:%s was already set in a database to %s", name, device_id, power_limit)
            log.info("Stored power limit info %s", limit_info)
        else:
            log.info("Stored power limit info %s on id %s", limit_info, upsert_result.upserted_id)

        power_limit_query = "http://" + computation_node['address'] + ":" + computation_node['port'] + "/power_limit"
        try:
            response = requests.put(power_limit_query, params={'device_id': device_id, 'power_limit': power_limit})
            log.info(response.text)
        except requests.exceptions.ConnectionError:
            log.error("Connection could not be established to %s", power_limit_query)
            return 'Failed to set power limit on device, but added to database', 201
        return 'Power limit successfully set', 201

    @swagger.operation(
        notes="This endpoint is used for getting power limit information from database",
        nickname="/nodes/computation_node/<string:name>/<string:device_id>/power_limit",
        parameters=[
            COMPUTATION_NODE_PARAM,
            {
                'name': 'device_id',
                'required': True,
                'allowMultiple': False,
                'description': 'Device identifier',
                'dataType': 'string',
                "paramType": 'path'
            }
        ],
        responseMessages=[
            COMPUTATION_NODE_FETCHED_RESPONSE,
            {
                "code": 404,
                "message": "Device could not be found"
            }
        ]
    )
    def get(self, name, device_id):
        result = database.get_power_limit_for_device(name, device_id)
        if not result:
            log.info("No such device %s:%s", name, device_id)
            abort(404)
        log.info('Successfully get device %s:%s power limit info: %s', name, device_id, result)
        return result, 200
