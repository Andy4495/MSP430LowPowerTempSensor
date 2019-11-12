/* -----------------------------------------------------------------
  MSP430 Remote Temp Sensor
  https://github.com/Andy4495/MSP430LowPowerTempSensor

    11/18/17 - A.T. - Initial version
    01/22/18 - A.T. - Updated to use MspTandV library
    02/01/18 - A.T. - Update message structure to align on word boundary.
*/
/* -----------------------------------------------------------------
   Code structure:
   Design requires CC110L BoosterPack (430BOOST-CC110L).

   setup()
     Radio setup

   loop()
   Collect and send the following data:
   - MSP430: Internal Temperature                F * 10
   - MSP430: Supply voltage (Vcc)                mV
   - Internal Timing:
                  # of times loop() has run
                  Current value of millis()
*/

/*
    External libraries:
    - MspTandV https://github.com/Andy4495/mspTandV

*/

#include <SPI.h>
#include <AIR430BoostFCC.h>
#include "MspTandV.h"

// CC110L Declarations
#define ADDRESS_LOCAL   0x03
#define ADDRESS_REMOTE  0x01    // Receiver hub address

enum {WEATHER_STRUCT, TEMP_STRUCT};

struct sPacket
{
  uint8_t from;           // Local node address that message originated from
  uint8_t struct_type;    // Flag to indicate type of message structure
  uint8_t message[58];    // Local node message
};

struct sPacket txPacket;


struct SensorData {
  int             MSP_T;     // Tenth degrees F
  unsigned int    Batt_mV;   // milliVolts
  unsigned int    Loops;
  unsigned long   Millis;
};

SensorData sensordata;

MspTemp myTemp;
MspVcc  myVcc;

unsigned int loopCount = 0;
const unsigned long sleepTime = 47000;

void setup() {

  // CC110L Setup
  txPacket.from = ADDRESS_LOCAL;
  txPacket.struct_type = TEMP_STRUCT;
  memset(txPacket.message, 0, sizeof(txPacket.message));
  Radio.begin(ADDRESS_LOCAL, CHANNEL_1, POWER_MAX);

}


void loop() {

  loopCount++;

  myTemp.read();
  myVcc.read();

  sensordata.MSP_T = myTemp.getTempCalibratedF();
  sensordata.Batt_mV = myVcc.getVccCalibrated();
  sensordata.Loops = loopCount;
  sensordata.Millis = millis();

  // Send the data over-the-air
  memcpy(&txPacket.message, &sensordata, sizeof(sensordata));
  Radio.transmit(ADDRESS_REMOTE, (unsigned char*)&txPacket, sizeof(sensordata) + 4);

  sleep(sleepTime);
}
