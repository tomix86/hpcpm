import requests
import json

from flask_restful import Resource, request, abort
from flask_restful_swagger import swagger

from hpcpm.api import log
from hpcpm.api.helpers.database import database
from hpcpm.api.helpers.utils import abort_when_port_invalid
from hpcpm.api.helpers.constants import COMPUTATION_NODE_PARAM_NAME, COMPUTATION_NODE_PARAM_ADDRESS, \
    COMPUTATION_NODE_PARAM_PORT, COMPUTATION_NODE_ADDED_RESPONSE, COMPUTATION_NODE_NOT_FOUND_RESPONSE, \
    COMPUTATION_NODE_FETCHED_RESPONSE, COMPUTATION_NODE_PUT_NOT_FOUND_RESPONSE
from hpcpm.api.helpers.requests import get_devices_list, delete_power_limit


class ComputationNode(Resource):
    @swagger.operation(
        notes='This endpoint is used for registering new computation node',
        nickname='/nodes/computation_node/<string:name>',
        parameters=[
            COMPUTATION_NODE_PARAM_NAME,
            COMPUTATION_NODE_PARAM_ADDRESS,
            COMPUTATION_NODE_PARAM_PORT
        ],
        responseMessages=[
            COMPUTATION_NODE_ADDED_RESPONSE,
            COMPUTATION_NODE_PUT_NOT_FOUND_RESPONSE
        ]
    )
    def put(self, name):
        address = request.args.get('address')
        port = request.args.get('port')

        abort_when_port_invalid(port)

        node_by_ip = database.get_computation_node_info_by_address(address, port)
        if node_by_ip and node_by_ip.get('name') != name:
            log.warning('Node with IP: %s:%s is present in database: %s', address, port, node_by_ip)

        try:
            response = get_devices_list(address, port)
        except requests.exceptions.ConnectionError:
            log.error('Connection could not be established to %s:%s', address, port)
            abort(406)

        log.info('Response %s:', response.text)

        backend_info = json.loads(response.text)
        node_info = {
            'name': name,
            'address': address,
            'port': port,
            'backend_info': backend_info
        }
        upsert_result = database.replace_computation_node_info(name, node_info)
        if upsert_result.modified_count:
            log.info('Node %s was already present in a database', name)
            log.info('Stored Node info %s', node_info)
        else:
            log.info('Stored Node info %s on id %s', node_info, upsert_result.upserted_id)
        return name, 201

    @swagger.operation(
        notes='This endpoint is used for getting computation node information from database',
        nickname='/nodes/computation_node/<string:name>',
        parameters=[
            COMPUTATION_NODE_PARAM_NAME
        ],
        responseMessages=[
            COMPUTATION_NODE_FETCHED_RESPONSE,
            COMPUTATION_NODE_NOT_FOUND_RESPONSE
        ]
    )
    def get(self, name):
        result = database.get_computation_node_info(name)
        if not result:
            log.info('No such computation node %s', name)
            abort(404)
        log.info('Successfully get node %s info: %s', name, result)
        return result, 200

    @swagger.operation(
        notes='This endpoint is used for removing computation node information from database',
        nickname='/nodes/computation_node/<string:name>',
        parameters=[
            COMPUTATION_NODE_PARAM_NAME
        ],
        responseMessages=[
            COMPUTATION_NODE_FETCHED_RESPONSE,
            COMPUTATION_NODE_NOT_FOUND_RESPONSE
        ]
    )
    def delete(self, name):
        result_node_info = database.delete_computation_node_info(name)
        result_power_limit_info = database.delete_power_limit_infos(name)

        if not result_node_info:
            log.info('No such computation node %s', name)
            abort(404)
        if not result_power_limit_info:
            log.info('No such power limit info for node %s', name)
            abort(404)

        address = result_node_info.get('address')
        port = result_node_info.get('port')
        abort_when_port_invalid(port)

        for device in result_node_info['backend_info']['devices']:
            try:
                response = delete_power_limit(address, port, device['id'])
                log.info('Device %s deletion info: %s', device['id'], response)
            except requests.exceptions.ConnectionError:
                log.error('Connection could not be established to %s:%s', address, port)
                abort(406)

        log.info('Successfully deleted node %s info and its power limit: %s %s', name, result_node_info,
                 result_power_limit_info)
        return 204
