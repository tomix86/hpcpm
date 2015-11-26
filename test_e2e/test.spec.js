describe('HPCPM', function(){
    browser.ignoreSynchronization = true;
    browser.get('/');
    it('should show main page', function(){
        browser.waitForAngular();
        expect(browser.getTitle()).toBe('HPC Power Management');
    });
    it('should show add node modal', function(){
        element(by.css('[ng-click="OpenNewNodeModal()"]')).click();
        browser.waitForAngular();
        expect(element(by.css('[class="modal-title"]')).getText()).toBe('Add new computation node');
    });
});