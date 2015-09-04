from flask_restful import Resource
from flask_restful_swagger import swagger

from hpcpm.api import log
from hpcpm.api.helpers.database import database


class AllComputationNodes(Resource):
    @swagger.operation(
        notes="This endpoint is used for retrieving all computation nodes info",
        nickname="/nodes/computation_nodes",
        parameters=[],
        responseMessages=[
            {
                "code": 200,
                "message": "Ok"
            }
        ]
    )
    def get(self):
        result = database.get_list_of_nodes()
        log.info('Got all nodes fetched: %s', result)
        return result, 200
