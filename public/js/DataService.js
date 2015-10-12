angular.module('hpcpm-ui')
    .factory('DataService', ['Restangular', 'EndpointsService', dataService]);

function dataService(Restangular, EndpointsService) {
    Restangular.setBaseUrl(EndpointsService.getBaseUrl());
    return {
        getAllComputationNodes: function(sorting, filters) {
            return Restangular.all(EndpointsService.getAllComputationNodesUrl()).getList();
        },
        addComputationNode: function(node) {
            return Restangular.one(EndpointsService.getComputationNodeUrl(), node.name).put(node);
        },
        removeComputationNode: function(name) {
            return Restangular.one(EndpointsService.deleteComputationNodeUrl(), name).remove();
        }
    }
}