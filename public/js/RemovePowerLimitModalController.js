angular.module('hpcpm-ui').controller('RemovePowerLimitModalController', removePowerLimitModalController);

removePowerLimitModalController.$inject = ['$scope', '$rootScope', '$modalInstance', 'toaster', 'DataService', 'nodeName', 'deviceId'];

function removePowerLimitModalController($scope, $rootScope, $modalInstance, toaster, DataService, nodeName, deviceId) {
    $scope.name = nodeName;
    $scope.deviceId = deviceId;

    $scope.cancel = function () {
        $modalInstance.dismiss('cancel');
    };

    $scope.getPowerLimitRule = function() {
      $scope.error = '';

      DataService.getDevicePowerLimitRuleTypes().then(function(response) {
            $scope.supportedRules = response.plain();
        },
        function(error) {
            $scope.supportedRules = ['TimeBased']; //fallback
        }
      );

      DataService.getDevicePowerLimitRule($scope.name, $scope.deviceId).then(function (response) {
        $scope.ruleData = response.plain();
        $scope.ruleFormatted = [];
        if($.inArray($scope.ruleData.rule_type, $scope.supportedRules) != -1) {
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
        function(error) {
          $scope.error = 'No current power limit rule';
        }
      );
    };
    $scope.getPowerLimitRule();

    $scope.removePowerLimitRule = function () {
        DataService.removeDevicePowerLimitRule($scope.name, $scope.deviceId).then(function (response) {
            $scope.response = response;
            toaster.pop('success', 'Success', 'Device' + $scope.deviceId + ' power limit rule on node ' + $scope.name + ' removed successfully');
            $rootScope.$broadcast('RefreshNodeDetails');
        },
        function (error) {
          toaster.pop('error', 'Error ' + error.status, 'Cannot remove power limit rule from device: ' + $scope.deviceId);
        });
        $modalInstance.close();
    };


}
