describe('BodyController', function() {

  beforeEach(module('hpcpm-ui'));
  var $controller, $rootScope, $state, $uibModal, $log;

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
