describe('DataService', function () {
    beforeEach(module('hpcpm-ui'));

    var Restangular, EndpointsService, DataService;


    beforeEach(inject(function (_Restangular_, _EndpointsService_, _DataService_) {
        Restangular = _Restangular_;
        EndpointsService = _EndpointsService_;
        DataService = _DataService_;
    }));

    beforeEach(function () {
        spyOn(Restangular, 'all').and.callFake(function () {
            return {
                getList: function () {
                    return 'get_all';
                }
            };
        });
        spyOn(Restangular, 'one').and.callFake(function () {
            return {
                put: function () {
                    return 'put';
                },
                remove: function () {
                    return 'remove';
                }
            };
        });
    });

    it('should get all nodes', function () {
        spyOn(EndpointsService, 'getAllComputationNodesUrl').and.callFake(function () {
            return 'param';
        });
        expect(DataService.getAllComputationNodes()).toEqual('get_all');
        expect(Restangular.all).toHaveBeenCalledWith('param');
    });

    it('should add node', function () {
        spyOn(EndpointsService, 'putComputationNodeUrl').and.callFake(function () {
            return 'param';
        });
        expect(DataService.addComputationNode({name: 'name'})).toEqual('put');
        expect(Restangular.one).toHaveBeenCalledWith('param', 'name');
    });

    it('should remove node', function () {
        spyOn(EndpointsService, 'deleteComputationNodeUrl').and.callFake(function () {
            return 'param';
        });
        expect(DataService.removeComputationNode('name')).toEqual('remove');
        expect(Restangular.one).toHaveBeenCalledWith('param', 'name');
    });

});

