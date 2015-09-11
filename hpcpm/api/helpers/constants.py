COMPUTATION_NODE_PARAM_NAME = {
    'name': 'name',
    'description': 'Computation Node name',
    'required': True,
    'allowMultiple': False,
    'dataType': 'string',
    'paramType': 'path'
}

COMPUTATION_NODE_PARAM_ADDRESS = {
    'name': 'address',
    'description': 'Computation Node address',
    'required': True,
    'allowMultiple': False,
    'dataType': 'string',
    'paramType': 'query'
}

COMPUTATION_NODE_PARAM_PORT = {
    'name': 'port',
    'description': 'Computation Node port',
    'required': True,
    'allowMultiple': False,
    'dataType': 'int',
    'paramType': 'query'
}

COMPUTATION_NODE_NOT_FOUND_RESPONSE = {
    'code': 404,
    'message': 'Computation node could not be found'
}

COMPUTATION_NODE_PUT_NOT_FOUND_RESPONSE = {
    'code': 406,
    'message': 'Computation node could not be found'
}

COMPUTATION_NODE_FETCHED_RESPONSE = {
    'code': 200,
    'message': 'Node info fetched successfully'
}

COMPUTATION_NODE_ADDED_RESPONSE = {
    'code': 201,
    'message': 'Node added successfully'
}

DEVICE_IDENTIFIER_PARAM = {
    'name': 'device_id',
    'description': 'Device identifier',
    'required': True,
    'allowMultiple': False,
    'dataType': 'string',
    'paramType': 'path'
}

DEVICE_POWER_LIMIT_PARAM = {
    'name': 'power_limit',
    'description': 'Power limit',
    'required': True,
    'allowMultiple': False,
    'dataType': 'int',
    'paramType': 'query'
}

DEVICE_POWER_LIMIT_SET_RESPONSE = {
    'code': 201,
    'message': 'Power limit set successfully'
}

DEVICE_NOT_FOUND_RESPONSE = {
    'code': 404,
    'message': 'Device could not be found'
}

POWER_LIMIT_DELETED_FROM_DB_BUT_NOT_FROM_DEVICE = {
    'code': 406,
    'message': 'Power limit was deleted from database but could not be deleted from device'
}

ALL_DEVICES_GET_OK_RESPONSE = {
    'code': 200,
    'message': 'Ok'
}