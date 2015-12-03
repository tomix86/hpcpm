describe('NodesController', function() {
  beforeEach(module('hpcpm-ui'));
  var $controller, $rootScope, $state, $uibModal, $filter, $timeout, NgTableParams, DataService;

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

