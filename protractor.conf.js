exports.config = {
    specs: [
        'test_e2e/*spec.js'
    ],
    baseUrl: 'http://vps.lel.lu:8081/',
    rootElement: 'body',
    framework: 'jasmine2',
    capabilities: {
        browserName: 'phantomjs'
    },
    onPrepare: function () {
        browser.driver.manage().window().setSize(1920, 1080);
        browser.getCapabilities().then(function(cap){
            browser.browserName = cap.caps_.browserName;
        });
        var SpecReporter = require('jasmine-spec-reporter');
        jasmine.getEnv().addReporter(new SpecReporter({
            displayStacktrace: 'all',
            displayPendingSpec: true,
            displaySpecDuration: true,
            displaySuiteNumber: true
        }));
        var HtmlScreenshotReporter = require('protractor-jasmine2-screenshot-reporter');
        jasmine.getEnv().addReporter(new HtmlScreenshotReporter({
            dest: 'screenshots',
            filename: 'index.html'
        }));

    },
    allScriptsTimeout: 1200000,
    jasmineNodeOpts: {
        showColors: true,
        defaultTimeoutInterval: 1200000,
        print: function() {}
    }

};