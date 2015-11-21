angular.module('hpcpm-ui').controller('PowerLimitModalController', powerLimitModalController);
powerLimitModalController.$inject = ['$scope', '$rootScope', '$modalInstance', 'toaster', 'DataService', 'nodeName', 'deviceId'];

function powerLimitModalController($scope, $rootScope, $modalInstance, toaster, DataService, nodeName, deviceId) {
    $scope.name = nodeName;
    $scope.deviceId = deviceId;

    $scope.cancel = function () {
        $modalInstance.dismiss('cancel');
    };

    $scope.setPowerLimit = function () {
        DataService.setDevicePowerLimit($scope.name, $scope.deviceId, {'power_limit': $scope.powerLimit}).then(function (response) {
            $scope.response = response;
            toaster.pop('success', 'Success', 'Power limit on node: ' + $scope.name + ', device: ' + $scope.deviceId + ' set successfully');
            $rootScope.$broadcast('RefreshNodeDetails');
        },
        function (error) {
          toaster.pop('error', 'Error ' + error.status, 'Cannot set power limit for device: ' + $scope.deviceId);
        });
        $modalInstance.close();
    };


}
