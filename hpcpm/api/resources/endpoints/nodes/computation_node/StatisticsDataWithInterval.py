import datetime
from flask_restful import Resource, request, abort
from flask_restful_swagger import swagger
from hpcpm.api import log
from hpcpm.api.helpers.database import database
from hpcpm.api.helpers.utils import abort_when_node_not_found, abort_when_device_not_found
from hpcpm.api.helpers.constants import COMPUTATION_NODE_PARAM_NAME, \
    DEVICE_NOT_FOUND_AND_COMPUTATION_NODE_FETCHED_RESPONSES, DEVICE_IDENTIFIER_PARAM, DATETIME_PARAM_BEGIN, \
    DATETIME_PARAM_END


class StatisticsDataWithInterval(Resource):
    @swagger.operation(
        notes='This endpoint is used for getting statistics data from database for given device and time period.',
        nickname='/nodes/computation_node/<string:name>/<string:device_id>/statistics_data',
        parameters=[
            COMPUTATION_NODE_PARAM_NAME,
            DEVICE_IDENTIFIER_PARAM,
            DATETIME_PARAM_BEGIN,
            DATETIME_PARAM_END
        ],
        responseMessages=DEVICE_NOT_FOUND_AND_COMPUTATION_NODE_FETCHED_RESPONSES
    )
    def get(self, name, device_id):
        beginning = request.args.get('date_time_begin')
        end = request.args.get('date_time_end')
        if beginning:
            try:
                beginning = datetime.datetime.strptime(beginning, '%Y-%m-%dT%H:%M')
            except ValueError:
                abort(400)
        if end:
            try:
                end = datetime.datetime.strptime(end, '%Y-%m-%dT%H:%M')
            except ValueError:
                abort(400)

        if beginning and end and beginning > end:
            abort(400)

        computation_node = abort_when_node_not_found(name)

        abort_when_device_not_found(device_id, computation_node)

        result = database.get_stats_data(name, device_id)
        if not result:
            log.info('No statistics for device %s:%s', name, device_id)
            abort(404)
        res_list = sorted([(datetime.datetime.strptime(k, '%Y-%m-%dT%H:%M'), v) for k, v in result.items()])
        if beginning:
            _, beg_index = self._find(lambda t: t[0] >= beginning, res_list)
        else:
            beg_index = 0
        if beg_index is None:
            return 404
        if end:
            _, end_index = self._find(lambda t: t[0] > end, res_list)
        else:
            end_index = None

        list_to_return = res_list[beg_index:end_index]

        list_to_return = [{k.strftime('%Y-%m-%dT%H:%M'): v} for k, v in list_to_return]
        log.info('Successfully get device %s:%s statistics data info: %s', name, device_id, list_to_return)
        return list_to_return, 200

    @staticmethod
    def _find(func, seq):
        for index, item in enumerate(seq):
            if func(item):
                return item, index
        return None, None
