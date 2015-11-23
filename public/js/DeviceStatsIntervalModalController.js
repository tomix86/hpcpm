angular.module('hpcpm-ui').controller('DeviceStatsIntervalModalController', deviceStatsIntervalModalController);
powerLimitModalController.$inject = ['$scope', '$rootScope', '$modalInstance', 'toaster', 'DataService', 'nodeName', 'deviceId'];

function deviceStatsIntervalModalController($scope, $rootScope, $modalInstance, toaster, DataService, nodeName, deviceId) {
    $scope.name = nodeName;
    $scope.deviceId = deviceId;

    DataService.getDeviceStatisticsInterval($scope.name, $scope.deviceId).then(function (response){
          $scope.currentInterval = response.plain().interval;
      },
      function (error) {
          $scope.currentInterval = 'none';
      }
    );

    $scope.removeCurrentInterval = function() {
        DataService.removeDeviceStatisticsInterval($scope.name, $scope.deviceId).then(function (response){
            toaster.pop('success', 'Success', 'Statistics interval on node: ' + $scope.name + ', device: ' + $scope.deviceId + ' removed successfully');
            $scope.currentInterval = 'none';

        },
        function (error) {
            toaster.pop('error', 'Error', 'Statistics interval on node: ' + $scope.name + ', device: ' + $scope.deviceId + ' could not be found');
        }
      );
    };

    $scope.setNewInterval = function() {
        if($scope.newInterval >= 1) {
        DataService.setDeviceStatisticsInterval($scope.name, $scope.deviceId, $scope.newInterval).then(function (response){
              toaster.pop('success', 'Success', 'Statistics interval on node: ' + $scope.name + ', device: ' + $scope.deviceId + ' set successfully');
              $scope.currentInterval = angular.copy($scope.newInterval);
          },
          function (error) {
              toaster.pop('error', 'Error', 'Node: ' + $scope.name + ' could not be found, unsuccessful statistics interval setting');
          }
        );
      }
      else {
          toaster.pop('error', 'Error', 'Invalid interval entered! It should be greater or equal to 1!');
      }
    };

    $scope.cancel = function () {
        $modalInstance.dismiss('cancel');
    };
}
