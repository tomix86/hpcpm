import datetime
from flask_restful import Resource, request, abort
from flask_restful_swagger import swagger
from hpcpm.api import log
from hpcpm.api.helpers.database import database
from hpcpm.api.helpers.utils import abort_when_not_int, abort_when_node_not_found, abort_when_device_not_found
from hpcpm.api.helpers.constants import COMPUTATION_NODE_PARAM_NAME, COMPUTATION_NODE_NOT_FOUND_RESPONSE, \
    DEVICE_NOT_FOUND_AND_COMPUTATION_NODE_FETCHED_RESPONSES, DEVICE_IDENTIFIER_PARAM, DATETIME_PARAM, \
    STATISTICS_DATA_UPDATED_RESPONSE, POWER_USAGE_PARAM


class StatisticsData(Resource):
    @swagger.operation(
        notes='This endpoint is used for setting statistics data for given device and time.',
        nickname='/nodes/computation_node/<string:name>/<string:device_id>/statistics_data/<string:date_time>',
        parameters=[
            COMPUTATION_NODE_PARAM_NAME,
            DEVICE_IDENTIFIER_PARAM,
            DATETIME_PARAM,
            POWER_USAGE_PARAM
        ],
        responseMessages=[
            COMPUTATION_NODE_NOT_FOUND_RESPONSE,
            STATISTICS_DATA_UPDATED_RESPONSE
        ]
    )
    def put(self, name, device_id, date_time):
        power_usage = request.args.get('power_usage')
        abort_when_not_int(power_usage)
        power_usage = int(power_usage)

        try:
            datetime.datetime.strptime(date_time, '%Y-%m-%dT%H:%M')
        except ValueError:
            abort(400)

        computation_node = abort_when_node_not_found(name)

        abort_when_device_not_found(device_id, computation_node)

        if database.check_stats_present(name, device_id):
            database.update_stats_data(name, device_id, {date_time: power_usage})
            log.debug('Stats updated for device %s:%s, date: %s, power usage: %s', name, device_id, date_time,
                      power_usage)
        else:
            database.replace_stats_data(name, device_id, {'name': name, 'device_id': device_id, date_time: power_usage})
            log.debug('Stats set for device %s:%s, date: %s, power usage: %s', name, device_id, date_time, power_usage)

        interval_info = database.get_stats_interval_info(name, device_id)
        interval = datetime.timedelta(minutes=interval_info['interval'])
        new_next_measurement = (datetime.datetime.utcnow() + interval)
        new_next_measurement_str = new_next_measurement.strftime('%Y-%m-%dT%H:%M')  # pylint: disable=no-member
        interval_info['next_measurement'] = new_next_measurement_str

        database.replace_stats_interval_info(name, device_id, interval_info)

        return 'Statistics data successfully set', 201

    @swagger.operation(
        notes='This endpoint is used for getting statistics data from database for given device and time.',
        nickname='/nodes/computation_node/<string:name>/<string:device_id>/statistics_data/<string:date_time>',
        parameters=[
            COMPUTATION_NODE_PARAM_NAME,
            DEVICE_IDENTIFIER_PARAM,
            DATETIME_PARAM
        ],
        responseMessages=DEVICE_NOT_FOUND_AND_COMPUTATION_NODE_FETCHED_RESPONSES
    )
    def get(self, name, device_id, date_time):
        try:
            datetime.datetime.strptime(date_time, '%Y-%m-%dT%H:%M')
        except ValueError:
            abort(400)

        computation_node = abort_when_node_not_found(name)

        abort_when_device_not_found(device_id, computation_node)

        result = database.get_stats_data_for_time(name, device_id, date_time)

        if not result:
            log.info('No such statistic for device %s:%s', name, device_id)
            abort(404)
        if not result.get(date_time):
            log.info('No statistics for %s', date_time)
            abort(404)
        log.info('Successfully get device %s:%s statistics data info: %s', name, device_id, result)
        return result, 200

    @swagger.operation(
        notes='This endpoint is used for removing statistics data from database',
        nickname='/nodes/computation_node/<string:name>/<string:device_id>/statistics_data/<string:date_time>',
        parameters=[
            COMPUTATION_NODE_PARAM_NAME,
            DEVICE_IDENTIFIER_PARAM,
            DATETIME_PARAM
        ],
        responseMessages=DEVICE_NOT_FOUND_AND_COMPUTATION_NODE_FETCHED_RESPONSES
    )
    def delete(self, name, device_id, date_time):
        try:
            datetime.datetime.strptime(date_time, '%Y-%m-%dT%H:%M')
        except ValueError:
            abort(400)

        computation_node = abort_when_node_not_found(name)
        abort_when_device_not_found(device_id, computation_node)

        result = database.delete_stats_for_time(name, device_id, {date_time: ""})
        if not result:
            log.info('No such statistic for device %s:%s', name, device_id)
            abort(404)
        if not result.matched_count:
            log.info('Did not manage to delete statistic for device %s:%s', name, device_id)
            abort(404)
        log.info('Successfully deleted device %s:%s statistics data info', name, device_id)
        return 204
