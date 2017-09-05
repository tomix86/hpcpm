from flask_restful import Resource, request, abort
from flask_restful_swagger import swagger

import requests

from hpcpm.api import log
from hpcpm.api.helpers.database import database
from hpcpm.api.helpers.utils import abort_when_node_not_found, get_com_node_and_dev_type
from hpcpm.api.helpers.requests import delete_power_limit, get_constraints
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
    def put(self, name: str, device_id: str):
        rule_type = request.args.get('rule_type')
        rule_params = request.json
        if rule_type not in AVAILABLE_RULE_TYPES:
            abort(400)
        computation_node = abort_when_node_not_found(name)

        if not any(d['id'] == device_id for d in computation_node['backend_info']['devices']):
            log.error('There is no such device: %s', device_id)
            abort(404)

        limit = rule_params.get("limit")
        if limit in {"HIGH", "MEDIUM", "LOW"}:
            computation_node, dev_type = get_com_node_and_dev_type(name, device_id)
            constraints = get_constraints(
                computation_node['address'],
                computation_node['port'], device_id, dev_type).json()[0]["PowerLimitConstraints"]
            lower = constraints["lower"]
            upper = constraints["upper"]
            if limit == "HIGH":
                rule_params["limit"] = upper
            elif limit == "MEDIUM":
                rule_params["limit"] = (upper + lower) / 2
            elif limit == "LOW":
                rule_params["limit"] = lower

        if rule_type == "Withdrawable":
            previous_rule = database.get_rule_for_device(name, device_id)
            if previous_rule:
                if previous_rule["rule_type"] == "Withdrawable":
                    previous_rule = previous_rule["rule_params"].get("previous_rule", None)
                rule_params["previous_rule"] = previous_rule

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
        address = computation_node['address']
        port = computation_node['port']

        if not any(d['id'] == device_id for d in computation_node['backend_info']['devices']):
            log.error('There is no such device: %s', device_id)
            abort(404)

        [device_type] = [d['Type'] for d in computation_node['backend_info']['devices'] if d['id'] == device_id]
        result = database.delete_rule(name, device_id)

        try:
            resp = delete_power_limit(address, port, device_id, device_type)
            log.info('Power limit for device %s deletion info: %s', device_id, resp)
        except requests.exceptions.ConnectionError:
            log.error('Connection could not be established to node %s:%s', address, port)
            return 'Warning: power limit was deleted from database but could not be deleted from device!', 406
        return result, 200
