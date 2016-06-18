import json
from flask import request
from flask_restful import Resource
from flask_restful_swagger import swagger

from hpcpm.api import log
from hpcpm.api.helpers.database import database
from hpcpm.api.helpers.constants import ALL_DEVICES_GET_OK_RESPONSE, ALL_COMPUTATION_NODES_SORT_PARAM, \
    ALL_COMPUTATION_NODES_FILTER_PARAM, ALL_COMPUTATION_NODES_PAGINATION_PARAM, ALL_COMPUTATION_NODES_NODE_ADDRESS_PARAM


class AllComputationNodes(Resource):
    @swagger.operation(
        notes='This endpoint is used for retrieving all computation nodes info',
        nickname='/nodes/computation_nodes',
        parameters=[
            ALL_COMPUTATION_NODES_SORT_PARAM,
            ALL_COMPUTATION_NODES_FILTER_PARAM,
            ALL_COMPUTATION_NODES_PAGINATION_PARAM,
            ALL_COMPUTATION_NODES_NODE_ADDRESS_PARAM
        ],
        responseMessages=[
            ALL_DEVICES_GET_OK_RESPONSE
        ]
    )
    def get(self):
        sorting_order_param = request.args.get('sort')
        name_filter_param = request.args.get('filter')
        pagination_param = request.args.get('pagination')
        address_param = request.args.get('address')
        sorting_order = None
        name_filter = None
        pagination = None
        address = None
        if sorting_order_param:
            sorting_order = json.loads(sorting_order_param)
        if name_filter_param:
            name_filter = json.loads(name_filter_param)
        if pagination_param:
            pagination = json.loads(pagination_param)
        if address_param:
            address = json.loads(address_param)
        result = database.get_list_of_nodes(sorting_order, name_filter, pagination, address)
        log.info('Got all nodes fetched: %s', result)
        return result, 200
