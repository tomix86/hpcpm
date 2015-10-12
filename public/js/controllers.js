var app = angular.module('hpcpm-ui', ['restangular', 'ui.router', "ngTable", "ui.bootstrap", "toaster"]);


app.config([
    '$stateProvider',
    '$urlRouterProvider',
    function ($stateProvider, $urlRouterProvider) {
        $stateProvider
            .state('default', {
                url: '/',
                templateUrl: '/default.html',
                controller: 'FormController'
            })
            .state('status', {
                url: '/status',
                templateUrl: '/status.html',
                controller: 'StatusController'
            })
            .state('get_computation_node', {
                url: '/get_computation_node',
                templateUrl: '/get_computation_node.html',
                controller: 'GetComputationNodeController'
            })
            .state('put_computation_node', {
                url: '/put_computation_node',
                templateUrl: '/put_computation_node.html',
                controller: 'PutComputationNodeController'
            })
            .state('delete_computation_node', {
                url: '/delete_computation_node',
                templateUrl: '/delete_computation_node.html',
                controller: 'DeleteComputationNodeController'
            })
            .state('get_power_limit', {
                url: '/get_power_limit',
                templateUrl: '/get_power_limit.html',
                controller: 'GetDevicePowerLimitController'
            })
            .state('put_power_limit', {
                url: '/put_power_limit',
                templateUrl: '/put_power_limit.html',
                controller: 'PutDevicePowerLimitController'
            })
            .state('delete_power_limit', {
                url: '/delete_power_limit',
                templateUrl: '/delete_power_limit.html',
                controller: 'DeleteDevicePowerLimitController'
            })
            .state('get_all_computation_nodes', {
                url: '/get_all_computation_nodes',
                templateUrl: '/get_all_computation_nodes.html',
                controller: 'GetAllComputationNodesController'
            })
            .state('login', {
                url: '/login',
                templateUrl: '/login.html',
                controller: 'LoginController'
            });

        $urlRouterProvider.otherwise('default');
    }
]);

var bodyController = app.controller('BodyController', function ($scope, $uibModal, $log) {
    $scope.OpenNewNodeModal = function () {

        var modalInstance = $uibModal.open({
            animation: true,
            templateUrl: 'AddNode.html',
            controller: 'NewNodeController'
        });

        modalInstance.result.then(function (selectedItem) {
            $scope.selected = selectedItem;
        });
    };
});


app.controller("nodesController", nodesController);
nodesController.$inject = ["$scope", "$filter", "NgTableParams", "DataService", "$uibModal", "$timeout"];
function nodesController($scope, $filter, NgTableParams, DataService, $uibModal, $timeout) {
    $scope.tableParams = new NgTableParams({count: 10}, {
        counts: [10, 20, 40, 80, 100],
        getData: function ($defer, params) {
            DataService.getAllComputationNodes(params.sorting(), params.filter()).then(function (response) {
                var data = response;

                data = params.sorting() ? data = $filter('orderBy')(data, params.orderBy()) : data;

                $defer.resolve(data.slice((params.page() - 1) * params.count(), params.page() * params.count()));

            });
        }

    });
    $scope.$on('RefreshNodes', function () {
        $scope.tableParams.reload();
    });
    $scope.OpenRemoveNodeModal = function (name) {
        var modalInstance = $uibModal.open({
            animation: true,
            templateUrl: 'RemoveNode.html',
            controller: 'RemoveNodeController',
            resolve: {
                nodeName: function () {
                    return name;
                }
            }
        });

        modalInstance.result.then(function (selectedItem) {
            $scope.selected = selectedItem;
        });
    };

    // Function to replicate setInterval using $timeout service.
    $scope.intervalFunction = function () {
        $timeout(function () {
            $scope.tableParams.reload();
            $scope.intervalFunction();
        }, 5000)
    };

    // Kick off the interval
    $scope.intervalFunction();

}