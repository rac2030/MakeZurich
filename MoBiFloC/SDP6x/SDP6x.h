/*
  SDP6x - A Low Pressor Sensor Library for Arduino.

  Supported Sensor modules:
    SDP600 series from Sensirion - http://www.futureelectronics.com/en/Technologies/Product.aspx?ProductID=SDP610125PASENSIRIONAG1050689&IM=0
  
  SHT2x Created by Christopher Ladden at Modern Device on December 2009.
  SDP6x Modified above by Antony Burness Jun 2016.
    - adapted for the I2C Pressure sensor
    - using example from sensirion https://www.sensirion.com/products/differential-pressure-sensors/all-documents-of-sensirions-differential-pressure-sensors-for-download/

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef SDP6X_H
#define SDP6X_H

#include <inttypes.h>

//Checksum CRC-8 polynomial generator (to check the recieved message is undamaged)
#define POLYNOMIAL 0x131 // P(x) = x^8 + x^5 + x^4 -1 = 100110001

typedef enum {
    eSDP6xAddress = 0x40, //(64)
} PRES_SENSOR_P;

typedef enum {
   ePresHoldCmd = 0xF1, //command to trigger a pressure measurement
   eSoftReset = 0xFE,   //command: soft reset
   eReadUserReg = 0xE5, //command: read advanced user register
   eWriteUserReg = 0xE4 //command: write advanced user register
} PRES_MEASUREMENT_CMD_P;

typedef enum{
  RESOLUTION_9BIT = 0x00,
  RESOLUTION_10BIT = 0x01,
  RESOLUTION_11BIT = 0x02,
  RESOLUTION_12BIT = 0x03,
  RESOLUTION_13BIT = 0x04,
  RESOLUTION_14BIT = 0x05,
  RESOLUTION_15BIT = 0x06,
  RESOLUTION_16BIT = 0x07,
} etSensorResolutions;

class SDP6xClass
{
  private:
  int16_t readSensor(uint8_t command);

  public:
    float GetPressureDiff(void);
};

extern SDP6xClass SDP6x;

#endif


