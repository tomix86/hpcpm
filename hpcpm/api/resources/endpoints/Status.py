from flask_restful import Resource
from flask_restful_swagger import swagger


class Status(Resource):
    @swagger.operation(
        notes='This endpoint is used for returning API status',
        nickname='/status',
        parameters=[],
        responseMessages=[
            {
                'code': 200,
                'message': 'Resource found'
            }
        ]
    )
    def get(self):
        return 'ok'
