import requests

from flask_restful import Resource, request, abort
from flask_restful_swagger import swagger

from hpcpm.api import log
from hpcpm.api.helpers.database import database
from hpcpm.api.helpers.utils import abort_when_not_int
from hpcpm.api.helpers.constants import COMPUTATION_NODE_PARAM_NAME, COMPUTATION_NODE_NOT_FOUND_RESPONSE, \
    COMPUTATION_NODE_FETCHED_RESPONSE, DEVICE_IDENTIFIER_PARAM, DEVICE_POWER_LIMIT_PARAM, \
    DEVICE_POWER_LIMIT_SET_RESPONSE, DEVICE_NOT_FOUND_RESPONSE, POWER_LIMIT_DELETED_FROM_DB_BUT_NOT_FROM_DEVICE
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
        computation_node = database.get_computation_node_info(name)
        if not computation_node:
            log.error('There is no such computation node: {}'.format(name))
            abort(404)

        if not any(d['id'] == device_id for d in computation_node['backend_info']['devices']):
            log.error('There is no such device: {}'.format(device_id))
            abort(404)

        limit_info = {
            'name': name,
            'device_id': device_id,
            'power_limit': power_limit
        }
        upsert_result = database.replace_power_limit_for_device(name, device_id, limit_info)
        if upsert_result.modified_count:
            log.info('Power limit for device {}:{} was already set in a database to {}'.format(name, device_id,
                                                                                               power_limit))
            log.info('Stored power limit info {}'.format(limit_info))
        else:
            log.info('Stored power limit info {} on id {}'.format(limit_info, upsert_result.upserted_id))

        try:
            response = put_power_limit(computation_node['address'], computation_node['port'], device_id, power_limit)
            log.info(response.text)
        except requests.exceptions.ConnectionError:
            log.error('Connection could not be established to {}:{}'.format(computation_node['address'],
                                                                            computation_node['port']))
            return 'Failed to set power limit on device, but added to database', 201
        return 'Power limit successfully set', 201

    @swagger.operation(
        notes='This endpoint is used for getting power limit information from database',
        nickname='/nodes/computation_node/<string:name>/<string:device_id>/power_limit',
        parameters=[
            COMPUTATION_NODE_PARAM_NAME,
            DEVICE_IDENTIFIER_PARAM
        ],
        responseMessages=[
            COMPUTATION_NODE_FETCHED_RESPONSE,
            DEVICE_NOT_FOUND_RESPONSE
        ]
    )
    def get(self, name, device_id):
        result = database.get_power_limit_for_device(name, device_id)
        if not result:
            log.info('No such device {}:{}'.format(name, device_id))
            abort(404)
        log.info('Successfully get device {}:{} power limit info: {}'.format(name, device_id, result))
        return result, 200

    @swagger.operation(
        notes='This endpoint is used for removing power limit information from database and device',
        nickname='/nodes/computation_node/<string:name>/<string:device_id>/power_limit',
        parameters=[
            COMPUTATION_NODE_PARAM_NAME,
            DEVICE_IDENTIFIER_PARAM
        ],
        responseMessages=[
            COMPUTATION_NODE_FETCHED_RESPONSE,
            DEVICE_NOT_FOUND_RESPONSE,
            POWER_LIMIT_DELETED_FROM_DB_BUT_NOT_FROM_DEVICE
        ]
    )
    def delete(self, name, device_id):
        node_info = database.get_computation_node_info(name)

        if not node_info:
            log.info('No such computation node info {}:{}'.format(name, device_id))
            abort(404)

        address = node_info['address']
        port = node_info['port']

        result = database.delete_power_limit_info(name, device_id)
        if not result:
            log.info('No such device {}:{}'.format(name, device_id))
            abort(404)

        try:
            response = delete_power_limit(address, port, device_id)
            log.info('Power limit for device {} deletion info: {}'.format(device_id, response))
        except requests.exceptions.ConnectionError:
            log.error('Connection could not be established to {}:{}'.format(address, port))
            return 'Warning: power limit was deleted from database but could not be deleted from device', 406

        log.info('Successfully removed power limit for device {}:{} power limit info: {}'.format(name, device_id,
                                                                                                 result))
        return result, 200
