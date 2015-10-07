exports.config = {
  seleniumAddress: 'http://localhost:4444/wd/hub',
  specs: ['ui-test.js', 'server-test.js'],
  chromeOnly: false,
  multiCapabilities: [{
    'browserName': 'phantomjs',
    'phantomjs.binary.path': require('phantomjs').path,
    'phantomjs.ghostdriver.cli.args': ['--loglevel=DEBUG']
  }],
};
