/*
  SDP6x - A Low Pressor Sensor Library for Arduino.

  Supported Sensor modules:
    SDP600 series from Sensirion - http://www.futureelectronics.com/en/Technologies/Product.aspx?ProductID=SDP610125PASENSIRIONAG1050689&IM=0
  
  Created by Christopher Ladden at Modern Device on December 2009.
  Modified by Paul Badger March 2010
  Modified by www.misenso.com on October 2011:
  - code optimisation
  - compatibility with Arduino 1.0
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

#include <inttypes.h>
#include <Wire.h>
#include "Arduino.h"
#include "SDP6x.h"

#define SCALEFACTOR 60.0
// NOTE you will need to change the SCALEFACTOR to the appropriate value for your sensor
//  Don't forget the .0 at the end, it makes sure Arduino does not round the number
// SENSOR       | SDP6xx-500Pa and SDP5xx  |  SDP6x0-125Pa  |  SDP6x0-25Pa  |
// SCALEFACTOR  |         60.0             |     240.0      |     1200.0    | (1/Pa)

/******************************************************************************
 * Global Functions
 ******************************************************************************/

/**********************************************************
 * GetPressureDiff
 *  Gets the current Pressure Differential from the sensor.
 *
 * @return float - The Pressure in Pascal
 **********************************************************/
float SDP6xClass::GetPressureDiff(void)
{
  return ((float)(readSensor(ePresHoldCmd))/SCALEFACTOR);
}


/******************************************************************************
 * Private Functions
 ******************************************************************************/

int16_t SDP6xClass::readSensor(uint8_t command)
{
    int16_t result;
    //Serial.print("[readSensor]");
    Wire.beginTransmission(eSDP6xAddress);  //begin
    Wire.write(command);          //send the pointer location
    delay(100);
    Wire.endTransmission();                 //end

    Wire.requestFrom(eSDP6xAddress, 3);
    //Serial.print("[while wire start]");
    while(Wire.available() < 3) {
      ; //wait
    }
    //Serial.print("[while wire end]");

    //Store the result
    result = ((Wire.read()) << 8);
    result += Wire.read();
  result &= ~0x0003;   // clear two low bits (status bits)
    return result;
}

SDP6xClass SDP6x;


