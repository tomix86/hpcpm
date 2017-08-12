angular.module('hpcpm-ui').controller('DeviceStatsShowController', deviceStatsShowController);
deviceStatsShowController.$inject = ['$scope', '$rootScope', 'DataService', '$stateParams', '$filter', '$timeout', 'amMoment', 'toaster'];

function deviceStatsShowController($scope, $rootScope, DataService, $stateParams, $filter, $timeout, amMoment, toaster) {
  $scope.chartOptions = {
    showTooltips: true,
    pointHitDetectionRadius: 1
  };
  $('#statsStartTime').datetimepicker({
    sideBySide: true,
    locale: 'en-GB',
    defaultDate: moment().utc().subtract(1, 'hours')
  });
  $('#statsEndTime').datetimepicker({
    sideBySide: true,
    locale: 'en-GB',
    defaultDate: moment().utc()
  });
  $scope.node_name = $stateParams.node_name;
  $scope.device_id = $stateParams.device_id;
  $scope.date = {};
  $scope.labels = [];
  $scope.data = [
    []
  ];

  $scope.refreshTime = function() {
    $('#statsStartTime').data("DateTimePicker").date(moment().utc().subtract(1, 'hours'));
    $('#statsEndTime').data("DateTimePicker").date(moment().utc());
  };

  $scope.getStats = function() {
    $scope.date.start = $('#statsStartTime').data('DateTimePicker').date();
    $scope.date.end = $('#statsEndTime').data('DateTimePicker').date();
    if (+$scope.date.start.toDate() >= +$scope.date.end.toDate()) {
      toaster.pop('error', 'Error ', 'Selected incorrect time interval!');
      $('#bar').css('display', 'none');
      $timeout(function() {
        $scope.labels = [];
        $scope.data = [
          []
        ];
      });
      $scope.error = 'Selected incorrect time interval!';
      return;
    }
    $scope.data = [
      []
    ];
    $scope.labels = [];
    $scope.series = ['power usage in watts'];
    DataService.getDeviceStatistics($scope.node_name, $scope.device_id,
        $scope.date.start.format('YYYY-MM-DDTHH:mm'),
        $scope.date.end.format('YYYY-MM-DDTHH:mm'))
      .then(function(response) {
        if (response != 404) {
          var values = [
            []
          ];
          var labels = [];
          $scope.error = '';
          var data = response.plain();
          for (var i = 0; i < data.length; ++i) {
            for (var key in data[i]) {
              labels.push(key);
              values[0].push(data[i][key]);
            }
          }
          $timeout(function() {
            $scope.labels = labels;
            $scope.data = values;
          });
          $('#bar').css('display', 'inline');
        } else {
          toaster.pop('error', 'Error ', 'No statistics data for given time span!');
          $('#bar').css('display', 'none');
          $timeout(function() {
            $scope.labels = [];
            $scope.data = [
              []
            ];
          });
          $scope.error = 'No statistics data for given time span!';
        }
      });
  };
}
