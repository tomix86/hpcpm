describe('EndpointsService', function() {
  beforeEach(module('hpcpm-ui'));

  beforeEach(inject(function(_EndpointsService_){
    EndpointsService = _EndpointsService_;
  }));

  describe('endpoints URLs', function() {
    it('checks if the getStatusUrl result is correct', function() {
      expect(EndpointsService.getStatusUrl()).not.toBeUndefined();
      expect(EndpointsService.getStatusUrl()).toEqual('status');
    });

    it('checks if the getComputationNodeUrl result is correct', function() {
      expect(EndpointsService.getComputationNodeUrl()).not.toBeUndefined();
      expect(EndpointsService.getComputationNodeUrl()).toEqual('nodes/computation_node');
    });

    it('checks if the getAllComputationNodesUrl result is correct', function() {
      expect(EndpointsService.getAllComputationNodesUrl()).not.toBeUndefined();
      expect(EndpointsService.getAllComputationNodesUrl()).toEqual('nodes/computation_nodes');
    });

    it('checks if the putComputationNodeUrl result is correct', function() {
      expect(EndpointsService.putComputationNodeUrl()).not.toBeUndefined();
      expect(EndpointsService.putComputationNodeUrl()).toEqual('nodes/computation_node');
    });

    it('checks if the deleteComputationNodeUrl result is correct', function() {
      expect(EndpointsService.deleteComputationNodeUrl()).not.toBeUndefined();
      expect(EndpointsService.deleteComputationNodeUrl()).toEqual('nodes/computation_node');
    });

    it('checks if the getPowerLimitUrl result is correct', function() {
      expect(EndpointsService.getPowerLimitUrl()).not.toBeUndefined();
      expect(EndpointsService.getPowerLimitUrl()).toEqual('nodes/computation_node');
    });

    it('checks if the putPowerLimitUrl result is correct', function() {
      expect(EndpointsService.putPowerLimitUrl()).not.toBeUndefined();
      expect(EndpointsService.putPowerLimitUrl()).toEqual('nodes/computation_node');
    });

    it('checks if the getStatisticsUrl result is correct', function() {
      expect(EndpointsService.getStatisticsUrl()).not.toBeUndefined();
      expect(EndpointsService.getStatisticsUrl()).toEqual('nodes/computation_node');
    });

    it('checks if the getEndpoints result is correct', function() {
      expect(EndpointsService.getEndpoints().length).toBeGreaterThan(0);
      expect(EndpointsService.getEndpoints()).toEqual([
        'status',
        'get_computation_node',
        'put_computation_node',
        'delete_computation_node',
        'get_power_limit',
        'put_power_limit',
        'delete_power_limit',
        'get_all_computation_nodes',
        'get_statistics',
        'get_interval',
        'put_interval',
        'delete_interval',
        'get_rule',
        'set_rule',
        'get_rule_types'
      ]);
    });
  });
});

describe('BodyController', function() {
  beforeEach(module('hpcpm-ui'));

  var $controller, $rootScope;

  beforeEach(inject(function(_$controller_, _$rootScope_, _$state_, _$uibModal_, _$log_){
    // The injector unwraps the underscores (_) from around the parameter names when matching
    $controller = _$controller_;
    $rootScope = _$rootScope_;
    $state = _$state_;
    $uibModal = _$uibModal_;
    $log = _$log_;
  }));

  describe('$scope.OpenNewNodeModal', function() {
    var $scope, controller;

    beforeEach(function() {
      $scope = $rootScope.$new();
      controller = $controller('BodyController', { $scope: $scope });
    });

    it('checks if the OpenNewNodeModal is not null', function() {
      expect($scope.OpenNewNodeModal).not.toBeNull();
    });
  });
});

describe('NodesController', function() {
  beforeEach(module('hpcpm-ui'));

  var $controller, $rootScope;

  beforeEach(inject(function(_$controller_, _$rootScope_, _$state_, _$uibModal_, _$filter_, _$timeout_, _NgTableParams_, _DataService_){
    // The injector unwraps the underscores (_) from around the parameter names when matching
    $controller = _$controller_;
    $rootScope = _$rootScope_;
    $state = _$state_;
    $uibModal = _$uibModal_;
    $filter = _$filter_;
    $timeout = _$timeout_;
    NgTableParams = _NgTableParams_;
    DataService = _DataService_;
  }));

  describe('tableParams', function() {
    var $scope, controller;

    beforeEach(function() {
      $scope = $rootScope.$new();
      controller = $controller('NodesController', { $scope: $scope, NgTableParams: NgTableParams });
    });

    it('checks if the tableParams is not undefined', function() {
      expect($scope.tableParams).not.toBeUndefined();
    });
  });
});
