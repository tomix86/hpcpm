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

    it('checks if the deletePowerLimitUrl result is correct', function() {
      expect(EndpointsService.deletePowerLimitUrl()).not.toBeUndefined();
      expect(EndpointsService.deletePowerLimitUrl()).toEqual('nodes/computation_node');
    });

    it('checks if the getStatisticsUrl result is correct', function() {
      expect(EndpointsService.getStatisticsUrl()).not.toBeUndefined();
      expect(EndpointsService.getStatisticsUrl()).toEqual('nodes/computation_node');
    });

    it('checks if the getStatisticsIntervalUrl result is correct', function() {
      expect(EndpointsService.getStatisticsIntervalUrl()).not.toBeUndefined();
      expect(EndpointsService.getStatisticsIntervalUrl()).toEqual('nodes/computation_node');
    });

    it('checks if the putStatisticsIntervalUrl result is correct', function() {
      expect(EndpointsService.putStatisticsIntervalUrl()).not.toBeUndefined();
      expect(EndpointsService.putStatisticsIntervalUrl()).toEqual('nodes/computation_node');
    });

    it('checks if the deleteStatisticsIntervalUrl result is correct', function() {
      expect(EndpointsService.deleteStatisticsIntervalUrl()).not.toBeUndefined();
      expect(EndpointsService.deleteStatisticsIntervalUrl()).toEqual('nodes/computation_node');
    });

    it('checks if the getRuleUrl result is correct', function() {
      expect(EndpointsService.getRuleUrl()).not.toBeUndefined();
      expect(EndpointsService.getRuleUrl()).toEqual('nodes/computation_node');
    });

    it('checks if the putRuleUrl result is correct', function() {
      expect(EndpointsService.putRuleUrl()).not.toBeUndefined();
      expect(EndpointsService.putRuleUrl()).toEqual('nodes/computation_node');
    });

   it('checks if the deleteRuleUrl result is correct', function() {
      expect(EndpointsService.deleteRuleUrl()).not.toBeUndefined();
      expect(EndpointsService.deleteRuleUrl()).toEqual('nodes/computation_node');
    });

    it('checks if the getPowerUsageUrl result is correct', function() {
      expect(EndpointsService.getPowerUsageUrl()).not.toBeUndefined();
      expect(EndpointsService.getPowerUsageUrl()).toEqual('nodes/computation_node');
    });

    it('checks if the getPowerLimitConstraintsUrl result is correct', function() {
      expect(EndpointsService.getPowerLimitConstraintsUrl()).not.toBeUndefined();
      expect(EndpointsService.getPowerLimitConstraintsUrl()).toEqual('nodes/computation_node');
    });

    it('checks if the getRuleTypesUrl result is correct', function() {
      expect(EndpointsService.getRuleTypesUrl()).not.toBeUndefined();
      expect(EndpointsService.getRuleTypesUrl()).toEqual('rule_types');
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
        'put_rule',
        'delete_rule',
        'get_power_usage',
        'get_power_limit_constraints',
        'get_rule_types'
      ]);
    });
  });
});
