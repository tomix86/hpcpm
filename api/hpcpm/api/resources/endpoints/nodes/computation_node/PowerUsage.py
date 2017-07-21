from flask_restful import Resource
from flask_restful_swagger import swagger

from hpcpm.api import log
from hpcpm.api.helpers.utils import get_com_node_and_dev_type
from hpcpm.api.helpers.constants import POWER_USAGE_FETCHED_RESPONSE, DEVICE_NOT_FOUND_RESPONSE, \
    NODE_AND_DEVICE_PARAMS
from hpcpm.api.helpers.requests import get_power_usage


class PowerUsage(Resource):
    @swagger.operation(
        notes='This endpoint is used for getting current power usage for given device',
        nickname='/nodes/computation_node/<string:name>/<string:device_id>/power_usage',
        parameters=NODE_AND_DEVICE_PARAMS,
        responseMessages=[
            POWER_USAGE_FETCHED_RESPONSE,
            DEVICE_NOT_FOUND_RESPONSE
        ]
    )
    def get(self, name, device_id):
        computation_node, device_type = get_com_node_and_dev_type(name, device_id)
        result = get_power_usage(computation_node['address'], computation_node['port'], device_id, device_type)
        log.info('Successfully get device %s:%s power usage info: %s', name, device_id, result)
        return result.json(), 200
