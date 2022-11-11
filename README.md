# MSP430 Temperature Sensor

[![Arduino Compile Sketches](https://github.com/Andy4495/MSP430LowPowerTempSensor/actions/workflows/arduino-compile-sketches.yml/badge.svg)](https://github.com/Andy4495/MSP430LowPowerTempSensor/actions/workflows/arduino-compile-sketches.yml)
[![Check Markdown Links](https://github.com/Andy4495/MSP430LowPowerTempSensor/actions/workflows/CheckMarkdownLinks.yml/badge.svg)](https://github.com/Andy4495/MSP430LowPowerTempSensor/actions/workflows/CheckMarkdownLinks.yml)

This project is a coin cell-powered wireless temperature sensor with with a [430BOOST-CC110L BoosterPack][4].

Since it is a wireless sensor, it depends on a receiver hub to process and store the data. See [Wireless Sensor Receiver Hub][5] for an implementation.

My particular design makes use of the prototyping area available on the CC110L BoosterPack to make a compact wireless sensor with a standalone MSP430G2553 PDIP IC (see photo below). However, since the sketch uses the [MspTandV library][6], it can be compiled for any MSP430 variant supported by the library.

Because the processor spends most of its time sleeping and the only external component is the low-power CC110L BoosterPack, the temperature sensor can last for a considerable amount of time using a single 3 V lithium coin cell. A CR2032 cell lasted over three years on my setup before having to be replaced.

If using this sketch on a standard LaunchPad, be sure to remove the jumpers connecting the processor to the emulation section of the LaunchPad in order to minimize power consumption.

## Program details

The sketch collects the following data from the MSP430 processor:

- Die temperature in degrees Fahrenheit * 10
  - For example, 733 represents 73.3 degrees F
- Battery voltage (Vcc) in millivolts
  - For example, 2977 represents 2.977 volts
- Number of times `loop()` has run since the last reboot
- Current `millis()` value at the time of transmission

The data is packaged and transmitted to a [receiver hub][5] which can then further process and store the data over time.

## Note on G2553/G2452 Low Voltage Operation

Per Figure 1 in the [MSP430G2553][1] Device Datasheet, the device should only be run at the full 16 Mhz when it has a supply voltage of 3.3 V. In order to run at lower voltages (e.g. in a battery-operated setup), you will need to configure a lower system frequency. The device can be run at a supply voltage as low as 2.2 V when running at 8 MHz.

By default, the MPS430 boards package sets the G2553 system frequency at 16 Mhz. To run the device at 8 MHz, the `boards.txt` file needs to be edited to add an 8 MHz entry. This repo contains an edited [`boards.txt`](./boards.txt) file which is based on the MSP 1.0.7 board package. An 8 MHz entry for the G2553 processor has been added, along with editing the original G2553 entry to clarify that it is 16 MHz.

The boards.txt file used by Energia is located at `~/Library/Energia15/packages/energia/hardware/msp430/1.0.7` on MacOS, and a similar path on Windows and Linux.

The boards.txt file used by Arduino or Visual Studio Code is located at `~/Library/Arduino15/packages/energia/hardware/msp430/1.0.5` on MacOS, and a similar path on Windows and Linux. Note that the specific package version (1.0.5 in this case) may be different depending on what you have installed.

## External Libraries

- [Calibrated Temperature and Vcc Library][6]
  - Used for the internal MSP430 temperature and battery voltage measurements.

## References

- CC110L BoosterPack obsolete catalog page from [Digikey][2]
- CC110L BoosterPack [Quick Start Guide][4]
- [Wireless Sensor Receiver Hub][5]
- [Discussion][7] for tips on editing `boards.txt` to change the system frequency

## License

The software and other files in this repository are released under what is commonly called the [MIT License][100]. See the file [`LICENSE.txt`][101] in this repository.

## Integrated MSP430G2553 and Coin Cell Battery on CC110L BoosterPack

- ![Board Front](jpg/G2-Front.jpg)
- ![Board Back](jpg/G2-Back.jpg)

[1]: https://www.ti.com/lit/ds/symlink/msp430g2553.pdf
<!-- markdown-link-check-disable-next-line -->
[2]: https://www.digikey.com/en/products/detail/texas-instruments/430BOOST-CC110L/2812673
[4]: https://www.ti.com/lit/ml/swru312b/swru312b.pdf
[5]: https://github.com/Andy4495/Wireless-Sensor-Receiver-Hub
[6]: https://github.com/Andy4495/mspTandV
[7]: https://forum.43oh.com/topic/4094-msp430g2553-1mhz-or-16mhz-how-to-set-it/
[100]: https://choosealicense.com/licenses/mit/
[101]: ./LICENSE.txt
[//]: # ([200]: https://github.com/Andy4495/MSP430LowPowerTempSensor)

[//]: # (Old TI product link that is no longer active: http://www.ti.com/tool/430BOOST-CC110L)
[//]: # (Mouser link that sometimes works: https://www.mouser.com/ProductDetail/Texas-Instruments/430BOOST-CC110L?qs=SKBAQrBh8xr4He0XWA6h2w%3D%3D )
