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

CONSTRAINTS_FETCHED_RESPONSE = {
    'code': 200,
    'message': 'Constraints fetched successfully'
}

POWER_USAGE_FETCHED_RESPONSE = {
    'code': 200,
    'message': 'Power usage fetched successfully'
}

COMPUTATION_NODE_ADDED_RESPONSE = {
    'code': 201,
    'message': 'Node added successfully'
}

ALL_COMPUTATION_NODES_SORT_PARAM = {
    'name': 'sort',
    'description': 'Records sorting order',
    'required': False,
    'allowMultiple': False,
    'dataType': 'string',
    'paramType': 'query'
}

ALL_COMPUTATION_NODES_FILTER_PARAM = {
    'name': 'filter',
    'description': 'Node name filter',
    'required': False,
    'allowMultiple': False,
    'dataType': 'string',
    'paramType': 'query'
}

ALL_COMPUTATION_NODES_PAGINATION_PARAM = {
    'name': 'pagination',
    'description': 'Database pagination',
    'required': False,
    'allowMultiple': False,
    'dataType': 'string',
    'paramType': 'query'
}

ALL_COMPUTATION_NODES_NODE_ADDRESS_PARAM = {
    'name': 'address',
    'description': 'Address (domain, ip) of computation node',
    'required': False,
    'allowMultiple': False,
    'dataType': 'string',
    'paramType': 'query'
}


DEVICE_IDENTIFIER_PARAM = {
    'name': 'device_id',
    'description': 'Device identifier',
    'required': True,
    'allowMultiple': False,
    'dataType': 'string',
    'paramType': 'path'
}

NODE_AND_DEVICE_PARAMS = [DEVICE_IDENTIFIER_PARAM, COMPUTATION_NODE_PARAM_NAME]

DEVICE_POWER_LIMIT_PARAM = {
    'name': 'power_limit',
    'description': 'Power limit',
    'required': True,
    'allowMultiple': False,
    'dataType': 'int',
    'paramType': 'query'
}

DEVICE_SOFT_LIMIT_PARAM = {
    'name': 'soft_limit',
    'description': 'Soft limit',
    'required': True,
    'allowMultiple': False,
    'dataType': 'int',
    'paramType': 'query'
}

STATISTICS_INTERVAL_PARAM = {
    'name': 'statistics_interval',
    'description': 'Statistics gathering interval in minutes',
    'required': True,
    'allowMultiple': False,
    'dataType': 'int',
    'paramType': 'query'
}

DATETIME_PARAM = {
    'name': 'date_time',
    'description': 'Date and time',
    'required': True,
    'allowMultiple': False,
    'dataType': 'string',
    'paramType': 'path'
}

DATETIME_PARAM_BEGIN = {
    'name': 'date_time_begin',
    'description': 'Date and time',
    'required': False,
    'allowMultiple': False,
    'dataType': 'string',
    'paramType': 'query'
}

DATETIME_PARAM_END = {
    'name': 'date_time_end',
    'description': 'Date and time',
    'required': False,
    'allowMultiple': False,
    'dataType': 'string',
    'paramType': 'query'
}

POWER_USAGE_PARAM = {
    'name': 'power_usage',
    'description': 'Power usage',
    'required': True,
    'allowMultiple': False,
    'dataType': 'string',
    'paramType': 'query'
}

RULE_TYPE_PARAM = {
    'name': 'rule_type',
    'description': 'Rule type',
    'required': True,
    'allowMultiple': False,
    'dataType': 'string',
    'paramType': 'query'
}

RULE_PARAMS_PARAM = {
    'name': 'rule_params',
    'description': 'Rule parameters',
    'required': True,
    'allowMultiple': False,
    'dataType': 'json',
    'paramType': 'body'
}

DEVICE_POWER_LIMIT_SET_RESPONSE = {
    'code': 201,
    'message': 'Power limit set successfully'
}

DEVICE_SOFT_LIMIT_SET_RESPONSE = {
    'code': 201,
    'message': 'Soft limit set successfully'
}

DEVICE_POWER_LIMIT_SET_RESPONSE_FAILURE = {
    'code': 406,
    'message': 'Power limit could not be set successfully'
}

DEVICE_SOFT_LIMIT_SET_RESPONSE_FAILURE = {
    'code': 406,
    'message': 'Soft limit could not be set successfully'
}

RULE_SET_RESPONSE = {
    'code': 201,
    'message': 'Rule set successfully'
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

STATISTICS_INTERVAL_SET_RESPONSE = {
    'code': 201,
    'message': 'Statistics gathering interval set successfully'
}

STATISTICS_DATA_UPDATED_RESPONSE = {
    'code': 201,
    'message': 'Statistics data updated successfully'
}

DEVICE_NOT_FOUND_AND_COMPUTATION_NODE_FETCHED_RESPONSES = [
    DEVICE_NOT_FOUND_RESPONSE,
    COMPUTATION_NODE_FETCHED_RESPONSE
]

AVAILABLE_RULE_TYPES = [
    'TimeBased',
    'HardLimit'
]
