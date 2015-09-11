from flask_restful import Resource
from flask_restful_swagger import swagger

from hpcpm.api import log
from hpcpm.api.helpers.database import database
from hpcpm.api.helpers.constants import ALL_DEVICES_GET_OK_RESPONSE


class AllComputationNodes(Resource):
    @swagger.operation(
        notes='This endpoint is used for retrieving all computation nodes info',
        nickname='/nodes/computation_nodes',
        parameters=[],
        responseMessages=[
            ALL_DEVICES_GET_OK_RESPONSE
        ]
    )
    def get(self):
        result = database.get_list_of_nodes()
        log.info('Got all nodes fetched: {}'.format(result))
        return result, 200
