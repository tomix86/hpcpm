describe('HPCPM', function () {
    browser.ignoreSynchronization = true;

    var node = {
        name: 'integration-test-node',
        address: 'vps.lel.lu',
        port: '40001'
    };
    var pageTitle = 'HPC Power Management';

    var HPCPMPage = require('./pages/hpcpm.page.js');
    page = new HPCPMPage();
    page.get();

    describe('Adding node', function () {
        it('should show main page', function () {
            expect(page.title.get()).toBe(pageTitle);
        });

        it('should show add node modal', function () {
            page.addNodeButton.click();
            expect(page.modal.title.get()).toBe('Add new computation node');
        });

        it('should add new node', function () {
            page.modal.nodeName.set(node.name);
            page.modal.nodeAddress.set(node.address);
            page.modal.nodePort.set(node.port);
            page.modal.addButton.click();

            browser.wait(page.toasts.any, 5000);
            expect(page.toasts.top.title.get()).toBe('Success');
            expect(page.toasts.top.content.get()).toBe('Computation node ' + node.name + ' added successfully');

            browser.driver.sleep(2000);
            var row = page.nodesTable.nodeRow(node.name).get();
            expect(row).toBeDefined();
        });

        it('should make node added toast disappear', function () {
            browser.wait(page.toasts.empty, 6000);
        })
    });
    describe('Node details', function() {
        it ('should show node details page', function () {
            page.nodesTable.nodeRow(node.name).nameField().link.click()
            expect(page.pageHeader.get()).toBe('Node: ' + node.name)
        })
    })
});