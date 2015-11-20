from flask import Blueprint
from flask_restful import Api
from flask_restful_swagger import swagger
from hpcpm.api.resources.endpoints.Status import Status
from hpcpm.api.resources.endpoints.nodes.computation_node.ComputationNode import ComputationNode
from hpcpm.api.resources.endpoints.nodes.computation_node.PowerLimit import PowerLimit
from hpcpm.api.resources.endpoints.nodes.computation_node.AllComputationNodes import AllComputationNodes
from hpcpm.api.resources.endpoints.nodes.computation_node.StatisticsInterval import StatisticsInterval
from hpcpm.api.resources.endpoints.nodes.computation_node.StatisticsData import StatisticsData
from hpcpm.api.resources.endpoints.nodes.computation_node.StatisticsDataWithInterval import StatisticsDataWithInterval
from hpcpm.api.resources.endpoints.RuleTypes import RuleTypes
from hpcpm.api.resources.endpoints.nodes.computation_node.Rule import Rule


class ApiSpec:  # pylint: disable=too-few-public-methods
    def __init__(self, config):
        self.blueprint = Blueprint('hpcpm-api', __name__)
        api = swagger.docs(Api(self.blueprint), apiVersion='0.1', basePath=config['base_url'], api_spec_url='/help')

        api.add_resource(Status, '/status')
        api.add_resource(ComputationNode, '/nodes/computation_node/<string:name>')
        api.add_resource(PowerLimit, '/nodes/computation_node/<string:name>/<string:device_id>/power_limit')
        api.add_resource(AllComputationNodes, '/nodes/computation_nodes')
        api.add_resource(StatisticsInterval,
                         '/nodes/computation_node/<string:name>/<string:device_id>/statistics_interval')
        api.add_resource(StatisticsData,
                         '/nodes/computation_node/<string:name>/<string:device_id>/statistics_data/<string:date_time>')
        api.add_resource(StatisticsDataWithInterval,
                         '/nodes/computation_node/<string:name>/<string:device_id>/statistics_data')
        api.add_resource(RuleTypes, '/rule_types')
        api.add_resource(Rule, '/nodes/computation_node/<string:name>/<string:device_id>/rule')
