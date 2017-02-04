
#include <inttypes.h>
#include <Wire.h>
#include "Arduino.h"
#include "SDP3x.h"

/******************************************************************************
 * Global Functions
 ******************************************************************************/

/**********************************************************
 * GetPressureDiff
 *  Gets the current Pressure Differential from the sensor.
 *
 * @return float - The Pressure in Pascal
 **********************************************************/
float SDP3xClass::GetPressureDiff(void)
{
  return (float)(readSensor());
}


/******************************************************************************
 * Private Functions
 ******************************************************************************/

int16_t SDP3xClass::readSensor(void)
{
    uint8_t readData[6];
  uint8_t rxByteCount=0;
  int16_t dp_ticks;
  int16_t  temperature_ticks;

  // triggered mode with 50ms conversion time
  const uint8_t SDP_MEASUREMENT_COMMAND_0 = 0x36;
  const uint8_t SDP_MEASUREMENT_COMMAND_1 = 0x2F;
  
  uint8_t txData[2] = {SDP_MEASUREMENT_COMMAND_0, SDP_MEASUREMENT_COMMAND_1};

  Wire.beginTransmission(SDP_1_ADDRESS);
  Wire.write(txData, sizeof(txData));
  Wire.endTransmission();
  // wait for data conversion in sensor
  delay(50);

  // 2 bytes DP, 1 CRC, 2 bytes T, 1 CRC
  Wire.requestFrom(SDP_1_ADDRESS, (uint8_t)6);
  rxByteCount = 0;
  while (Wire.available()) { // wait till all arrive
      readData[rxByteCount] = Wire.read();
      rxByteCount++;
  }
  // merge chars to one int
  dp_ticks = BIU16(readData, 0);
  temperature_ticks = BIU16(readData, 3);
  // send data to serial interface with tab as spacer
// adapt scale factor according to datasheet (here for version SDP31)
float dp_scale = 60.0;
float t_scale = 200.0;
    return dp_ticks/dp_scale;
}

SDP3xClass SDP3x;


