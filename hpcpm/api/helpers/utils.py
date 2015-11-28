from flask_restful import abort
from hpcpm.api import log
from hpcpm.api.helpers.database import database


def abort_when_port_invalid(port):
    try:
        parsed_port = int(port)
        if 0 > parsed_port > 65535:
            log.error(str.format('Port number is not valid: {}', parsed_port))
            abort(400)
    except ValueError:
        log.error(str.format('Port number is not valid: {}', port))
        abort(400)


def abort_when_not_int(number):
    try:
        int(number)
    except ValueError:
        log.error(str.format('Number is not valid: {}', number))
        abort(400)


def abort_when_node_not_found(name):
    node_info = database.get_computation_node_info(name)

    if not node_info:
        log.info('No such computation node info %s', name)
        abort(404)

    return node_info


def abort_when_device_not_found(device_id, computation_node):
    if not any(d['id'] == device_id for d in computation_node['backend_info']['devices']):
        log.error('There is no such device: %s', device_id)
        abort(404)


def get_com_node_and_dev_type(name, device_id):
    computation_node = abort_when_node_not_found(name)
    [device_type] = [d['Type'] for d in computation_node['backend_info']['devices'] if d['id'] == device_id]
    return computation_node, device_type
