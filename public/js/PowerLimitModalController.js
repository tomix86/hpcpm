angular.module('hpcpm-ui').controller('PowerLimitModalController', powerLimitModalController);
powerLimitModalController.$inject = ['$scope', '$rootScope', '$modalInstance', 'toaster', 'DataService', 'nodeName', 'deviceId', 'amMoment'];

function powerLimitModalController($scope, $rootScope, $modalInstance, toaster, DataService, nodeName, deviceId, amMoment) {
    $scope.name = nodeName;
    $scope.deviceId = deviceId;
    $scope.error = '';
    $scope.subrules = {};
    $scope.supportedRules = ['TimeBased'];

    $scope.cancel = function () {
        $modalInstance.dismiss('cancel');
    };

    $scope.getPowerLimitRule = function() {
      $scope.error = '';
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

    $scope.setPowerLimit = function() {
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

    $scope.setPowerLimitRule = function() {
        var params = [];
        for(var key in $scope.subrules) {
            var param = {};
            param.start = $('#startTime'+key).combodate('getValue', null);
            param.end = $('#endTime'+key).combodate('getValue', null);
            if(+param.start.toDate() >= +param.end.toDate()) {
              toaster.pop('error', 'Error ', 'Selected incorrect time time range!');
              return;
            }
            param.start = param.start.format('YYYY-MM-DDTHH:mm');
            param.end = param.end.format('YYYY-MM-DDTHH:mm');
            param.limit = $scope.subrules[key].powerLimit;
            params.push(param);
        }
        params = JSON.stringify(params);
        DataService.setDevicePowerLimitRule($scope.name, $scope.deviceId, params, $scope.selectedRule).then(function(response) {
            $scope.response = response;
            toaster.pop('success', 'Success', 'Power limit rule on node: ' + $scope.name + ', device: ' + $scope.deviceId + ' set successfully');
            $scope.getPowerLimitRule();
            $rootScope.$broadcast('RefreshNodeDetails');
          },
          function(error) {
            toaster.pop('error', 'Error ' + error.status, 'Cannot set power limit rule for device: ' + $scope.deviceId);
          }
      );
    };

    $scope.addSubrule = function() {
        var key = $scope.newSubRule();
        $scope.$on('onRepeatLast', function(scope, element, attrs){
          $scope.activateDatePickers(key);
        });
    };

    $scope.newSubRule = function() {
        var _index = Object.keys($scope.subrules).length;

        $scope.subrules[_index] = {
          index: _index,
          powerLimit: ''
        };

        return _index;
    };

    $scope.activateDatePickers = function(key) {
      var time = moment().utc().format('DD-MM-YYYY HH:mm');
      $('#startTime'+key).combodate({
          minYear: 2008,
          minuteStep: 1,
          value: angular.copy(time)
      });
      $('#endTime'+key).combodate({
          minYear: 2008,
          minuteStep: 1,
          value: angular.copy(time)
      });
    };

    $scope.removeSubrule = function(index) {
        delete $scope.subrules[index];
    };

}
