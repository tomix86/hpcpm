import requests
import json

from flask_restful import Resource, request, abort
from flask_restful_swagger import swagger

from hpcpm.api import log
from hpcpm.api.helpers.database import database


class ComputationNode(Resource):
    @swagger.operation(
        notes="This endpoint is used for registering new computation node",
        nickname="/nodes/computation_node/<string:name>",
        parameters=[
            {
                'name': 'name',
                'description': 'Computation Node name',
                'required': True,
                'allowMultiple': False,
                'dataType': 'string',
                "paramType": 'path'
            },
            {
                'name': 'address',
                'description': 'Computation Node address',
                'required': True,
                'allowMultiple': False,
                'dataType': 'string',
                "paramType": 'query'
            },
            {
                'name': 'port',
                'description': 'Computation Node port',
                'required': True,
                'allowMultiple': False,
                'dataType': 'int',
                "paramType": 'query'
            }
        ],
        responseMessages=[
            {
                "code": 201,
                "message": "Node added successfully"
            },
            {
                "code": 404,
                "message": "Computation node could not be found"
            }
        ]
    )
    def put(self, name):
        address = request.args.get('address')
        port = request.args.get('port')
        devices_query = "http://" + address + ":" + port + "/devices_list"
        try:
            response = requests.get(devices_query)
        except requests.exceptions.ConnectionError:
            log.error("Connection could not be established to %s", devices_query)
            abort(404)

        log.info("Response from %s: %s", devices_query, response.text)

        node_info = json.loads(response.text)
        node_info['name'] = name
        upsert_result = database.replace_computation_node_info(name, node_info)
        if upsert_result.modified_count:
            log.info("Node %s was already present in a database", name)
            log.info("Stored Node info %s", node_info)
        else:
            log.info("Stored Node info %s on id %s", node_info, upsert_result.upserted_id)
        return name, 201
