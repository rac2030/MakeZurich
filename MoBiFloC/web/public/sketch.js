var socket;

function setup() {

	
	
	createP("Time treshold: ").id('timeThresholdP').child(createInput(500).id('timeThresholdI'))
	createP("Pressure treshold: ").id('pressureThresholdP').child(createInput(200).id('pressureThresholdI'))
	createP("").child(createButton("send new config").mousePressed(sendNewConfig));
	
	//event listener to receive from server
	
	createCanvas(600, 300);
	
	createP("");
	createElement('textarea').id('loraLog').style('width','100%').style('height','100px')
	
	//connects to the server
	socket = io.connect('http://localhost:3000'); //io variable comes from importing socket.io
	socket.on( 'connect', socketConnect);
	socket.on( 'disconnect', socketDisconnect);
	socket.on( 'connect_failed', socketFailed);
	socket.on( 'error', socketErr);
	
	socket.on("data", displayData);
	socket.on("messageCode", displayMessage);
	
}

function draw() {
	background(0);
	
}


var displayMessage = function (message) {
	writeToTextArea("Message: "+JSON.stringify(message, null, 0));
}
var displayData = function (data) {
	writeToTextArea("Incoming data: "+JSON.stringify(data, null, 0));
}


// ######################
// #### SOCKETS: I/O ####
// ######################


var sendNewConfig = function () {
	var timeThreshold = JSON.parse(select('#timeThresholdI').value());
	var pressureThreshold = JSON.parse(select('#pressureThresholdI').value());
	var newConfig = { "timeThreshold": timeThreshold, "pressureThreshold": pressureThreshold };
	writeToTextArea("Sending new config: "+JSON.stringify(newConfig, null, 0))
	socket.emit("config", newConfig);
}

var socketConnect = function() {
	writeToTextArea("Socket connected");
};

var socketDisconnect = function() {
	writeToTextArea("Socket disconnected");
};

var socketFailed = function() {
	writeToTextArea("Socket connection failed");
};

var socketErr = function() {
	writeToTextArea("Socket error");
};


// ##########################
// ##### MISCELLANEOUS ######
// ##########################

var writeToTextArea = function (message) {
	var currentdate = new Date();
	var dateString = "["+currentdate.getHours() + ":" + currentdate.getMinutes() + ":" + currentdate.getSeconds()+"] ";
	var textareaString = dateString + message;
	select('#loraLog').html(textareaString+"\n"+select('#loraLog').value())
}

