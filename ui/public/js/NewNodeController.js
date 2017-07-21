angular.module('hpcpm-ui').controller('NewNodeController', newNodeController);
newNodeController.$inject = ['$scope', '$rootScope', '$modalInstance', 'toaster', 'DataService'];

function newNodeController($scope, $rootScope, $modalInstance, toaster, DataService) {
    $scope.cancel = function () {
        $modalInstance.dismiss('cancel');
    };

    $scope.addNode = function () {
        DataService.addComputationNode($scope.node).then(function (response) {
            $scope.response = response;
            toaster.pop('success', 'Success', 'Computation node ' + $scope.node.name + ' added successfully');
            $rootScope.$broadcast('RefreshNodes');
        },
        function (error) {
          toaster.pop('error', 'Error ' + error.status, 'Cannot add noew node: ' + $scope.node.name);
        });
        $modalInstance.close();
    };


}
