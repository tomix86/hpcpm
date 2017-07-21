from flask_restful import Resource, abort
from flask_restful_swagger import swagger


from hpcpm.api import log
from hpcpm.api.helpers.database import database
from hpcpm.api.helpers.utils import abort_when_node_not_found, abort_when_device_not_found
from hpcpm.api.helpers.constants import COMPUTATION_NODE_PARAM_NAME, DEVICE_IDENTIFIER_PARAM, \
    DEVICE_NOT_FOUND_AND_COMPUTATION_NODE_FETCHED_RESPONSES


class LastPowerUsage(Resource):
    @swagger.operation(
        notes='This endpoint is used for getting last power usage of given device from database',
        nickname='/nodes/computation_node/<string:name>/<string:device_id>/last_power_usage',
        parameters=[
            COMPUTATION_NODE_PARAM_NAME,
            DEVICE_IDENTIFIER_PARAM,
        ],
        responseMessages=DEVICE_NOT_FOUND_AND_COMPUTATION_NODE_FETCHED_RESPONSES
    )
    def get(self, name, device_id):
        computation_node = abort_when_node_not_found(name)

        abort_when_device_not_found(device_id, computation_node)

        db_records = database.get_last_power_usage(name, device_id)
        if not db_records or len(db_records) <= 0:
            log.info('No last power usage for device %s:%s', name, device_id)
            abort(404)

        sorted_result = sorted(zip(db_records.keys(), db_records.values()), key=lambda usage: usage[0], reverse=True)
        sorted_result = [u[1] for u in sorted_result if isinstance(u[1], int)]

        result = {'last_power_usage': sorted_result[0]}

        log.info('Successfully get device %s:%s last power usage info: %s', name, device_id, result)
        return result, 200
