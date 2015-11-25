angular.module('hpcpm-ui').controller('PowerLimitModalController', powerLimitModalController);
powerLimitModalController.$inject = ['$scope', '$rootScope', '$modalInstance', 'toaster', 'DataService', 'nodeName', 'deviceId'];

function powerLimitModalController($scope, $rootScope, $modalInstance, toaster, DataService, nodeName, deviceId) {
    $scope.name = nodeName;
    $scope.deviceId = deviceId;
    $scope.error = '';

    var supportedRules = ['TimeBased'];

    $scope.cancel = function () {
        $modalInstance.dismiss('cancel');
    };

    DataService.getDevicePowerLimitRule($scope.name, $scope.deviceId).then(function (response) {
        $scope.ruleData = response.plain();
        $scope.ruleFormatted = [];
        if($.inArray($scope.ruleData.rule_type, supportedRules) != -1) {
            for(var i = 0; i < $scope.ruleData.rule_params.length; i++) {
                $scope.ruleFormatted.push({});
                $scope.ruleFormatted[i].start = $scope.ruleData.rule_params[i].start;
                $scope.ruleFormatted[i].end = $scope.ruleData.rule_params[i].end;
                $scope.ruleFormatted[i].limit = $scope.ruleData.rule_params[i].limit;
            }
        }
        else {
            $scope.error = 'Unsupported rule type!';
            toaster.pop('error', 'Error', 'Unsupported power limit rule: ' + $scope.ruleData.rule_type);
        }
      },
      function (error) {
          $scope.error = 'No current power limit rule for this device!';
      }
    );

    $scope.removeCurrentRule = function() {
        DataService.removeDevicePowerLimitRule($scope.name, $scope.deviceId).then(function (response) {
            $scope.error = 'No current power limit rule for this device!';
            toaster.pop('success', 'Success', 'Removed power limit rule from node: ' + $scope.name + ', device: ' + $scope.deviceId);
        },
        function (error) {
            toaster.pop('error', 'Error ' + error.status, 'Cannot remove power limit rule from device: ' + $scope.deviceId);
        }
      );
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
