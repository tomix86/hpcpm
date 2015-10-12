var config = require('./config.js'); // plain config file
var express = require('express');
var app = express(); // create our app w/ express
var morgan = require('morgan'); // log requests to the console (express4)
var bodyParser = require('body-parser'); // pull information from HTML POST (express4)
var methodOverride = require('method-override'); // simulate DELETE and PUT (express4)
// configuration =================

app.use(function(req, res, next) {
  res.setHeader("Access-Control-Allow-Origin", "*"); // set the Access-Control-Allow-Origin header
  return next();
});

app.use(express.static(__dirname + '/public')); // set the static files location /public/img will be /img for users
app.use(morgan(config.morganLevel)); // log every request to the console
app.use(bodyParser.urlencoded({
  'extended': 'true'
})); // parse application/x-www-form-urlencoded
app.use(bodyParser.json()); // parse application/json
app.use(bodyParser.json({
  type: 'application/vnd.api+json'
})); // parse application/vnd.api+json as json
app.use(methodOverride());

// static resources paths
app.use(express.static(__dirname + config.publicPath));
app.use('/bower_components', express.static(__dirname + config.bowerPath));

app.get('*', function(req, res) {
  res.sendFile('index.html', {
    root: path.join(__dirname, '/public')
  }); // load the single view file (angular will handle the page changes on the front-end)
});

// listen (start app with node server.js) ======================================
app.listen(config.port);
console.log("App listening on port " + config.port);
