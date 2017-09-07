from flask_restful import Resource
from flask_restful_swagger import swagger

from hpcpm.api.helpers.database import database
from hpcpm.api.helpers.constants import RULE_WITHDRAWN_RESPONSE
from hpcpm.api.resources.endpoints.nodes.computation_node.WithdrawRule import WithdrawRule


class WithdrawAll(Resource):
    @swagger.operation(
        notes='This endpoint is used for withdrawing all Withdrawable rules.',
        nickname='/nodes/computation_node/rules/withdraw',
        responseMessages=[
            RULE_WITHDRAWN_RESPONSE
        ]
    )
    def post(self):
        withdraw_endpoint = WithdrawRule()
        node_names = {node['name'] for node in database.get_list_of_nodes()}
        for rule in database.get_rules_of_type('Withdrawable'):
            if rule['name'] in node_names:
                withdraw_endpoint.post(rule['name'], rule['device_id'])
        return 'Rules successfully withdrawn', 201
