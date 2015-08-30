from flask_restful import Resource
from flask_restful_swagger import swagger


class Status(Resource):
    @swagger.operation(
        notes="This endpoint is used for returning API status",
        nickname="get",
        parameters=[],
        responseMessages=[
            {
                "code": 200,
                "message": "Resource found"
            },
            {
                "code": 500,
                "message": "Internal exception occurred"
            }
        ]
    )
    @staticmethod
    def get():
        return "ok"
