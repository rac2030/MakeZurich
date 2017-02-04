var socket;
var monkeys = [];
var toDispatch = 0;


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
	
	socket.on("data", incomingData);
	socket.on("messageCode", displayMessage);
	
}

function draw() {
	// background(0);
	background("#6ACAFC");
	// image(monkeyGif, 0, 0,100,100);
	// imageMode(CENTER);
	// if (monkeyX > width - monkeyW/2) monkeyX = monkeyW/2 * -1;
	// monkeyX++;
	// xPos = 600-monkeyW - monkeyX;
	// xPos = xPos + 150;
	
	for (var i = 0; i < monkeys.length; i++) {
		monkeys[i].update();
	}
	
	for (var i = monkeys.length-1; i >= 0; i--) {
		if (monkeys[i].x > width) monkeys.splice(i,1);
		
	}
	
	// if (toDispatch > 0) {
		// if (monkeys.length > 0) {
			// if (monkeys[length].x > 100) {
				// createMonkey();
				// toDispatch--;
			// }
		// }
		// else {
			// createMonkey();
			// toDispatch--;
		// }
	// }
}


var incomingData = function (data) {
	toDispatch = toDispatch + data.countBikes;
	createMonkey();
	
	// dispatcher(data.countBikes);
	displayData(data);
}
var displayData = function (data) {
	writeToTextArea("Incoming data: "+JSON.stringify(data, null, 0));
}
var displayMessage = function (message) {
	writeToTextArea("Message: "+JSON.stringify(message, null, 0));
}

var dispatcher = function (amount) {
	for (i = 0; i < amount; i++) {
		setTimeout(createMonkey,3000*i);		
	}
}

var createMonkey = function() {
	if (toDispatch > 0) {
		monkey = new Monkey();
		monkey.loadGif();
		monkey.setCoord(0,random(0,height-150));
		monkeys.push(monkey);
		toDispatch = toDispatch - 1;
	}
}


function Monkey () {
	this.monkeyGif;
	this.w = 100;
	this.h = 100;
	this.x = 0;
	this.y = 0;
	// this.isLoaded = false;
	
	this.loadGif = function () {
		this.monkeyGif = loadGif('monkey.gif',this.initMonkey);
		console.log("loading");
	}
	this.initMonkey = function () {
		console.log("loaded");
		setTimeout(createMonkey,2000);
		console.log("to be dispatched: "+toDispatch);
		// this.isLoaded = true;
		// this.x = 600-this.w;
		// this.y = 0;
	}
	
	this.setCoord = function (x,y) {
		this.x = x;
		this.y = y;
	}
	
	this.update = function () {
		if (!this.monkeyGif.loaded()) return;
		
		// if (this.x > width - this.w/2) this.x = this.w/2 * -1;
		this.x++;
		var xPos = 600 - this.w - this.x;
		var yPos = this.y + noise(this.x/100)*100; //adding perlin noise

		push(); 
		translate(this.monkeyGif.width,0); 
		scale(-1.0,1.0); 
		image(this.monkeyGif,xPos,yPos,this.w,this.h); 
		pop();
		
		
		
	}
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

