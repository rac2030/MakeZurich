#include <rn2xx3.h> // Library for the LoRaWAN module RN2438 from microchip
#include <SoftwareSerial.h>
#include <Wire.h> // Needed for i2c
#include "SDP6x.h" // Library to read from SDP610 differential presure sensor
#include "Adafruit_SHT31.h" // Library for temperature and humidity sensor

float difPressure; // Variable that holds the last reading of the SDP610

SoftwareSerial mySerial(7, 8); // RX, TX
#define RST  2

// Tresholds for filtering out noise and non bikes
#define UPPER_TRESHOLD 200.0
#define LOWER_TRESHOLD -200.0

// How long to ignore new spikes in bike detection after a successful count
#define COUNTING_PAUSE_TIME 3000

// Recording cycle running variables, will be reset whenever a message is full or time is up
// Count of bikes in the current rexording cycle
unsigned int bikeCount = 0;
// Start time of the current recording cycle
unsigned long cycleStartTime = 0;
unsigned long cycleEndTime = 0;
// duty time of a cycle
long cycleDuration = 60000; // 1 minute (only for PoC, should be 10 minutes later)

// README FIRST:
// Rename or copy the File TTNCredentials.h.editme and remove the .editme ending.
// Edit it it accordingly and put your TTN keys in there, make sure you add it to git ignores.
// Here we load it to be used, this file is kept out of GIT as its the private parts
#include "TTNCredentials.h"

Adafruit_SHT31 sht31 = Adafruit_SHT31();

// Debugging options
#define debug FALSE

rn2xx3 myLora(mySerial);

// Setup routine runs once when you press reset
void setup() {
  pinMode(13, OUTPUT);
  led_on();

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.println("Startup");
  setupRN2483();
  setupSDP610();
  setupSHT31();
   
  led_off();
  delay(2000);
  startReportingCycle();
  
  myLora.txUncnf("mRqCnfgSetup");
  }

// Just for testing the snippet I used to send a float
// static uint8_t floatdata[] = { 0,0,0,0,0,0,0,0};
// Example on how to send a float
// dtostrf(floatVar, minStringWidthIncDecimalPoint, numVarsAfterDecimal, charBuf);
// dtostrf(difPressure,5,2,(char*)floatdata);  
// myLora.txUncnf(floatdata); // non blocking call
// myLora.txCnf(difPressure);    // blocking call

// the loop routine runs over and over again forever
void loop() {
  cycleEndCheck();
  led_on();
  detectBicycle();
}

void startReportingCycle() {
  cycleStartTime = millis();
}

void cycleEndCheck() {
  cycleEndTime = millis();
  // Calculate the current duration
  long duration = cycleEndTime - cycleStartTime;
  //Serial.println("Current duration: " + String(duration) + " with a max duration of " + String(cycleDuration) );
  //Serial.println("cycleSartTime: " + String(cycleStartTime) + " ; sycleEndTime: " + cycleEndTime);
  
  // compare with max duration
  if ( duration > cycleDuration ) {
   // We reached the end of the cycle
   finishReportingCycle(); 
  } 
}

void finishReportingCycle() {
  //TODO implement
  // Datastructure: Count;Temparature;Humidity;Average Speed
  // TODO GPS, for now we leave it out
  String gpspos = "Unknown";
  // TODO get temp and humidity
  float temparature = sht31.readTemperature();
  float humidity = sht31.readHumidity();
  float averageSpeed = 0.0;
  // Send message
  String msg = String(bikeCount) + ";" + String(temparature, DEC) + ";" + String(humidity, DEC) + ";" + String(averageSpeed);
  Serial.println("Transmitting [" + msg + "]");
  myLora.txUncnf(msg); // non blocking call
  // Reset cylce
  bikeCount = 0;
  cycleEndTime = 0;
  // Start a new cycle
  cycleStartTime = millis();
}

void detectBicycle(){
  readPressure();
  debug("difPresure: " + String(difPressure));
  // Check if it is above the upper and below the lower treshold
  if(difPressure <= LOWER_TRESHOLD || UPPER_TRESHOLD <= difPressure){
    // TODO we might want to measure time to later calculate speed
    // Block and wait until it is between again
    while(LOWER_TRESHOLD <= difPressure &&  difPressure <= UPPER_TRESHOLD) {
      led_off(); // Make it blink quickly to show it is triggered
      delay(5);
      led_on();
      readPressure();
    }
    // Count it as bike
    bikeCount++;
    debug("Counted a new bike");
    led_off();
    // Take a break before continuing counting
    delay(COUNTING_PAUSE_TIME);
  }
}

void setupSHT31(){
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
}

void setupSDP610(){
  // Initialize I2C communication
  Wire.begin();
  // Set the starting value for the pressure difference to 0
  difPressure = 0.0;
}

void setupRN2483(){
  mySerial.begin(9600);
  // Reset rn2483
  pinMode(RST, OUTPUT);
  digitalWrite(RST, HIGH);
  digitalWrite(RST, LOW);
  delay(500);
  digitalWrite(RST, HIGH);

  // Initialise the rn2483 module
  myLora.autobaud();

  Serial.println("When using OTAA, register this DevEUI: ");
  Serial.println(myLora.hweui());
  Serial.print("RN2483 version number: ");
  Serial.println(myLora.sysver());

  //configure your keys and join the network
  Serial.println("Trying to join TTN");
  bool join_result = false;

  join_result = myLora.initABP(devAddr, appSKey, nwkSKey);
  while(!join_result)
  {
    Serial.println("Unable to join. Are your keys correct, and do you have TTN coverage?");
    delay(60000); //delay a minute before retry
    join_result = myLora.init();
  }
  Serial.println("Successfully joined TTN");
}

void readPressure() {
  difPressure = SDP6x.GetPressureDiff();
}

void led_on()
{
  digitalWrite(13, 1);
}

void led_off()
{
  digitalWrite(13, 0);
}

void debug(String msg) {
  if(debug) {
    Serial.println("[" + String(millis()) + "]" + msg);
  }
}


