from flask_restful import Resource
from flask_restful_swagger import swagger

from hpcpm.api import log
from hpcpm.api.helpers.utils import get_com_node_and_dev_type
from hpcpm.api.helpers.constants import CONSTRAINTS_FETCHED_RESPONSE, DEVICE_NOT_FOUND_RESPONSE, \
    NODE_AND_DEVICE_PARAMS
from hpcpm.api.helpers.requests import get_constraints


class PowerLimitConstraints(Resource):
    @swagger.operation(
        notes='This endpoint is used for getting current power usage for given device',
        nickname='/nodes/computation_node/<string:name>/<string:device_id>/power_limit_constraints',
        parameters=NODE_AND_DEVICE_PARAMS,
        responseMessages=[
            DEVICE_NOT_FOUND_RESPONSE,
            CONSTRAINTS_FETCHED_RESPONSE
        ]
    )
    def get(self, name, device_id):
        computation_node, dev_type = get_com_node_and_dev_type(name, device_id)
        result = get_constraints(computation_node['address'], computation_node['port'], device_id, dev_type)
        log.info('Successfully get device %s:%s power limit constraints info: %s', name, device_id, result)
        return result.json(), 200
