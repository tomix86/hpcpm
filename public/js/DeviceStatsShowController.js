angular.module('hpcpm-ui').controller('DeviceStatsShowController', deviceStatsShowController);
deviceStatsShowController.$inject = ['$scope', '$rootScope', 'DataService', '$stateParams', '$filter', 'amMoment'];

function deviceStatsShowController($scope, $rootScope, DataService, $stateParams, $filter, amMoment) {
  $scope.node_name = $stateParams.node_name;
  $scope.device_id = $stateParams.device_id;
  $scope.date = {};
  $scope.labels = [];//[now.format('YYYY-MM-DD HH:mm'), now.add(5, 'm').format('YYYY-MM-DD HH:mm'), now.add(10, 'm').format('YYYY-MM-DD HH:mm'), now.add(15, 'm').format('YYYY-MM-DD HH:mm'), now.add(20, 'm').format('YYYY-MM-DD HH:mm'), now.add(25, 'm').format('YYYY-MM-DD HH:mm'), now.add(30, 'm').format('YYYY-MM-DD HH:mm'), now.add(35, 'm').format('YYYY-MM-DD HH:mm'), now.add(40, 'm').format('YYYY-MM-DD HH:mm'), now.add(45, 'm').format('YYYY-MM-DD HH:mm'), now.add(50, 'm').format('YYYY-MM-DD HH:mm')];
  $scope.series = ['power limit [W]'];
  $scope.data = [
    []
    //[65, 59, 80, 81, 56, 55, 40,65, 59, 80, 11]
  ];
  $scope.onDateSet = function (newDate, oldDate) {
    $scope.date.start = moment(newDate);
    $scope.date.end = moment(oldDate);
  };
  $scope.getStats = function() {
    DataService.getDeviceStatistics($scope.node_name, $scope.device_id, $scope.date.start.format('YYYY-MM-DDTHH:mm'), $scope.date.end.format('YYYY-MM-DDTHH:mm')).then(function(response) {
      var data = response.plain();
      for (var i = 0; i < data.length; ++i) {
        for(var key in data[i]) {
          $scope.labels.push(key);
          $scope.data[0].push(data[i][key]);
        }
      }
    });
  };
}
