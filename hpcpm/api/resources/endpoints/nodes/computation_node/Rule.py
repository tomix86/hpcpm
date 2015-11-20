from flask_restful import Resource, request, abort
from flask_restful_swagger import swagger

from hpcpm.api import log
from hpcpm.api.helpers.database import database
from hpcpm.api.helpers.utils import abort_when_node_not_found
from hpcpm.api.helpers.constants import COMPUTATION_NODE_PARAM_NAME, COMPUTATION_NODE_NOT_FOUND_RESPONSE, \
    COMPUTATION_NODE_FETCHED_RESPONSE, DEVICE_IDENTIFIER_PARAM, RULE_TYPE_PARAM, RULE_PARAMS_PARAM, \
    RULE_SET_RESPONSE, DEVICE_NOT_FOUND_RESPONSE, \
    NODE_AND_DEVICE_PARAMS, AVAILABLE_RULE_TYPES, DEVICE_NOT_FOUND_AND_COMPUTATION_NODE_FETCHED_RESPONSES


class Rule(Resource):
    @swagger.operation(
        notes='This endpoint is used for setting rule for given device.',
        nickname='/nodes/computation_node/<string:name>/<string:device_id>/rule',
        parameters=[
            COMPUTATION_NODE_PARAM_NAME,
            DEVICE_IDENTIFIER_PARAM,
            RULE_PARAMS_PARAM,
            RULE_TYPE_PARAM
        ],
        responseMessages=[
            COMPUTATION_NODE_NOT_FOUND_RESPONSE,
            RULE_SET_RESPONSE
        ]
    )
    def put(self, name, device_id):
        rule_type = request.args.get('rule_type')
        rule_params = request.json
        if rule_type not in AVAILABLE_RULE_TYPES:
            abort(400)
        computation_node = abort_when_node_not_found(name)

        if not any(d['id'] == device_id for d in computation_node['backend_info']['devices']):
            log.error('There is no such device: %s', device_id)
            abort(404)

        database.replace_rule_for_device(name, device_id, {'name': name, 'device_id': device_id, 'rule_type': rule_type,
                                                           'rule_params': rule_params})
        return 'Rule successfully set', 201

    @swagger.operation(
        notes='This endpoint is used for getting rule information from database',
        nickname='/nodes/computation_node/<string:name>/<string:device_id>/rule',
        parameters=NODE_AND_DEVICE_PARAMS,
        responseMessages=DEVICE_NOT_FOUND_AND_COMPUTATION_NODE_FETCHED_RESPONSES

    )
    def get(self, name, device_id):
        result = database.get_rule_for_device(name, device_id)
        if not result:
            log.info('No such device %s:%s', name, device_id)
            abort(404)
        log.info('Successfully get device %s:%s rule: %s', name, device_id, result)
        return result, 200

    @swagger.operation(
        notes='This endpoint is used for removing rule information from database and device',
        nickname='/nodes/computation_node/<string:name>/<string:device_id>/rule',
        parameters=NODE_AND_DEVICE_PARAMS,
        responseMessages=[
            COMPUTATION_NODE_FETCHED_RESPONSE,
            DEVICE_NOT_FOUND_RESPONSE,
            ]
    )
    def delete(self, name, device_id):
        computation_node = abort_when_node_not_found(name)
        if not any(d['id'] == device_id for d in computation_node['backend_info']['devices']):
            log.error('There is no such device: %s', device_id)
            abort(404)
        result = database.delete_rule(name, device_id)
        return result, 200
