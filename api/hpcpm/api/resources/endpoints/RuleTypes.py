from flask_restful import Resource
from flask_restful_swagger import swagger

from hpcpm.api.helpers.constants import AVAILABLE_RULE_TYPES


class RuleTypes(Resource):
    @swagger.operation(
        notes='This endpoint is used for returning available power management rule types',
        nickname='/rule_types',
        parameters=[],
        responseMessages=[
            {
                'code': 200,
                'message': 'Rule types found'
            }
        ]
    )
    def get(self):
        return AVAILABLE_RULE_TYPES, 200
