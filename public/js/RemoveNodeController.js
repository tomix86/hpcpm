angular.module('hpcpm-ui').controller('RemoveNodeController', removeNodeController);

removeNodeController.$inject = ["$scope", "$rootScope", "$modalInstance", "toaster", "DataService", "nodeName"];

function removeNodeController($scope, $rootScope, $modalInstance, toaster, DataService, nodeName) {
    $scope.name = nodeName;

    $scope.cancel = function () {
        $modalInstance.dismiss('cancel');
    };

    $scope.removeNode = function () {
        DataService.removeComputationNode(nodeName).then(function (response) {
            $scope.response = response;
            toaster.pop('success', "Success", "Computation node " + nodeName + " removed successfully");
            $rootScope.$broadcast('RefreshNodes');
        });
        $modalInstance.close();
    };


}
