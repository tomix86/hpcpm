angular.module('hpcpm-ui').controller('DeviceStatsShowController', deviceStatsShowController);
deviceStatsShowController.$inject = ['$scope', '$rootScope', 'DataService', '$stateParams', '$filter', 'amMoment'];

function deviceStatsShowController($scope, $rootScope, DataService, $stateParams, $filter, amMoment) {
  $('#statsStartTime').combodate({
      minYear: 2008,
      minuteStep: 1,
      value: moment().subtract(1, 'days').format('DD-MM-YYYY HH:mm')
  });
  $('#statsEndTime').combodate({
      minYear: 2008,
      minuteStep: 1,
      value: moment().format('DD-MM-YYYY HH:mm')
  });
  $scope.node_name = $stateParams.node_name;
  $scope.device_id = $stateParams.device_id;
  $scope.date = {};
  $scope.labels = [];
  $scope.series = ['power limit [W]'];
  $scope.data = [[]];

  $scope.getStats = function() {
    $scope.date.start = $('#statsStartTime').combodate('getValue', null);
    $scope.date.end = $('#statsEndTime').combodate('getValue', null);
    $scope.data = [[]];
    $scope.labels = [];
    DataService.getDeviceStatistics($scope.node_name, $scope.device_id, $scope.date.start.format('YYYY-MM-DDTHH:mm'), $scope.date.end.format('YYYY-MM-DDTHH:mm')).then(function(response) {
      if(response != 404){
        var data = response.plain();
        for (var i = 0; i < data.length; ++i) {
          for(var key in data[i]) {
            $scope.labels.push(key);
            $scope.data[0].push(data[i][key]);
          }
        }
        $('#bar').css('display', 'inline');
      }
      else {
        $scope.error = 'No statistics data for given time span!';
      }
    });
  };
}
