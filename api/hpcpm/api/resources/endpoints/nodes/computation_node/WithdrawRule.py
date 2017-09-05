from flask_restful import Resource
from flask_restful_swagger import swagger


from hpcpm.api import log
from hpcpm.api.helpers.database import database
from hpcpm.api.helpers.constants import COMPUTATION_NODE_PARAM_NAME, COMPUTATION_NODE_NOT_FOUND_RESPONSE, \
    DEVICE_IDENTIFIER_PARAM, RULE_WITHDRAWN_RESPONSE


class WithdrawRule(Resource):
    @swagger.operation(
        notes='This endpoint is used for withdrawing Withdrawable rule for given device.',
        nickname='/nodes/computation_node/<string:name>/<string:device_id>/rule/withdraw',
        parameters=[
            COMPUTATION_NODE_PARAM_NAME,
            DEVICE_IDENTIFIER_PARAM
        ],
        responseMessages=[
            RULE_WITHDRAWN_RESPONSE,
            COMPUTATION_NODE_NOT_FOUND_RESPONSE

        ]
    )
    def post(self, name: str, device_id: str):
        rule = database.get_rule_for_device(name, device_id)
        if rule and rule.get('rule_type', "") == 'Withdrawable':
            rule['rule_params']['withdraw'] = True
            database.replace_rule_for_device(name, device_id, rule)
            return 'Rule successfully withdrawn', 201
        log.info('Rule on the device was not Withdrawable')
        return 'Rule was not Withdrawable', 201
