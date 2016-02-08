// Karma configuration
// Generated on Wed Nov 18 2015 13:16:11 GMT+0100 (Środkowoeuropejski czas stand.)

module.exports = function(config) {
  config.set({

    // start these browsers
    // available browser launchers: https://npmjs.org/browse/keyword/karma-launcher
    browsers: //['Chrome_without_security'],//, 'PhantomJS', 'Chrome'],
              ['PhantomJS'],
    // you can define custom flags
    customLaunchers: {
      Chrome_without_security: {
        base: 'Chrome',
        flags: ['--disable-web-security']
      }
    },


    // used plugins list
    plugins : [
      'karma-htmlfile-reporter',
      'karma-jasmine',
      'karma-phantomjs-launcher',
      'karma-spec-reporter',
      'karma-chrome-launcher',
      'karma-coverage'
    ],


    // reporters used
    reporters: ['spec', 'html', 'coverage'],


    // htmlReporter config
    htmlReporter: {
        outputFile: 'test_report/result.html',
        pageTitle: 'HPCPM UI Unit Tests'
    },


    // base path that will be used to resolve all patterns (eg. files, exclude)
    basePath: '',


    // frameworks to use
    // available frameworks: https://npmjs.org/browse/keyword/karma-adapter
    frameworks: ['jasmine'],


    // list of files / patterns to load in the browser
    files: [
      'bower_components/angular/angular.js',
      'bower_components/angular-mocks/angular-mocks.js',
      'bower_components/jquery/dist/jquery.min.js',
      'bower_components/json-formatter/dist/json-formatter.min.js',
      'bower_components/angular-animate/angular-animate.min.js',
      'bower_components/angular-ui-router/release/angular-ui-router.min.js',
      'bower_components/angular-resource/angular-resource.min.js',
      'bower_components/restangular/dist/restangular.min.js',
      'bower_components/underscore/underscore-min.js',
      'bower_components/ng-table/dist/ng-table.min.js',
      'bower_components/angular-bootstrap/ui-bootstrap-tpls.min.js',
      'bower_components/angularjs-toaster/toaster.min.js',
      'bower_components/Chart.js/Chart.min.js',
      'bower_components/angular-chart.js/dist/angular-chart.min.js',
      'bower_components/bootstrap/dist/js/bootstrap.min.js',
      'bower_components/moment/min/moment.min.js',
      'bower_components/angular-moment/angular-moment.min.js',
      'bower_components/eonasdan-bootstrap-datetimepicker/build/js/bootstrap-datetimepicker.js',

      'public/js/HomeControllers.js',
      'public/js/EndpointsService.js',
      'public/js/DataService.js',
      'public/js/NewNodeController.js',
      'public/js/RemoveNodeController.js',
      'public/js/NodeDetailsController.js',
      'public/js/NodeDetailsModalController.js',
      'public/js/DeviceStatsShowController.js',
      'public/js/PowerLimitModalController.js',
      'public/js/RemovePowerLimitModalController.js',

      'test/*.js'
    ],


    // list of files to exclude
    exclude: [
      'test/conf.js'
    ],


    // preprocess matching files before serving them to the browser
    // available preprocessors: https://npmjs.org/browse/keyword/karma-preprocessor
    preprocessors: {
      'public/js/*.js': ['coverage']
    },


    // coverage reporter config
    coverageReporter: {
      type : 'html',
      dir : 'test_report/coverage/'
    },

    // web server port
    port: 9876,


    // enable / disable colors in the output (reporters and logs)
    colors: true,


    // level of logging
    // possible values: config.LOG_DISABLE || config.LOG_ERROR || config.LOG_WARN || config.LOG_INFO || config.LOG_DEBUG
    logLevel: config.LOG_INFO,


    // enable / disable watching file and executing tests whenever any file changes
    autoWatch: false,


    // Continuous Integration mode
    // if true, Karma captures browsers, runs the tests and exits
    singleRun: true,

    // Concurrency level
    // how many browser should be started simultanous
    concurrency: Infinity
  })
}
