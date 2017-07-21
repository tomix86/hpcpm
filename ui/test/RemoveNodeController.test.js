describe('RemoveNodeController', function() {
  beforeEach(module('hpcpm-ui'));
  var $controller, controller, $rootScope, $scope, modalInstance, toaster, DataService, nodeName;

  beforeEach(function() {
    module(function($provide) {
      $provide.value('DataService', {
        removeComputationNode: null
      });
      return null;
    });
  });

  beforeEach(inject(function(_$controller_, _$rootScope_, _toaster_, _DataService_){
    $controller = _$controller_;
    $rootScope = _$rootScope_;
    toaster = _toaster_;
    DataService = _DataService_;
    $scope = $rootScope.$new();

    modalInstance = {
      close: jasmine.createSpy('modalInstance.close'),
      dismiss: jasmine.createSpy('modalInstance.dismiss'),
      result: {
        then: jasmine.createSpy('modalInstance.result.then')
      }
    };

    this.init = function(name) {
      controller = $controller('RemoveNodeController', { $scope: $scope, DataService: DataService, nodeName: name, $modalInstance: modalInstance });
    };

    spyOn(DataService, 'removeComputationNode').and.callFake(function(name) {
      return {
        then: function(callback, error) {
          if(name === undefined) {
            return error({status: 404});
          }
          else {
            return callback(name);
          }
        }
      };
    });

  }));

  it('should be canceled', function() {
    this.init(undefined);
    $scope.cancel();
    expect(modalInstance.dismiss).toHaveBeenCalled();
  });

  it('should try to remove node and handle error', function() {
    this.init(undefined);
    $scope.removeNode();
    expect(DataService.removeComputationNode).toHaveBeenCalledWith(undefined);
    expect(modalInstance.close).toHaveBeenCalled();
  });

  it('should remove node and close', function() {
    this.init('name');
    $scope.removeNode();
    expect(DataService.removeComputationNode).toHaveBeenCalledWith('name');
    expect(modalInstance.close).toHaveBeenCalled();
  });

});
