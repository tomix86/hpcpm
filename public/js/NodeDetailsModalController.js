angular.module('hpcpm-ui').controller('NodeDetailsModalController', nodeDetailsModalController);
nodeDetailsModalController.$inject = ["$scope", "$rootScope", "$modalInstance", "DataService", "nodeData"];

function nodeDetailsModalController($scope, $rootScope, $modalInstance, DataService, nodeData) {
    $scope.nodeData = nodeData;

    $scope.close = function () {
        $modalInstance.dismiss('cancel');
    };

}
