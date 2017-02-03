/*
// This application uses express as its web server
// for more info, see: http://expressjs.com
require('dotenv').config({
	path: "envVars.env"
});
var express = require('express');

// create a new express server
var app = express();

// serve the files out of ./public as our main files
app.use(express.static(__dirname + '/public'));

// get the app environment from Cloud Foundry
var appEnv = cfenv.getAppEnv();

// start server on the specified port and binding host
var server = app.listen(appEnv.port, '0.0.0.0', function () {
		// print a message when the server starts listening
		console.log("server starting on " + appEnv.url);
	});
*/	


// #############################
// ##### LORAWAN LISTENER ######
// #############################

var hoseMap = ["0004A30B001BB30C"]

// TTN nodejs quickstart: https://www.thethingsnetwork.org/docs/applications/nodejs/quick-start.html
	
var ttn = require('ttn');

var region = 'eu'; //based on ttn-handler-eu on https://console.thethingsnetwork.org/applications/makezurichrac
var appId = 'makezurichrac'; //based on ApplicationID on https://console.thethingsnetwork.org/applications/makezurichrac
var accessKey = 'ttn-account-v2.ncRUxO-qisIjcjtpuBOHcJG-SpZmKbMKIDoILHAk-zY'; //based on default key on https://console.thethingsnetwork.org/applications/makezurichrac


    // options.username = appId;
    // options.password = appAccessKey;


var client = new ttn.Client(region, appId, accessKey);

client.on('connect', function(connack) {
  console.log('[DEBUG]', 'Connect:', connack);
});

client.on('error', function(err) {
  console.error('[ERROR]', err.message);
});

client.on('activation', function(deviceId, data) {
  console.log('[INFO] ', 'Activation:', deviceId, JSON.stringify(data, null, 2));
});

client.on('device', null, 'down/scheduled', function(deviceId, data) {
  console.log('[INFO] ', 'Scheduled:', deviceId, JSON.stringify(data, null, 2));
});

client.on('message', function(deviceId, data) {
  console.info('[INFO] ', 'Message:', deviceId, JSON.stringify(data, null, 2));
  
  var hoseId = hoseMap.indexOf(data.hardware_serial);
  var pressure = payload_raw.data[0];
  writePressureData(hoseId, pressure);
});

client.on('message', null, 'led', function(deviceId, led) {

  // Toggle the LED
  var payload = {
    led: !led
  };

  // If you don't have an encoder payload function:
  // var payload = [led ? 0 : 1];

  console.log('[DEBUG]', 'Sending:', JSON.stringify(payload));
  client.send(deviceId, payload);
});


// #################################
// ##### FIREBASE INTEGRATION ######
// #################################

//json data best practices in firebase: https://firebase.google.com/docs/database/web/structure-data


var fbEmail = "antonio.kumin@gmail.com";
var fbPw = "firebaseDB12345";

var firebase = require("firebase");

var config = {
	apiKey: "AIzaSyDap6emBiWVtZX13vaziW3iLbZ3oOOhvdU",
	authDomain: "makezurich2017.firebaseapp.com",
	databaseURL: "https://makezurich2017.firebaseio.com",
	storageBucket: "makezurich2017.appspot.com",
	messagingSenderId: "292656342457"
};
firebase.initializeApp(config);


firebase.auth().signInWithEmailAndPassword(fbEmail, fbPw).catch(function(error) {
  // Handle Errors here.
  var errorCode = error.code;
  var errorMessage = error.message;
  console.log(errorCode + ": "+ errorMessage);
  // ...
});


function writePressureData(hoseID, pressure) {
	var unixtime = (new Date).getTime();
	
	// var newPostKey = firebase.database().ref().child('pressureData').child(hoseID).push().key // this creates a hash key
	
	var newPostKey = 'hose'+hoseID;
	newPostKey += '/pressureData';
	newPostKey += '/'+unixtime;
	var postData = {
		"pressure": pressure,
		"unixtime": unixtime
	};
	
	var updates = {};
	updates[newPostKey] = postData;

	return firebase.database().ref().update(updates);
}

function readPressureData(hoseID, unixtimeStart, unixtimeEnd) {
	var pressureDataRef = firebase.database().ref('/pressureData').orderByChild('starCount');
    // [END top_posts_query]
    Promise.all([pressureDataRef.once('value')]).then(function(resp) {
      var pressureVals = resp[0].val();
	  console.log(pressureVals);
	  
    }).catch(function(error) {
      console.log('Failed to read pressure data:', error);
    });
}



/*
firebase.auth().signOut().then(function() {
  // Sign-out successful.
}, function(error) {
  // An error happened.
});
*/



// ##########################
// ##### MISCELLANEOUS ######
// ##########################

/*
var stdin = process.openStdin();

stdin.addListener("data", function(d) {
    // note:  d is an object, and when converted to a string it will
    // end with a linefeed.  so we (rather crudely) account for that  
    // with toString() and then trim() 
    console.log("you entered: [" + 
        d.toString().trim() + "]");
  });
 */

