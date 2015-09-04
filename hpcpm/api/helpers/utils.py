from flask_restful import abort
from hpcpm.api import log


def abort_when_port_invalid(port):
    try:
        parsed_port = int(port)
        if 0 > parsed_port > 65535:
            log.error('Port number is not valid: %s', parsed_port)
            abort(400)
    except ValueError:
        log.error('Port number is not valid: %s', port)
        abort(400)


def abort_when_not_int(number):
    try:
        int(number)
    except ValueError:
        log.error('Number is not valid: %s', number)
        abort(400)


COMPUTATION_NODE_PARAM = {
    'name': 'name',
    'description': 'Computation Node name',
    'required': True,
    'allowMultiple': False,
    'dataType': 'string',
    "paramType": 'path'
}

COMPUTATION_NODE_NOT_FOUND_RESPONSE = {
    "code": 404,
    "message": "Computation node could not be found"
}

COMPUTATION_NODE_FETCHED_RESPONSE = {
    "code": 200,
    "message": "Node info fetched successfully"
}
