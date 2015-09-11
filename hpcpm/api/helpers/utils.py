from flask_restful import abort
from hpcpm.api import log


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
