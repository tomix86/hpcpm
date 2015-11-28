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
        setDevicePowerLimitRule: function(name, device_id, _rule_params, _rule_type) {
            return Restangular.one(EndpointsService.putRuleUrl(), name).one(device_id, 'rule').customPUT(_rule_params, undefined, {rule_type: _rule_type}, undefined);
        },
        getDevicePowerUsage: function(name, device_id) {
            return Restangular.one(EndpointsService.getPowerUsageUrl(), name).one(device_id, 'power_usage').get();
        },
        getDevicePowerLimitConstraints: function(name, device_id) {
            return Restangular.one(EndpointsService.getPowerLimitConstraintsUrl(), name).one(device_id, 'power_limit_constraints').get();
        },
        getDevicePowerLimitRuleTypes: function(node) {
            return Restangular.one(EndpointsService.getRuleTypesUrl()).get();
        }
    };
}
