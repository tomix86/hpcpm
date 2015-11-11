angular.module('hpcpm-ui').factory('EndpointsService', function() {
    var _endpoints = {
        'status': 'status',
        'get_computation_node': 'nodes/computation_node',
        'put_computation_node': 'nodes/computation_node',
        'delete_computation_node': 'nodes/computation_node',
        'get_power_limit': 'nodes/computation_node',
        'put_power_limit': 'nodes/computation_node',
        'delete_power_limit': 'nodes/computation_node',
        'get_all_computation_nodes': 'nodes/computation_nodes',
        'get_statictics': 'nodes/computation_nodes'
    };
    var _baseUrl = 'http://vps.lel.lu:8080/api/hpcpm/';

    var o = {};

    o.getBaseUrl = function() {
        return _baseUrl;
    };

    o.getStatusUrl = function() {
        return _endpoints.status;
    };

    o.getComputationNodeUrl = function() {
        return _endpoints.get_computation_node;
    };

    o.getAllComputationNodesUrl = function() {
        return _endpoints.get_all_computation_nodes;
    };

    o.putComputationNodeUrl = function() {
        return _endpoints.put_computation_node;
    };

    o.deleteComputationNodeUrl = function() {
        return _endpoints.delete_computation_node;
    };

    o.getPowerLimitUrl = function() {
        return _endpoints.get_power_limit;
    };

    o.putPowerLimitUrl = function() {
        return _endpoints.put_power_limit;
    };

    o.getStatisticsUrl = function() {
        return _endpoints.get_statictics;
    };

    o.getEndpoints = function() {
        return Object.keys(_endpoints);
    };

    return o;
});
