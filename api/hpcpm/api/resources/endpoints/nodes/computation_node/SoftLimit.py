from flask_restful import Resource, request, abort
from flask_restful_swagger import swagger

from hpcpm.api import log
from hpcpm.api.helpers.database import database
from hpcpm.api.helpers.utils import abort_when_not_int, abort_when_node_not_found
from hpcpm.api.helpers.constants import COMPUTATION_NODE_PARAM_NAME, COMPUTATION_NODE_NOT_FOUND_RESPONSE, \
    COMPUTATION_NODE_FETCHED_RESPONSE, DEVICE_IDENTIFIER_PARAM, DEVICE_SOFT_LIMIT_PARAM, \
    DEVICE_SOFT_LIMIT_SET_RESPONSE, DEVICE_NOT_FOUND_RESPONSE, \
    NODE_AND_DEVICE_PARAMS, DEVICE_SOFT_LIMIT_SET_RESPONSE_FAILURE


class SoftLimit(Resource):
    @swagger.operation(
        notes='This endpoint is used for setting soft limit for given device.',
        nickname='/nodes/computation_node/<string:name>/<string:device_id>/soft_limit',
        parameters=[
            COMPUTATION_NODE_PARAM_NAME,
            DEVICE_IDENTIFIER_PARAM,
            DEVICE_SOFT_LIMIT_PARAM
        ],
        responseMessages=[
            DEVICE_SOFT_LIMIT_SET_RESPONSE,
            DEVICE_SOFT_LIMIT_SET_RESPONSE_FAILURE,
            COMPUTATION_NODE_NOT_FOUND_RESPONSE
        ]
    )
    def put(self, name, device_id):
        soft_limit = request.args.get('soft_limit')
        abort_when_not_int(soft_limit)
        computation_node = abort_when_node_not_found(name)
        if int(soft_limit) < 0:
            log.error(str.format('Number is not positive: {}', soft_limit))
            abort(400)

        if not any(d['id'] == device_id for d in computation_node['backend_info']['devices']):
            log.error('There is no such device: %s', device_id)
            abort(404)

        limit_info = {
            'name': name,
            'device_id': device_id,
            'soft_limit': soft_limit
        }

        upsert_result = database.replace_soft_limit_for_device(name, device_id, limit_info)

        if upsert_result.modified_count:
            log.info('Power limit for device %s:%s was already set in a database to %s', name, device_id, soft_limit)
            log.info('Stored power limit info %s', limit_info)
        else:
            log.info('Stored power limit info %s on id %s', limit_info, upsert_result.upserted_id)

        return 'Soft limit successfully set', 201

    @swagger.operation(
        notes='This endpoint is used for getting soft limit information from database',
        nickname='/nodes/computation_node/<string:name>/<string:device_id>/soft_limit',
        parameters=NODE_AND_DEVICE_PARAMS,
        responseMessages=[
            COMPUTATION_NODE_FETCHED_RESPONSE,
            DEVICE_NOT_FOUND_RESPONSE
        ]
    )
    def get(self, name, device_id):
        result = database.get_soft_limit_for_device(name, device_id)
        if not result:
            log.info('No such device %s:%s', name, device_id)
            abort(404)
        log.info('Successfully get device %s:%s soft limit info: %s', name, device_id, result)
        return result, 200

    @swagger.operation(
        notes='This endpoint is used for removing soft limit information from database and device',
        nickname='/nodes/computation_node/<string:name>/<string:device_id>/soft_limit',
        parameters=NODE_AND_DEVICE_PARAMS,
        responseMessages=[
            COMPUTATION_NODE_FETCHED_RESPONSE,
            DEVICE_NOT_FOUND_RESPONSE,
        ]
    )
    def delete(self, name, device_id):
        result = database.delete_soft_limit_info(name, device_id)
        if not result:
            log.info('No such device %s:%s', name, device_id)
            abort(404)

        log.info('Successfully removed soft limit for device %s:%s soft limit info: %s', name, device_id,
                 result)
        return result, 200
