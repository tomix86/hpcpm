from datetime import datetime
from apscheduler.schedulers.blocking import BlockingScheduler

from hpcpm.management import log
from hpcpm.management.api_requests import ApiRequests
from hpcpm.management.backend_requests import BackendRequests


class App:
    def __init__(self, config):
        self.interval = int(config.get('scheduler', 'interval'))
        self.api_requests = ApiRequests(config.get('api', 'base_uri'))

    def _update_device_power_limit(self, device, node_name, backend_requests):
        device_id = device['id']
        device_type = device['Type']  # probably temporary - device_type is necessary for backend queries
        log.debug('started retrieving power limit info for %s:%s', node_name, device_id)
        power_limit_response = self.api_requests.get_power_limit_info_for_device(node_name, device_id)
        if power_limit_response.status_code == 404:
            log.info('no power limit info set in database for %s:%s', node_name, device_id)
        else:
            power_limit = power_limit_response.json()
            log.info('power limit info for %s:%s received: %s', node_name, device_id, power_limit)

            log.debug('started retrieving power limit info from device %s:%s', node_name, device_id)
            device_power_limit = backend_requests.get_power_limit_for_device(
                device_type, device_id).json()[0]['PowerLimit']  # Naive implementation
            log.info('power limit info from device %s:%s received: %s', node_name, device_id,
                     device_power_limit)

            if device_power_limit != int(power_limit['power_limit']):
                log.debug('setting power limit for device %s:%s to %s', node_name, device_id,
                          int(power_limit['power_limit']))
                resp = backend_requests.set_power_limit_for_device(device_type, device_id,
                                                                   int(power_limit['power_limit']))
                log.info('set power limit for device %s:%s, response: %s', node_name, device_id, resp.text)

    def check_and_set_nodes_power_limit(self):
        start_time = datetime.now()
        log.debug('check_and_set_nodes_power_limit job started')

        computation_nodes = self.api_requests.get_all_computation_nodes_info().json()
        log.info('computation nodes: %s', computation_nodes)

        for node in computation_nodes:
            node_name = node['name']

            log.debug('started retrieving node details for %s', node_name)
            node_details = self.api_requests.get_computation_node_details(node_name).json()
            log.info('node details for %s received: %s', node_name, node_details)

            node_address = node_details['address']
            node_port = node_details['port']

            backend_requests = BackendRequests('{0}:{1}'.format(node_address, node_port))

            for device in node_details['backend_info']['devices']:
                self._update_device_power_limit(device, node_name, backend_requests)

        finish_time = datetime.now()
        log.info('check_and_set_nodes_power_limit job finished, it took %s seconds',
                 (finish_time - start_time).total_seconds())

    def run(self):
        sched = BlockingScheduler()
        sched.add_job(self.check_and_set_nodes_power_limit, 'interval', seconds=self.interval)
        sched.start()
