# MAX77650-Arduino-Library

This project provides an Arduino Library for the Maxim MAX77650 PMIC.

The MAX77650/MAX77651 provide highly-integrated battery charging and power supply solutions for low-power wearable applications where size and efficiency are critical. Both devices feature a SIMO buck-boost regulator that provides three independently programmable power rails from a single inductor to minimize total solution size. A 150mA LDO provides ripple rejection for audio and other noise-sensitive applications. A highly configurable linear charger supports a wide range of Li+ battery capacities and includes battery temperature monitoring for additional safety (JEITA).
The devices include other features such as current sinks for driving LED indicators and an analog multiplexer that switches several internal voltage and current signals to an external node for monitoring with an external ADC. A bidirectional I2C interface allows for configuring and checking the status of the devices. An internal on/off controller provides a controlled startup sequence for the regulators and provides supervisory functionality when the devices are on. Numerous factory programmable options allow the device to be tailored for many applications, enabling faster time to market.

## Getting Started

To get started just clone the project into your library folder, add the library and you can use the functions provided. See also "Installing" for further details. More ressources can be found here:
* https://www.maximintegrated.com/en/products/power/battery-management/MAX77650.html
* https://www.maximintegrated.com/en/app-notes/index.mvp/id/6428

The provided examples were writen and tested to suit the MAX32620FTHR board (https://www.maximintegrated.com/en/products/microcontrollers/MAX32620FTHR.html) but should work on any other board using the MAX77650.

### Prerequisites

- installed Adruino (version 1.8.5 or newer)

### Installing

Step by step installation for Arduino:

1. Clone the GitHub repo into your library-folder (usually C:\Users\<user>\Documents\Arduino\libraries for Windows systems)
2. Insert the library into your sketch using #include <MAX77650-Arduino-Library.h>
3. You're ready to use the PMIC functions
4. For a how-to just have a look into the examples; they're small and cover only one piece of the PMIC

## Versioning

We use [SemVer](http://semver.org/) for versioning.

## Authors

* **Ole Dreessen** - *Initial work* - [amperpirat](https://github.com/amperpirat)

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE.md](LICENSE.md) file for details
