import requests

from flask_restful import Resource, request, abort
from flask_restful_swagger import swagger

from hpcpm.api import log
from hpcpm.api.helpers.database import database
from hpcpm.api.helpers.utils import abort_when_not_int, abort_when_node_not_found
from hpcpm.api.helpers.constants import COMPUTATION_NODE_PARAM_NAME, COMPUTATION_NODE_NOT_FOUND_RESPONSE, \
    COMPUTATION_NODE_FETCHED_RESPONSE, DEVICE_IDENTIFIER_PARAM, DEVICE_POWER_LIMIT_PARAM, \
    DEVICE_POWER_LIMIT_SET_RESPONSE, DEVICE_NOT_FOUND_RESPONSE, POWER_LIMIT_DELETED_FROM_DB_BUT_NOT_FROM_DEVICE, \
    NODE_AND_DEVICE_PARAMS
from hpcpm.api.helpers.requests import put_power_limit, delete_power_limit


class PowerLimit(Resource):
    @swagger.operation(
        notes='This endpoint is used for setting power limit for given device.',
        nickname='/nodes/computation_node/<string:name>/<string:device_id>/power_limit',
        parameters=[
            COMPUTATION_NODE_PARAM_NAME,
            DEVICE_IDENTIFIER_PARAM,
            DEVICE_POWER_LIMIT_PARAM
        ],
        responseMessages=[
            DEVICE_POWER_LIMIT_SET_RESPONSE,
            COMPUTATION_NODE_NOT_FOUND_RESPONSE
        ]
    )
    def put(self, name, device_id):
        power_limit = request.args.get('power_limit')
        abort_when_not_int(power_limit)
        computation_node = abort_when_node_not_found(name)

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
            log.info('Power limit for device %s:%s was already set in a database to %s', name, device_id, power_limit)
            log.info('Stored power limit info %s', limit_info)
        else:
            log.info('Stored power limit info %s on id %s', limit_info, upsert_result.upserted_id)

        try:
            response = put_power_limit(computation_node['address'], computation_node['port'], device_id, power_limit)
            log.info(response.text)
        except requests.exceptions.ConnectionError:
            log.error('Connection could not be established to %s:%s', computation_node['address'],
                      computation_node['port'])
            return 'Failed to set power limit on device, but added to database', 201
        return 'Power limit successfully set', 201

    @swagger.operation(
        notes='This endpoint is used for getting power limit information from database',
        nickname='/nodes/computation_node/<string:name>/<string:device_id>/power_limit',
        parameters=NODE_AND_DEVICE_PARAMS,
        responseMessages=[
            COMPUTATION_NODE_FETCHED_RESPONSE,
            DEVICE_NOT_FOUND_RESPONSE
        ]
    )
    def get(self, name, device_id):
        result = database.get_power_limit_for_device(name, device_id)
        if not result:
            log.info('No such device %s:%s', name, device_id)
            abort(404)
        log.info('Successfully get device %s:%s power limit info: %s', name, device_id, result)
        return result, 200

    @swagger.operation(
        notes='This endpoint is used for removing power limit information from database and device',
        nickname='/nodes/computation_node/<string:name>/<string:device_id>/power_limit',
        parameters=NODE_AND_DEVICE_PARAMS,
        responseMessages=[
            COMPUTATION_NODE_FETCHED_RESPONSE,
            DEVICE_NOT_FOUND_RESPONSE,
            POWER_LIMIT_DELETED_FROM_DB_BUT_NOT_FROM_DEVICE
        ]
    )
    def delete(self, name, device_id):
        node_info = abort_when_node_not_found(name)

        address = node_info['address']
        port = node_info['port']

        result = database.delete_power_limit_info(name, device_id)
        if not result:
            log.info('No such device %s:%s', name, device_id)
            abort(404)

        try:
            response = delete_power_limit(address, port, device_id)
            log.info('Power limit for device %s deletion info: %s', device_id, response)
        except requests.exceptions.ConnectionError:
            log.error('Connection could not be established to %s:%s', address, port)
            return 'Warning: power limit was deleted from database but could not be deleted from device', 406

        log.info('Successfully removed power limit for device %s:%s power limit info: %s', name, device_id,
                 result)
        return result, 200
