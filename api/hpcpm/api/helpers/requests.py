import requests


def get_node_information(address, port):
    devices_query = str.format('http://{}:{}/node_information', address, port)
    return requests.get(devices_query)


def delete_power_limit(address, port, device_id, device_type):
    deletion_query = str.format('http://{}:{}/power_limit?{},{}', address, port, device_type, device_id)
    return requests.delete(deletion_query)


def put_power_limit(address, port, device_id, device_type, power_limit):
    power_limit_query = str.format('http://{}:{}/power_limit?{},{}={}', address, port,
                                   device_type, device_id, power_limit)
    return requests.put(power_limit_query)


def get_power_limit(address, port, device_id, device_type):
    power_limit_query = str.format('http://{}:{}/power_limit?{},{}', address, port, device_type, device_id)
    return requests.get(power_limit_query)


def get_power_usage(address, port, device_id, device_type):
    power_usage_query = str.format('http://{}:{}/power_usage?{},{}', address, port, device_type, device_id)
    return requests.get(power_usage_query)


def get_constraints(address, port, device_id, device_type):
    constraints_query = str.format('http://{}:{}/power_limit_constraints?{},{}', address, port, device_type, device_id)
    return requests.get(constraints_query)
