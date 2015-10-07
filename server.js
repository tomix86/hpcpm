var config = require('./config.js'); // plain config file
var express = require('express');
var app = express(); // create our app w/ express
var mongoose = require('mongoose'); // mongoose for mongodb
var morgan = require('morgan'); // log requests to the console (express4)
var bodyParser = require('body-parser'); // pull information from HTML POST (express4)
var methodOverride = require('method-override'); // simulate DELETE and PUT (express4)
var passport = require('passport'); // authentication
var mongo = require('mongoskin');
// configuration =================

mongoose.connect(config.mongoUri); // connect to mongoDB database on modulus.io
var db = mongo.db("mongodb://localhost:27017/test", {native_parser:true});

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

var Endpoints = mongoose.model('Endpoints', {
  name: String,
  endpoint: String
});

// passport config
db.bind('user');
var LocalStrategy = require('passport-local').Strategy;

passport.use(new LocalStrategy({
    // set the field name here
    usernameField: 'login',
    passwordField: 'password'
  },
  function(login, password, done) {
    // get the username and password from the input arguments of the function
    var user = db.user.findOne({'login': login}); // query the user from the database
    if(!user) {
      return done(null, false, {message: "The user is not exist"});
    }
    else if(password !== user.password) {
      return done(null, false, {message: "Wrong password"});
    }
    else {
      return done(null, user);
    }
  }
));

passport.serializeUser(function(user, done) {
  done(null, user._id.valueOf());
});

passport.deserializeUser(function(id, done) {
  // query the current user from database
  var user = db.user.findOne({'_id': ObjectId(id)});
  if(!user) {
    done(new Error('User ' + id + ' does not exist'));
  }
  else {
    done(null, user);
  }
});

// default routing
app.get('/login', loginGet);
function loginGet(req, res) {
  if(req.user){
    // already logged in
    res.redirect('/');
  } else {
    // not logged in, show the login form, remember to pass the message
    // for displaying when error happens
    res.sendFile('login.html', { message: req.session.messages, root: path.join(__dirname, '/public') });
    // and then remember to clear the message
    req.session.messages = null;
  }
}

app.post('/login', loginPost);
function loginPost(req, res, next) {
  // ask passport to authenticate
  passport.authenticate('local', function(err, user, info) {
    if (err) {
      // if error happens
      return next(err);
    }

    if (!user) {
      // if authentication fail, get the error message that we set
      // from previous (info.message) step, assign it into to
      // req.session and redirect to the login page again to display
      req.session.messages = info.message;
      return res.sendFile('login.html', {
        root: path.join(__dirname, '/public')});
    }

    // if everything's OK
    req.logIn(user, function(err) {
      if (err) {
        req.session.messages = "Error";
        return next(err);
      }

      // set the message
      req.session.messages = "Login successfully";
      return res.sendFile('index.html', {
        root: path.join(__dirname, '/public')});
    });

  })(req, res, next);
}

app.get('*', function(req, res) {
  res.sendFile('index.html', {
    root: path.join(__dirname, '/public')
  }); // load the single view file (angular will handle the page changes on the front-end)
});

// listen (start app with node server.js) ======================================
app.listen(config.port);
console.log("App listening on port " + config.port);
