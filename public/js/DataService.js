angular.module('hpcpm-ui')
    .factory('DataService', ['Restangular', 'EndpointsService', dataService]);

function dataService(Restangular, EndpointsService) {
    Restangular.setBaseUrl(EndpointsService.getBaseUrl());
    return {
        getAllComputationNodes: function(sorting, filters) {
            return Restangular.all(EndpointsService.getAllComputationNodesUrl()).getList();
        },
        addComputationNode: function(node) {
            return Restangular.one(EndpointsService.putComputationNodeUrl(), node.name).put(node);
        },
        removeComputationNode: function(name) {
            return Restangular.one(EndpointsService.deleteComputationNodeUrl(), name).remove();
        },
        getComputationNode: function(name) {
            return Restangular.one(EndpointsService.getComputationNodeUrl(), name).get();
        },
        getDevicePowerLimit: function(name, device_id) {
            return Restangular.one(EndpointsService.getPowerLimitUrl(), name).one(device_id, 'power_limit').get();
        },
        setDevicePowerLimit: function(name, device_id, power_limit) {
            return Restangular.one(EndpointsService.putPowerLimitUrl(), name).one(device_id, 'power_limit').put(power_limit);
        },
        removeDevicePowerLimit: function(name, device_id) {
            return Restangular.one(EndpointsService.deletePowerLimitUrl(), name).one(device_id, 'power_limit').remove();
        },
        getDeviceStatistics: function(name, device_id, begin_date, end_date) {
            return Restangular.one(EndpointsService.getPowerLimitUrl(), name).one(device_id, 'statistics_data').get({date_time_begin: begin_date, date_time_end: end_date});
        },
        getDeviceStatisticsInterval: function(name, device_id) {
            return Restangular.one(EndpointsService.getStatisticsIntervalUrl(), name).one(device_id, 'statistics_interval').get();
        },
        removeDeviceStatisticsInterval: function(name, device_id) {
            return Restangular.one(EndpointsService.deleteStatisticsIntervalUrl(), name).one(device_id, 'statistics_interval').remove();
        },
        setDeviceStatisticsInterval: function(name, device_id, interval) {
            return Restangular.one(EndpointsService.putStatisticsIntervalUrl(), name).one(device_id, 'statistics_interval').put({statistics_interval: interval});
        },
        getDevicePowerLimitRule: function(name, device_id) {
            return Restangular.one(EndpointsService.getRuleUrl(), name).one(device_id, 'rule').get();
        },
        removeDevicePowerLimitRule: function(name, device_id) {
            return Restangular.one(EndpointsService.deleteRuleUrl(), name).one(device_id, 'rule').remove();
        },
        setDevicePowerLimitRule: function(name, device_id, rule_params, rule_type) {
            return Restangular.one(EndpointsService.deleteRuleUrl(), name).one(device_id, 'rule').customPUT(rule_params, {rule_type: rule_type});
        },
        getDevicePowerLimitRuleTypes: function(node) {
            return Restangular.one(EndpointsService.getRuleTypesUrl()).get();
        }
    };
}
