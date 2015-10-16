angular.module('hpcpm-ui').controller('NodeDetailsController', nodeDetailsController);
nodeDetailsController.$inject = ["$scope", "$rootScope", "DataService", '$stateParams', 'NgTableParams', '$filter', '$uibModal'];

function nodeDetailsController($scope, $rootScope, DataService, $stateParams, NgTableParams, $filter, $uibModal) {
    $scope.selectedNode = $stateParams.node_name;

    $scope.openRawDetailsModal = function(details) {
      var modalInstance = $uibModal.open({
          animation: true,
          size: 'lg',
          templateUrl: 'NodeDetailsModal.html',
          controller: 'NodeDetailsModalController',
          resolve: {
              nodeData: function () {
                  return details;
              }
          }
      });
    };

    $scope.tableParams = new NgTableParams({count: 10}, {
        counts: [10, 20, 40, 80, 100],
        getData: function ($defer, params) {
            DataService.getComputationNode($stateParams.node_name).then(function (response) {
                $scope.nodeData = response;
                $defer.resolve($scope.nodeData);

                $scope.simpleDetails = {
                  'Nickname': $scope.nodeData.name,
                  'Address': $scope.nodeData.address,
                  'Port': $scope.nodeData.port,
                  'Hostname': $scope.nodeData.backend_info.nodename,
                  'System name': $scope.nodeData.backend_info.sysname,
                  'Version': $scope.nodeData.backend_info.version,
                  'Machine architecture': $scope.nodeData.backend_info.machine,
                  'Release': $scope.nodeData.backend_info.release,
                }

            });
        }

    });
};
