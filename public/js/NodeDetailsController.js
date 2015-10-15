angular.module('hpcpm-ui').controller('NodeDetailsController', nodeDetailsController);

nodeDetailsController.$inject = ["$scope", "$rootScope", "DataService", '$stateParams', 'NgTableParams', '$filter'];

function nodeDetailsController($scope, $rootScope, DataService, $stateParams, NgTableParams, $filter) {
    $scope.selectedNode = $stateParams.node_name;
    $scope.tableParams = new NgTableParams({count: 10}, {
        counts: [10, 20, 40, 80, 100],
        getData: function ($defer, params) {
            DataService.getComputationNode($stateParams.node_name).then(function (response) {
                var data = response;
                //data = params.sorting() ? data = $filter('orderBy')(data, params.orderBy()) : data;

                //$defer.resolve(data.slice((params.page() - 1) * params.count(), params.page() * params.count()));

            });
        }

    });
}
