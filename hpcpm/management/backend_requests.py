import requests


class BackendRequests:
    def __init__(self, base_uri):
        self.base_uri = base_uri

    def get_power_limit_for_device(self, device_type, device_id):
        return requests.get('http://{0}/power_limit?{1}={2}'.format(self.base_uri, device_type, device_id))

    def set_power_limit_for_device(self, device_id, device_type, power_limit):
        return requests.put('http://{0}/power_limit'.format(self.base_uri),
                            params={device_type: device_id, 'power_limit': power_limit})

    def get_power_usage_for_device(self, device_type, device_id):
        return requests.get('http://{0}/power_usage?{1}={2}'.format(self.base_uri, device_type, device_id))
