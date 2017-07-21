from datetime import datetime
from apscheduler.schedulers.blocking import BlockingScheduler
from hpcpm.management import log
from hpcpm.management.api_requests import ApiRequests
from hpcpm.management.backend_requests import BackendRequests
from hpcpm.management import rules
from hpcpm.management.utils import find


class App:
    def __init__(self, config):
        self.interval = int(config.get('scheduler', 'interval'))
        self.api_requests = ApiRequests(config.get('api', 'base_uri'))

    def _update_device_power_limit(self, device, node_name, backend_requests):
        device_id = device['id']
        device_type = device['Type']  # probably temporary - device_type is necessary for backend queries
        log.debug('started retrieving power limit info for %s:%s', node_name, device_id)
        power_limit_response = self.api_requests.get_power_limit_info(node_name, device_id)
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
        start_time = datetime.utcnow()
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

        finish_time = datetime.utcnow()
        log.info('check_and_set_nodes_power_limit job finished, it took %s seconds',
                 (finish_time - start_time).total_seconds())

    def gather_statistics_data(self):
        start_time = datetime.utcnow()
        log.debug('gather_statistics_data job started')

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
                self._gather_device_stats(node_name, device, backend_requests)

        finish_time = datetime.utcnow()
        log.info('gather_statistics_data job finished, it took %s seconds',
                 (finish_time - start_time).total_seconds())

    def _gather_device_stats(self, node_name, device, backend_requests):
        device_id = device['id']
        device_type = device['Type']
        response = self.api_requests.get_statistics_interval_info(node_name, device_id)
        if response.status_code != 200:
            log.debug('no statistics interval info for %s', device_id)
            return
        interval_info = response.json()
        log.debug('interval info for %s: %s', device_id, interval_info)

        next_measurement = datetime.strptime(interval_info['next_measurement'], '%Y-%m-%dT%H:%M')

        current_time = datetime.utcnow()
        if next_measurement > current_time:
            log.debug('no need to gather statistics data now')
            return
        response = backend_requests.get_power_usage_for_device(device_type, device_id)
        usage = response.json()
        self.api_requests.put_statistics_data(node_name, device_id, interval_info['next_measurement'],
                                              usage[0]['PowerUsage'])
        log.debug('stored statistics data for %s: %s', node_name, device_id)

    def run_rules(self):
        start_time = datetime.utcnow()
        log.debug('run_rules job started')

        rule_types = rules.Rule.__subclasses__()  # pylint: disable=no-member

        computation_nodes = self.api_requests.get_all_computation_nodes_info().json()
        log.info('computation nodes: %s', computation_nodes)

        for node in computation_nodes:
            node_name = node['name']

            log.debug('started retrieving node details for %s', node_name)
            node_details = self.api_requests.get_computation_node_details(node_name).json()
            log.info('node details for %s received: %s', node_name, node_details)

            for device in node_details['backend_info']['devices']:
                self._run_rule_for_device(node_name, device, rule_types)

        finish_time = datetime.utcnow()
        log.info('run_rules job finished, it took %s seconds',
                 (finish_time - start_time).total_seconds())

    def _run_rule_for_device(self, node_name, device, rule_types):
        device_id = device['id']
        rule_response = self.api_requests.get_rule_for_device(node_name, device_id)
        if rule_response.status_code != 200:
            return
        rule_info = rule_response.json()
        rule_type = rule_info['rule_type']
        obj, _ = find(lambda t, rt=rule_type: t.__name__ == rt, rule_types)
        rule = obj(self.api_requests, node_name, device_id)
        rule.proceed(rule_info['rule_params'])

    def do_work(self):
        try:
            self.gather_statistics_data()
            self.check_and_set_nodes_power_limit()
            self.run_rules()
        except Exception as ex:
            log.exception(ex)
            raise  # could add some failure recovery logic

    def run(self):
        sched = BlockingScheduler()
        sched.add_job(self.do_work, 'interval', seconds=self.interval)
        sched.start()
