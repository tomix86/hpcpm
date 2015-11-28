var app = angular.module('hpcpm-ui', ['restangular', 'ui.router', 'ngTable', 'ui.bootstrap', 'toaster', 'jsonFormatter', 'chart.js', 'angularMoment']);


app.config([
  '$stateProvider',
  '$urlRouterProvider',
  'RestangularProvider',
  function($stateProvider, $urlRouterProvider) {
    $stateProvider
      .state('home', {
        url: '/',
        templateUrl: '/Nodes.html',
        controller: 'NodesController'
      })
      .state('node_details', {
        url: '/node/:node_name',
        templateUrl: '/NodeDetails.html',
        controller: 'NodeDetailsController',
        params: {
          node: null
        }
      })
      .state('device_stats_show', {
        url: '/node/:node_name/:device_id/statistics',
        templateUrl: '/DeviceStatsShow.html',
        controller: 'DeviceStatsShowController'
      });

    $urlRouterProvider.otherwise('/');


  }
]);

app.directive('onLastRepeat', function() {
    return function(scope, element, attrs) {
        if (scope.$last) setTimeout(function(){
            scope.$emit('onRepeatLast', element, attrs);
        }, 1);
    };
});

app.directive('validLimit', function() {
  return {
    require: 'ngModel',
    scope: { limits: '@validLimit' },
    link: function(scope, elm, attrs, ctrl) {
      ctrl.$validators.validLimit = function(modelValue, viewValue) {
        var limits = JSON.parse(scope.limits);
        if (ctrl.$isEmpty(modelValue)) {
          return true;
        }
        if (/^\-?\d+$/.test(viewValue) && viewValue > 0 && limits.lower <= viewValue && limits.upper >= viewValue) {
          return true;
        }
        return false;
      };
    }
  };
});


var BodyController = app.controller('BodyController', bodyController);
bodyController.$inject = ['$scope', '$state', '$uibModal', '$log'];

function bodyController($scope, $state, $uibModal, $log) {
  $scope.OpenNewNodeModal = function() {

    var modalInstance = $uibModal.open({
      animation: true,
      templateUrl: 'NewNode.html',
      controller: 'NewNodeController'
    });

    modalInstance.result.then(function(selectedItem) {
      $scope.selected = selectedItem;
    });
  };

}

var NodesController = app.controller('NodesController', nodesController);
nodesController.$inject = ['$scope', '$filter', 'NgTableParams', 'DataService', '$uibModal', '$timeout'];

function nodesController($scope, $filter, NgTableParams, DataService, $uibModal, $timeout) {
  $scope.tableParams = new NgTableParams({
    count: 10
  }, {
    counts: [10, 20, 40, 80, 100],
    getData: function($defer, params) {
      DataService.getAllComputationNodes(params.sorting(), params.filter()).then(function(response) {
        var data = response;
        data = params.sorting() ? data = $filter('orderBy')(data, params.orderBy()) : data;

        $defer.resolve(data.slice((params.page() - 1) * params.count(), params.page() * params.count()));

      },
      function (error) {
        toaster.pop('error', 'Error ' + error.status, 'Cannot get computation nodes from server!');
      });
    }

  });
  $scope.$on('RefreshNodes', function() {
    $scope.tableParams.reload();
  });
  $scope.OpenRemoveNodeModal = function(name) {
    var modalInstance = $uibModal.open({
      animation: true,
      templateUrl: 'RemoveNode.html',
      controller: 'RemoveNodeController',
      resolve: {
        nodeName: function() {
          return name;
        }
      }
    });

    modalInstance.result.then(function(selectedItem) {
      $scope.selected = selectedItem;
    });
  };

  $scope.intervalFunction = function() {
    $timeout(function() {
      $scope.tableParams.reload();
      $scope.intervalFunction();
    }, 5000);
  };

  $scope.intervalFunction();

}
