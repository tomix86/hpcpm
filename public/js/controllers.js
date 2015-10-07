var app = angular.module('frontend', ['restangular', 'ui.router']);

app.factory('RestService', function() {
  var _endpoints = {
    'status': 'status',
    'get_computation_node': 'nodes/computation_node',
    'put_computation_node': 'nodes/computation_node',
    'delete_computation_node': 'nodes/computation_node',
    'get_power_limit': 'nodes/computation_node',
    'put_power_limit': 'nodes/computation_node',
    'delete_power_limit': 'nodes/computation_node',
    'get_all_computation_nodes': 'nodes/computation_nodes'
  };
  var _baseUrl = 'http://localhost:8080/api/hpcpm/';

  var o = {};

  o.getBaseUrl = function() {
    return _baseUrl;
  };

  o.getStatusUrl = function() {
    return _endpoints.status;
  };

  o.getComputationNodeUrl = function() {
    return _endpoints.get_computation_node;
  };

  o.getAllComputationNodesUrl = function() {
    return _endpoints.get_all_computation_nodes;
  };

  o.putComputationNodeUrl = function() {
    return _endpoints.put_computation_node;
  };

  o.getEndpoints = function() {
    return Object.keys(_endpoints);
  };

  return o;
});

/*app.factory('ComputationNode', function() {
  return {
    name: '',
    address: '',
    port: ''
  }
});*/

app.config([
  '$stateProvider',
  '$urlRouterProvider',
  function($stateProvider, $urlRouterProvider) {
    $stateProvider
      .state('default', {
        url: '/',
        templateUrl: '/default.html',
        controller: 'FormController'
      })
      .state('status', {
        url: '/status',
        templateUrl: '/status.html',
        controller: 'StatusController'
      })
      .state('get_computation_node', {
        url: '/get_computation_node',
        templateUrl: '/get_computation_node.html',
        controller: 'GetComputationNodeController'
      })
      .state('put_computation_node', {
        url: '/put_computation_node',
        templateUrl: '/put_computation_node.html',
        controller: 'PutComputationNodeController'
      })
      .state('delete_computation_node', {
        url: '/delete_computation_node',
        templateUrl: '/delete_computation_node.html',
        controller: 'DeleteComputationNodeController'
      })
      .state('get_power_limit', {
        url: '/get_power_limit',
        templateUrl: '/get_power_limit.html',
        controller: 'GetDevicePowerLimitController'
      })
      .state('put_power_limit', {
        url: '/put_power_limit',
        templateUrl: '/put_power_limit.html',
        controller: 'PutDevicePowerLimitController'
      })
      .state('delete_power_limit', {
        url: '/delete_power_limit',
        templateUrl: '/delete_power_limit.html',
        controller: 'DeleteDevicePowerLimitController'
      })
      .state('get_all_computation_nodes', {
        url: '/get_all_computation_nodes',
        templateUrl: '/get_all_computation_nodes.html',
        controller: 'GetAllComputationNodesController'
      })
      .state('login', {
        url: '/login',
        templateUrl: '/login.html',
        controller: 'LoginController'
      });

    $urlRouterProvider.otherwise('default');
  }
]);

var formController = app.controller('BodyController', function($scope) {})
  .controller('FormController', function($scope, $state, Restangular, RestService) {
    $scope.selection = '';
    $scope.response = '';
    $scope.items = RestService.getEndpoints();

    Restangular.setBaseUrl(RestService.getBaseUrl());

    $scope.changeState = function() {
      $state.go($scope.selection);
    };
  });

formController.controller('StatusController', function($scope, Restangular, RestService) {
  $scope.$parent.selection = 'status';

  $scope.getStatus = function() {
    Restangular.one(RestService.getStatusUrl()).get().then(function(status) {
      $scope.response = status;
    });
  };
});

formController.controller('GetComputationNodeController', function($scope, Restangular, RestService) {
  $scope.$parent.selection = 'get_computation_node';

  $scope.getComputationNode = function() {
    Restangular.one(RestService.getComputationNodeUrl(), $scope.node.name).get().then(function(response) {
      $scope.response = response;
    });
  };
});

formController.controller('PutComputationNodeController', function($scope, Restangular, RestService) {
  $scope.$parent.selection = 'put_computation_node';

  $scope.putComputationNode = function() {
    Restangular.one(RestService.getComputationNodeUrl(), $scope.node.name)
      .put({
        address: $scope.node.address,
        port: $scope.node.port
      }).then(function(response) {
        $scope.response = response;
      });
  };
});

formController.controller('DeleteComputationNodeController', function($scope, Restangular, RestService) {
  $scope.$parent.selection = 'delete_computation_node';

  $scope.deleteComputationNode = function() {
    Restangular.one(RestService.getComputationNodeUrl(), $scope.node.name)
      .remove({
        name: $scope.node.name
      }).then(function(response) {
        $scope.response = response;
      });
  };
});

formController.controller('GetDevicePowerLimitController', function($scope, Restangular, RestService) {
  $scope.$parent.selection = 'get_power_limit';

  $scope.getPowerLimit = function() {
    Restangular.one(RestService.getComputationNodeUrl(), $scope.node.name + '/' + $scope.node.device_id + '/power_limit').get().then(function(response) {
      $scope.response = response;
    });
  };
});

formController.controller('PutDevicePowerLimitController', function($scope, Restangular, RestService) {
  $scope.$parent.selection = 'put_power_limit';

  $scope.putPowerLimit = function() {
    Restangular.one(RestService.getComputationNodeUrl(),  $scope.node.name + '/' + $scope.node.device_id + '/power_limit')
      .put({
        power_limit: $scope.node.power_limit
      }).then(function(response) {
        $scope.response = response;
      });
  };
});

formController.controller('DeleteDevicePowerLimitController', function($scope, Restangular, RestService) {
  $scope.$parent.selection = 'delete_power_limit';

  $scope.deletePowerLimit = function() {
    Restangular.one(RestService.getComputationNodeUrl(),  $scope.node.name + '/' + $scope.node.device_id + '/power_limit')
      .remove({
        name: $scope.node.name,
        device_id: $scope.node.device_id
      }).then(function(response) {
        $scope.response = response;
      });
  };
});

formController.controller('GetAllComputationNodesController', function($scope, Restangular, RestService) {
  $scope.$parent.selection = 'get_all_computation_nodes';

  $scope.getAllComputationNodes = function() {
    Restangular.all(RestService.getAllComputationNodesUrl()).getList().then(function(response) {
      $scope.response = response;
    });
  };
});
