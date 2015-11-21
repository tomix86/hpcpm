angular.module('hpcpm-ui').controller('RemovePowerLimitModalController', removePowerLimitModalController);

removePowerLimitModalController.$inject = ['$scope', '$rootScope', '$modalInstance', 'toaster', 'DataService', 'nodeName', 'deviceId'];

function removePowerLimitModalController($scope, $rootScope, $modalInstance, toaster, DataService, nodeName, deviceId) {
    $scope.name = nodeName;
    $scope.deviceId = deviceId;

    $scope.cancel = function () {
        $modalInstance.dismiss('cancel');
    };

    $scope.removePowerLimit = function () {
        DataService.removeDevicePowerLimit($scope.name, $scope.deviceId).then(function (response) {
            $scope.response = response;
            toaster.pop('success', 'Success', 'Device' + $scope.deviceId + ' power on node ' + $scope.name + ' removed successfully');
            $rootScope.$broadcast('RefreshNodeDetails');
        },
        function (error) {
          toaster.pop('error', 'Error ' + error.status, 'Cannot remove power limit from device: ' + $scope.deviceId);
        });
        $modalInstance.close();
    };


}
