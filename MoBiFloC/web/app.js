var port = 3000;
var express = require("express");
var app = express();
var server = app.listen(port);
app.use(express.static('public')); //makes the folder "public" be used in express
console.log("connected via port "+port);

// ######################
// #### SOCKETS: I/O ####
// ######################

var clients = [];
var socket = require('socket.io');
var io = socket(server);
io.sockets.on('connection', newConnection); //event listener for a connection


function newConnection (client) { //insert here all functions for a connection
	var newConfig = {};
	console.log("new client connected: " +client.id);
	
	client.on("config", function (data) { 
		this.newConfig = data;
		console.log(JSON.stringify(this.newConfig, null, 2));
	});
	
	clients.push(client);
	
	
	
	
	
	// var messageObj = {
		  // "countBikes": 3,
		  // "temperature": 2,
		  // "humidity": 3,
		  // "avgSpeed": 4
	  // };
	// writeData(messageObj);
	// client.emit("data",messageObj);

}


// #############################
// ##### LORAWAN LISTENER ######
// #############################


// TTN nodejs quickstart: https://www.thethingsnetwork.org/docs/applications/nodejs/quick-start.html
	
var ttn = require('ttn');

var region = 'eu'; //based on ttn-handler-eu on https://console.thethingsnetwork.org/applications/makezurichrac
var appId = 'makezurichrac'; //based on ApplicationID on https://console.thethingsnetwork.org/applications/makezurichrac
var accessKey = 'ttn-account-v2.ncRUxO-qisIjcjtpuBOHcJG-SpZmKbMKIDoILHAk-zY'; //based on default key on https://console.thethingsnetwork.org/applications/makezurichrac

var dataObj = {};
var bufferObj = {};

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
  // console.info('[INFO] ', 'Message:', deviceId, JSON.stringify(data, null, 2));
  
  dataObj = data;
  bufferObj = new Buffer(data.payload_raw, 'hex')
  
  var message_s = bufferObj.toString('ascii');
  console.log(message_s);
  
  
  
  if (message_s.charAt(0) == "m") {
	  for (var i = 0; i < clients.length; i++) {
		  var thisClient = clients[i];
			thisClient.emit("messageCode",message_s);
	  }
  }
  else {

	  var data_a = message_s.split(";");
	  
	  var messageObj = {
		  "countBikes": data_a[0],
		  "temperature": data_a[1],
		  "humidity": data_a[2],
		  "avgSpeed": data_a[3]
	  }
	  console.log(JSON.stringify(messageObj, null, 2));
	  
	  for (var i = 0; i < clients.length; i++) {
		  var thisClient = clients[i];
			thisClient.emit("data",messageObj);
	  }
  }

  //var pressure = isPropertyOf(data.payload_raw.data[0];
  //writePressureData(hoseId, pressure);
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


function writeData(data) {
	var unixtime = (new Date).getTime();
	
	// var newPostKey = firebase.database().ref().child('pressureData').child(hoseID).push().key // this creates a hash key
	
	var newPostKey = '/data';
	newPostKey += '/'+unixtime;
	var postData = data;
	Object.assign(postData,{ "unixtime": unixtime });
	
	var updates = {};
	updates[newPostKey] = postData;

	return firebase.database().ref().update(updates);
}

function readPressureData(unixtimeStart, unixtimeEnd) {
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

