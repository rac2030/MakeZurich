#include <rn2xx3.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(7, 8); // RX, TX
#define RST  2

// Copy the following lines from TTN Console -> Devices -> Overview tab -> "EXAMPLE CODE"
// You need to create an application first, then add a device 
// (run this code once and it prints the EUID which you can enter)
const char *devAddr = "xxxxxxxxxxxxx";
const char *nwkSKey = "xxxxxxxxxxxxx";
const char *appSKey = "xxxxxxxxxxxxx";

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

  myLora.initABP(devAddr, appSKey, nwkSKey);

  led_off();
  delay(2000);
}

// the loop routine runs over and over again forever:
void loop() {
  led_on();
  Serial.println("TXing");

  myLora.txUncnf("X");

  led_off();

  delay(20000);
}

void led_on()
{
  digitalWrite(13, 1);
}

void led_off()
{
  digitalWrite(13, 0);
}
