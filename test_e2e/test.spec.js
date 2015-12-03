describe('HPCPM', function () {

    var HPCPMPage = require('./pages/hpcpm.page.js');
    page = new HPCPMPage();
    page.get();
    var nodeName = 'integration-test-node';
    var pageTitle = 'HPC Power Management';
    var toasts = element(by.id('toast-container')).all(by.tagName('div'));

    browser.ignoreSynchronization = true;

    it('should show main page', function () {
        expect(page.title.get()).toBe(pageTitle);
    });

    it('should show add node modal', function () {
        page.addNodeButton.click();
        expect(page.modal.title.get()).toBe('Add new computation node');
    });

    it('should add new node', function () {
        page.modal.nodeName.set(nodeName);
        page.modal.nodeAddress.set('vps.lel.lu');
        page.modal.nodePort.set('40001');
        element(by.css('[class="modal-dialog"]')).element(by.partialButtonText('Add')).click();
        browser.wait(function () {
            return toasts.count().then(function (count) {
                return count > 0;
            });
        }, 5000);
        expect(toasts.get(1).getText()).toBe('Success');
        expect(toasts.get(2).getText()).toBe('Computation node ' + nodeName + ' added successfully');
        browser.driver.sleep(2000);
        var row = page.nodesTable.getNodeRow(nodeName);
        expect(row).toBeDefined();

    });


    it('should make node added toast disappear', function () {
        browser.wait(function () {
            return toasts.count().then(function (count) {
                return count === 0;
            });
        }, 5000);

    })
});