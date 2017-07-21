import requests


class ApiRequests:
    def __init__(self, base_uri):
        self.base_uri = base_uri

    def get_all_computation_nodes_info(self):
        return requests.get('http://{0}/nodes/computation_nodes'.format(self.base_uri))

    def get_computation_node_details(self, node_name):
        return requests.get('http://{0}/nodes/computation_node/{1}'.format(self.base_uri, node_name))

    def get_power_limit_info(self, node_name, device_id):
        return requests.get(
            'http://{0}/nodes/computation_node/{1}/{2}/power_limit'.format(self.base_uri, node_name, device_id))

    def put_power_limit_info(self, node_name, device_id, limit):
        return requests.put(
            'http://{0}/nodes/computation_node/{1}/{2}/power_limit'.format(self.base_uri, node_name, device_id),
            params={'power_limit': limit})

    def delete_power_limit_info(self, node_name, device_id):
        return requests.delete(
            'http://{0}/nodes/computation_node/{1}/{2}/power_limit'.format(self.base_uri, node_name, device_id))

    def get_statistics_interval_info(self, node_name, device_id):
        return requests.get(
            'http://{0}/nodes/computation_node/{1}/{2}/statistics_interval'.format(self.base_uri, node_name, device_id))

    def put_statistics_data(self, node_name, device_id, date_time, power_usage):
        return requests.put(
            'http://{0}/nodes/computation_node/{1}/{2}/statistics_data/{3}'.format(self.base_uri, node_name, device_id,
                                                                                   date_time),
            params={'power_usage': power_usage})

    def get_rule_for_device(self, node_name, device_id):
        return requests.get(
            'http://{0}/nodes/computation_node/{1}/{2}/rule'.format(self.base_uri, node_name, device_id))
