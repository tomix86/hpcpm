import datetime
from flask_restful import Resource, request, abort
from flask_restful_swagger import swagger
from hpcpm.api import log
from hpcpm.api.helpers.database import database
from hpcpm.api.helpers.utils import abort_when_not_int, abort_when_node_not_found
from hpcpm.api.helpers.constants import COMPUTATION_NODE_PARAM_NAME, COMPUTATION_NODE_NOT_FOUND_RESPONSE, \
    COMPUTATION_NODE_FETCHED_RESPONSE, DEVICE_IDENTIFIER_PARAM, STATISTICS_INTERVAL_PARAM, \
    STATISTICS_INTERVAL_SET_RESPONSE, DEVICE_NOT_FOUND_RESPONSE, NODE_AND_DEVICE_PARAMS


class StatisticsInterval(Resource):
    @swagger.operation(
        notes='This endpoint is used for setting statistics gathering interval for given device.',
        nickname='/nodes/computation_node/<string:name>/<string:device_id>/statistics_interval',
        parameters=[
            COMPUTATION_NODE_PARAM_NAME,
            DEVICE_IDENTIFIER_PARAM,
            STATISTICS_INTERVAL_PARAM
        ],
        responseMessages=[
            COMPUTATION_NODE_NOT_FOUND_RESPONSE,
            STATISTICS_INTERVAL_SET_RESPONSE
        ]
    )
    def put(self, name, device_id):
        statistics_interval = request.args.get('statistics_interval')
        abort_when_not_int(statistics_interval)
        statistics_interval = int(statistics_interval)
        computation_node = abort_when_node_not_found(name)

        if not any(d['id'] == device_id for d in computation_node['backend_info']['devices']):
            log.error('There is no such device: %s', device_id)
            abort(404)

        current_interval_info = database.get_stats_interval_info(name, device_id)
        if current_interval_info:
            old_interval = datetime.timedelta(minutes=current_interval_info['interval'])
            old_next_measurement = datetime.datetime.strptime(current_interval_info['next_measurement'],
                                                              '%Y-%m-%dT%H:%M')
            new_interval = datetime.timedelta(minutes=statistics_interval)
            new_next_measurement = (old_next_measurement - old_interval + new_interval)
            interval_info = current_interval_info
            interval_info['interval'] = statistics_interval
            new_next_measurement_str = new_next_measurement.strftime('%Y-%m-%dT%H:%M')  # pylint: disable=no-member
            interval_info['next_measurement'] = new_next_measurement_str
        else:
            interval_info = {
                'name': name,
                'device_id': device_id,
                'interval': statistics_interval,
                'next_measurement': (datetime.datetime.utcnow() + datetime.timedelta(
                    minutes=statistics_interval)).strftime('%Y-%m-%dT%H:%M')  # pylint: disable=no-member
            }

        upsert_result = database.replace_stats_interval_info(name, device_id, interval_info)
        if upsert_result.modified_count:
            log.info('Statistics gathering interval for device %s:%s was already set in a database', name, device_id)
            log.info('Stored statistics gathering interval info %s', interval_info)
        else:
            log.info('Stored statistics gathering interval info %s on id %s', interval_info, upsert_result.upserted_id)

        return 'Statistics gathering interval successfully set', 201

    @swagger.operation(
        notes='This endpoint is used for getting statistics gathering interval information from database.',
        nickname='/nodes/computation_node/<string:name>/<string:device_id>/statistics_interval',
        parameters=NODE_AND_DEVICE_PARAMS,
        responseMessages=[
            DEVICE_NOT_FOUND_RESPONSE,
            COMPUTATION_NODE_FETCHED_RESPONSE
        ]
    )
    def get(self, name, device_id):
        result = database.get_stats_interval_info(name, device_id)
        if not result:
            log.info('No such device %s:%s', name, device_id)
            abort(404)
        log.info('Successfully get device %s:%s statistics gathering interval info: %s', name, device_id, result)
        return result, 200

    @swagger.operation(
        notes='This endpoint is used for removing statistics gathering interval information from database',
        nickname='/nodes/computation_node/<string:name>/<string:device_id>/statistics_interval',
        parameters=NODE_AND_DEVICE_PARAMS,
        responseMessages=[
            DEVICE_NOT_FOUND_RESPONSE,
            COMPUTATION_NODE_FETCHED_RESPONSE
        ]
    )
    def delete(self, name, device_id):
        abort_when_node_not_found(name)

        result = database.delete_stats_interval_info(name, device_id)
        if not result:
            log.info('No such device %s:%s', name, device_id)
            abort(404)

        log.info('Successfully removed statistics gathering interval \
        for device %s:%s statistics gathering interval info: %s', name, device_id, result)
        return result, 200
