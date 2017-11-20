/* -----------------------------------------------------------------
  /* MSP430G2553 Remote Temp Sensor
    11/18/17 - A.T. - Initial version
*/
/* -----------------------------------------------------------------
   Code structure:
   Design is specific to MSP430G2553 and CC110L BoosterPack

   setup()
     I/O and sensor setup
     Temperature calibration

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
    - None

*/

#include <SPI.h>
#include <AIR430BoostFCC.h>

// CC110L Declarations
#define ADDRESS_LOCAL   0x03
#define ADDRESS_REMOTE  0x01

struct sPacket
{
  uint8_t from;           // Local node address that message originated from
  uint8_t message[59];    // Local node message [MAX. 59 bytes]
};

struct sPacket txPacket;

int* Cal30;   // MSP chip Temp calibration @ 30C
int* Cal85;   // MSP chip Temp calibration @ 85C
long Tc;     // Tc coeffecient calculated from calibration values
// Calculated as "times 10,000" to avoid floating math


struct SensorData {
  int             MSP_T;     // Tenth degrees F
  unsigned int    Batt_mV;   // milliVolts
  unsigned int    Loops;
  unsigned long   Millis;
};

SensorData sensordata;

unsigned int loopCount = 0;
const int sleepTime = 5000;


int            ADCraw;
long           msp430T;
unsigned long  msp430mV;

void setup() {

  // CC110L Setup
  txPacket.from = ADDRESS_LOCAL;
  memset(txPacket.message, 0, sizeof(txPacket.message));
  Radio.begin(ADDRESS_LOCAL, CHANNEL_1, POWER_MAX);

  // Calculate the internal Temp Coeffecient using factory calibration values
  // See MSP430G2553 Family Guide Section 24.2.2.1, and
  // MS430G2553 User Guide Page 15
  Cal30 = (int*) 0x10e2;
  Cal85 = (int*) 0x10e4;
  // Scale the calibration factory by 10,000 to avoid floating point math
  Tc = 550000L / (*Cal85 - *Cal30);
}


void loop() {

  loopCount++;

  // MSP430 internal temp sensor
  analogReference(INTERNAL1V5);
  ADCraw = analogRead(TEMPSENSOR);
  ADCraw = analogRead(TEMPSENSOR);
  // Calibration factor Tc is scaled by 10,000 to avoid floating point math,
  // so adjust temp calculation and conversion to degrees F accordingly
  msp430T = (Tc * (ADCraw - (*Cal30)) + 300000L) * 18L / 10000L + 320L;

  // MSP430 battery voltage (Vcc) calibrated against reference
  // Start with 2.5V reference, which requires Vcc >= 2.9 V
  // Once Vcc is below 2.95V, then switch to 1.5V reference.
  // Vcc/2 input is on ADC channel 11
  analogReference(INTERNAL2V5);
  ADCraw = analogRead(128 + 11);
  // Need calculation to be Long int due to mV scaling
  msp430mV = ADCraw * 5000L;
  msp430mV = msp430mV / 1023L;
  // Use 1.5V reference if Vcc < 2.95V
  if (msp430mV < 2950) {
    analogReference(INTERNAL1V5);
    ADCraw = analogRead(128 + 11);
    msp430mV = ADCraw * 3000L;
    msp430mV = msp430mV / 1023L;
  }

  sensordata.MSP_T = msp430T;
  sensordata.Batt_mV = msp430mV;
  sensordata.Loops = loopCount;
  sensordata.Millis = millis();

  // Send the data over-the-air
  memcpy(&txPacket.message, &sensordata, sizeof(sensordata));
  Radio.transmit(ADDRESS_REMOTE, (unsigned char*)&txPacket, sizeof(sensordata) + 4);

  sleep(sleepTime);
}
