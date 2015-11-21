angular.module('hpcpm-ui').controller('NodeDetailsController', nodeDetailsController);
nodeDetailsController.$inject = ['$scope', '$rootScope', 'DataService', '$stateParams', 'NgTableParams', '$filter', '$uibModal', 'toaster'];

function nodeDetailsController($scope, $rootScope, DataService, $stateParams, NgTableParams, $filter, $uibModal, toaster) {
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

    $scope.openSetPowerLimitModal = function(name, id) {
      var modalInstance = $uibModal.open({
          animation: true,
          templateUrl: 'PowerLimitModal.html',
          controller: 'PowerLimitModalController',
          resolve: {
              nodeName: function () {
                  return name;
              },
              deviceId: function () {
                  return id;
              }
          }
      });
    };

    $scope.openRemovePowerLimitModal = function(name, id) {
      var modalInstance = $uibModal.open({
          animation: true,
          templateUrl: 'RemovePowerLimitModal.html',
          controller: 'RemovePowerLimitModalController',
          resolve: {
              nodeName: function () {
                  return name;
              },
              deviceId: function () {
                  return id;
              }
          }
      });
    };

    $scope.tableParams = new NgTableParams({count: 10}, {
        counts: [10, 20, 40, 80, 100],
        getData: function ($defer, params) {
            DataService.getComputationNode($stateParams.node_name).then(function (response) {
                $scope.nodeData = response;
                $scope.rawNodeData = angular.copy(response);
                $scope.getPowerLimitsForDevices();
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
                };

            },
            function (error) {
              toaster.pop('error', 'Error ' + error.status, 'Cannot get data for node: ' + $stateParams.node_name);
            });
        }

    });

    $scope.getPowerLimitsForDevices = function() {
        $scope.nodeData.backend_info.devices.forEach(function(device) {
          DataService.getDevicePowerLimit($scope.nodeData.name, device.id).then(function (response) {
            device.power_limit = response.power_limit;
          },
          function (error) {
            toaster.pop('warning', '', 'No current power limit available for device: ' + device.id);
          });
        });

    };

    $scope.$on('RefreshNodeDetails', function () {
        $scope.tableParams.reload();
    });
}
