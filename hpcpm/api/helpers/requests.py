import requests


def get_node_information(address, port):
    devices_query = str.format('http://{}:{}/node_information', address, port)
    return requests.get(devices_query)


def delete_power_limit(address, port, device_id):
    deletion_query = str.format('http://{}:{}/power_limit', address, port)
    return requests.delete(deletion_query, params={'device_id': device_id})


def put_power_limit(address, port, device_id, device_type, power_limit):
    power_limit_query = str.format('http://{}:{}/power_limit?{},{}={}', address, port,
                                   device_type, device_id, power_limit)
    return requests.put(power_limit_query)
