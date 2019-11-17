MSP430 Temperature Sensor
==============================

This project is a coin cell-powered wireless temperature sensor with with a
430BOOST-CC110L BoosterPack.

Since it is a wireless sensor, it depends on a receiver hub to process and
store the data. See [Wireless Sensor Receiver Hub](https://github.com/Andy4495/Wireless-Sensor-Receiver-Hub)
for an implementation of a receiver hub.

My particular design makes use of the prototyping area available on the CC110L
BoosterPack to make a compact wireless sensor with a standalone MSP430G2553
PDIP IC (see photo below). However, since the sketch uses the [MspTandV library](https://github.com/Andy4495/mspTandV), it can be compiled for any MSP430 variant
supported by the library.

Because the processor spends most of its time sleeping, and the only external
component is the low-power CC110L BoosterPack, the temperature sensor can
last for well over a year using a single 3 V lithium coin battery. On my current setup, the battery has lasted nearly three years and is still functioning normally.

If using this sketch on a standard LaunchPad, be sure to remove the jumpers
connecting the processor to the emulation section of the LaunchPad in order
to minimize power consumption.

## Program details ##

The sketch collects the following data:

- MSP430
     - Die temperature in degrees Fahrenheit * 10
         - For example, 733 represents 73.3 degrees F
     - Battery voltage (Vcc) in millivolts
         - For example, 2977 represents 2.977 volts
     - Number of times "loop()" has run since the last reboot
     - Current "millis()" value at the time of transmission

After collecting the sensor data, the data is packaged and transmitted to a
receiver hub which can then further process and store the data over time.

Note on G2553/G2452 Low Voltage Operation
-----------------------------------------

Per Figure 1 in the [MSP430G2553][1] Device Datasheet, the device should only be run at 16 Mhz with a supply voltage of 3.3 V. In order to run at lower voltages (e.g. in a battery-operated setup), you will need to configure a lower system frequency. The device can be run at a supply voltage as low as 2.2 V when running at 8 MHz. By default, Energia sets the G2553 system frequency at 16 Mhz. To run the device at 8 MHz, the `boards.txt` file needs to be edited to add an 8 MHz entry. See [this discussion](https://forum.43oh.com/topic/4094-msp430g2553-1mhz-or-16mhz-how-to-set-it/)
for tips on editing `boards.txt` to change the system frequency.

## External Libraries ##

[Calibrated Temperature and Vcc Library](https://github.com/Andy4495/mspTandV)

* Used for the internal MSP430 temperature and battery voltage measurements.

## References ##

* [CC110L BoosterPack](http://www.ti.com/tool/430BOOST-CC110L)
* [Wireless Sensor Receiver Hub](https://github.com/Andy4495/Wireless-Sensor-Receiver-Hub)

## Integrated MSP430G2553 and Coin Cell Battery on CC110L BoosterPack ##

- ![Board Front](jpg/G2-Front.jpg)
- ![Board Back](jpg/G2-Back.jpg)

[1]: http://www.ti.com/lit/ds/symlink/msp430g2553.pdf
