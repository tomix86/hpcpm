import requests
import json

from flask_restful import Resource, request, abort
from flask_restful_swagger import swagger

from hpcpm.api import log
from hpcpm.api.helpers.database import database
from hpcpm.api.helpers.utils import abort_when_port_invalid, COMPUTATION_NODE_PARAM, \
    COMPUTATION_NODE_NOT_FOUND_RESPONSE, COMPUTATION_NODE_FETCHED_RESPONSE


class ComputationNode(Resource):
    @swagger.operation(
        notes='This endpoint is used for registering new computation node',
        nickname='/nodes/computation_node/<string:name>',
        parameters=[
            COMPUTATION_NODE_PARAM,
            {
                'name': 'address',
                'description': 'Computation Node address',
                'required': True,
                'allowMultiple': False,
                'dataType': 'string',
                'paramType': 'query'
            },
            {
                'name': 'port',
                'description': 'Computation Node port',
                'required': True,
                'allowMultiple': False,
                'dataType': 'int',
                'paramType': 'query'
            }
        ],
        responseMessages=[
            {
                'code': 201,
                'message': 'Node added successfully'
            },
            {
                'code': 406,
                'message': 'Computation node could not be found'
            }
        ]
    )
    def put(self, name):
        address = request.args.get('address')
        port = request.args.get('port')

        abort_when_port_invalid(port)

        node_by_ip = database.get_computation_node_info_by_address(address, port)
        if node_by_ip and node_by_ip.get('name') != name:
            log.warning(str.format('Node with IP: {}:{} is present in database: {}', address, port, node_by_ip))

        devices_query = str.format('http://{}:{}/devices_list', address, port)
        try:
            response = requests.get(devices_query)
        except requests.exceptions.ConnectionError:
            log.error(str.format('Connection could not be established to {}', devices_query))
            abort(406)

        log.info(str.format('Response from {}: {}', devices_query, response.text))

        backend_info = json.loads(response.text)
        node_info = {
            'name': name,
            'address': address,
            'port': port,
            'backend_info': backend_info
        }
        upsert_result = database.replace_computation_node_info(name, node_info)
        if upsert_result.modified_count:
            log.info(str.format('Node {} was already present in a database', name))
            log.info(str.format('Stored Node info {}', node_info))
        else:
            log.info(str.format('Stored Node info {} on id {}', node_info, upsert_result.upserted_id))
        return name, 201

    @swagger.operation(
        notes='This endpoint is used for getting computation node information from database',
        nickname='/nodes/computation_node/<string:name>',
        parameters=[
            COMPUTATION_NODE_PARAM
        ],
        responseMessages=[
            COMPUTATION_NODE_FETCHED_RESPONSE,
            COMPUTATION_NODE_NOT_FOUND_RESPONSE
        ]
    )
    def get(self, name):
        result = database.get_computation_node_info(name)
        if not result:
            log.info(str.format('No such computation node {}', name))
            abort(404)
        log.info(str.format('Successfully get node {} info: {}', name, result))
        return result, 200

    @swagger.operation(
        notes='This endpoint is used for removing computation node information from database',
        nickname='/nodes/computation_node/<string:name>',
        parameters=[
            COMPUTATION_NODE_PARAM
        ],
        responseMessages=[
            COMPUTATION_NODE_FETCHED_RESPONSE,
            COMPUTATION_NODE_NOT_FOUND_RESPONSE
        ]
    )
    def delete(self, name):
        result_node_info = database.delete_computation_node_info(name)
        result_power_limit_info = database.delete_power_limit_info(name)
        if not result_node_info:
            log.info(str.format('No such computation node {}', name))
            abort(404)
        if not result_power_limit_info:
            log.info(str.format('No such power limit info for node {}', name))
            abort(404)

        address = result_node_info.get('address')
        port = result_node_info.get('port')
        abort_when_port_invalid(port)

        deletion_query = str.format('http://{}:{}/power_limit', address, port)
        for device in result_node_info['backend_info']['devices']:
            try:
                response = requests.delete(deletion_query, params={'device_id': device['id']})
                log.info(str.format('Device {} deletion info: {}', device['id'], response))
            except requests.exceptions.ConnectionError:
                log.error(str.format('Connection could not be established to {}', deletion_query))
                abort(406)

        log.info(str.format('Successfully deleted node {} info and its power limit: {} {}', name, result_node_info,
                            result_power_limit_info))
        return 204
