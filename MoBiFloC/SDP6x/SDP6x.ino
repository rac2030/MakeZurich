/****************************************************************
 * ReadSDP6x
 *  An example sketch that reads the sensor and prints the
 *  Differential Pressure to the PC's serial port
 *
 *  Tested with:
 *    - SDP600/610-125Pa (bought from FutureElectronics.com)
 ***************************************************************/

#include <Wire.h>
#include "SDP6x.h"

float difPressure;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  difPressure = 0.0;
}

void loop()
{
  Serial.print("Pressure Differential (Pa): ");
  difPressure = SDP6x.GetPressureDiff();
  Serial.print(difPressure);
  Serial.print("\n");
  delay(1000);
}



