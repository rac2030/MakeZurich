#include <rn2xx3.h> // Library for the LoRaWAN module RN2438 from microchip
#include <SoftwareSerial.h>
#include <Wire.h> // Needed for i2c
#include "SDP6x.h" // Library to read from SDP610 differential presure sensor

float difPressure; // Variable that holds the last reading of the SDP610

SoftwareSerial mySerial(7, 8); // RX, TX
#define RST  2


// README FIRST:
// Rename or copy the File TTNCredentials.h.editme and remove the .editme ending.
// Edit it it accordingly and put your TTN keys in there, make sure you add it to git ignores.
// Here we load it to be used, this file is kept out of GIT as its the private parts
#include "TTNCredentials.h"

rn2xx3 myLora(mySerial);

// Setup routine runs once when you press reset
void setup() {
  pinMode(13, OUTPUT);
  led_on();

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("Startup");

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
  
  led_off();

  // Initialize I2C communication
  Wire.begin();
  // Set the starting value for the pressure difference to 0
  difPressure = 0.0;
  delay(2000);
  
}

// Just for testing sending a float
static uint8_t floatdata[] = { 0,0,0,0,0,0,0,0};

// the loop routine runs over and over again forever
void loop() {
  led_on();
  readPressure();
  Serial.print("TXing: ");
  Serial.println(difPressure);
  dtostrf(difPressure,5,2,(char*)floatdata);
  myLora.txUncnf(floatdata); // non blocking call
  //myLora.txCnf(difPressure);    // blocking call
  
  led_off();

  delay(20000);
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
