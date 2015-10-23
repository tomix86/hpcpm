angular.module('hpcpm-ui')
    .factory('DataService', ['Restangular', 'EndpointsService', dataService]);

function dataService(Restangular, EndpointsService) {
    Restangular.setBaseUrl(EndpointsService.getBaseUrl());
    return {
        getAllComputationNodes: function(sorting, filters) {
            return Restangular.all(EndpointsService.getAllComputationNodesUrl()).getList();
        },
        addComputationNode: function(node) {
            return Restangular.one(EndpointsService.putComputationNodeUrl(), node.name).put(node);
        },
        removeComputationNode: function(name) {
            return Restangular.one(EndpointsService.deleteComputationNodeUrl(), name).remove();
        },
        getComputationNode: function(name) {
            return Restangular.one(EndpointsService.getComputationNodeUrl(), name).get();
        },
        getDevicePowerLimit: function(name, device_id) {
            return Restangular.one(EndpointsService.getPowerLimitUrl(), name).one(device_id, 'power_limit').get();
        },
        setDevicePowerLimit: function(name, device_id, power_limit) {
            return Restangular.one(EndpointsService.putPowerLimitUrl(), name).one(device_id, 'power_limit').put(power_limit);
        },
        removeDevicePowerLimit: function(name, device_id) {
            return Restangular.one(EndpointsService.putPowerLimitUrl(), name).one(device_id, 'power_limit').remove();
        }
    }
}
