from flask_restful import Resource
from flask_restful_swagger import swagger

from hpcpm.api.helpers.database import database
from hpcpm.api.helpers.constants import RULES_DELETED_RESPONSE
from hpcpm.api.resources.endpoints.nodes.computation_node.Rule import Rule


class AllRules(Resource):
    @swagger.operation(
        notes='This endpoint is used for removing all rules from database and devices',
        nickname='/nodes/computation_node/rules',
        responseMessages=[
            RULES_DELETED_RESPONSE,
            ]
    )
    def delete(self):
        rule_endpoint = Rule()
        node_names = {node['name'] for node in database.get_list_of_nodes()}
        for rule in database.get_rules():
            if rule['name'] in node_names:
                rule_endpoint.delete(rule['name'], rule['device_id'])
        return 'Rules successfully deleted.'
