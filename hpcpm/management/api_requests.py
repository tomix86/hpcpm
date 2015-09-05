import requests


class ApiRequests:
    def __init__(self, base_uri):
        self.base_uri = base_uri

    def get_all_computation_nodes_info(self):
        return requests.get('http://{0}/nodes/computation_nodes'.format(self.base_uri))

    def get_computation_node_details(self, node_name):
        return requests.get('http://{0}/nodes/computation_node/{1}'.format(self.base_uri, node_name))

    def get_power_limit_info_for_device(self, node_name, device_id):
        return requests.get(
            'http://{0}/nodes/computation_node/{1}/{2}/power_limit'.format(self.base_uri, node_name, device_id))
