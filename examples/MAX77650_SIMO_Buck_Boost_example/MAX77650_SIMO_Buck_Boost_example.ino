/**********************************************************************
  Copyright (C) 2017 Maxim Integrated Products, Inc., All Rights Reserved.

  Permission is hereby granted, free of charge, to any person obtaining a
  copy of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
  OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
  OTHER DEALINGS IN THE SOFTWARE.

  Except as contained in this notice, the name of Maxim Integrated
  Products, Inc. shall not be used except as stated in the Maxim Integrated
  Products, Inc. Branding Policy.

  The mere transfer of this software does not imply any licenses
  of trade secrets, proprietary technology, copyrights, patents,
  trademarks, maskwork rights, or any other form of intellectual
  property whatsoever. Maxim Integrated Products, Inc. retains all
  ownership rights.

  Authors:
  Ole Dreessen; ole.dreessen@maximintegrated.com

  Revisions:
  1.0.0       21.03.2018      initial code


  The MAX77650/MAX77651 provide highly-integrated battery charging and
  power supply solutions for low-power wearable applications where size and efficiency
  are critical. Both devices feature a SIMO buck-boost regulator that provides three
  independently programmable power rails from a single inductor to minimize total solution
  size. A 150mA LDO provides ripple rejection for audio and other noise-sensitive applications.
  A highly configurable linear charger supports a wide range of Li+ battery capacities and
  includes battery temperature monitoring for additional safety (JEITA).

  The devices include other features such as current sinks for driving LED indicators and
  an analog multiplexer that switches several internal voltage and current signals to an
  external node for monitoring with an external ADC. A bidirectional I2C interface allows
  for configuring and checking the status of the devices. An internal on/off controller
  provides a controlled startup sequence for the regulators and provides supervisory
  functionality when the devices are on. Numerous factory programmable options allow the
  device to be tailored for many applications, enabling faster time to market.

  Ressources can be found at
  https://www.maximintegrated.com/en/products/power/battery-management/MAX77650.html
  https://www.maximintegrated.com/en/app-notes/index.mvp/id/6428
**********************************************************************/

// Includes
#include <Wire.h>
#include <MAX77650-Arduino-Library.h>

//Definitions
#define MAX77650_debug true
#define MAX77650_PHLD P2_2   //Pin 18 -> connected to MAX77650 power hold input pin (A1)

// Globals
int i = 0;

void setup(void)
{
  //Configure the Power-Management (Power-Hold)
  pinMode(MAX77650_PHLD, OUTPUT);          //configure pin as output
  digitalWrite(MAX77650_PHLD, HIGH);       //set output to HIGH to hold the power-on state

  Serial.begin(115200);
  MAX77650_init();

  //SIMO Buck-Boost Initialization - MAX77650 Programmers Guide Chapter 9.4.2
  //
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //Use extreme caution when changing these values! Wrong Values can cause higher output voltages than your connected devices can handle!!!
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //
  MAX77650_setIP_SBB0(0b11);  //Limit output of SBB0 to 500mA
  MAX77650_setTV_SBB0(0b101000); //Set output Voltage of SBB0 to 1.8V
  MAX77650_setADE_SBB0(0b0); //Disable Active Discharge at SBB0 Output
  MAX77650_setEN_SBB0(0b110); //Enable SBB0 is on irrespective of FPS whenever the on/off controller is in its "On via Software" or "On via On/Off Controller" states

  MAX77650_setIP_SBB1(0b11);  //Limit output of SBB1 to 500mA
  MAX77650_setTV_SBB1(0b100000); //Set output Voltage of SBB1 to 1.2V
  MAX77650_setADE_SBB1(0b0); //Disable Active Discharge at SBB1 Output
  MAX77650_setEN_SBB1(0b110); //Enable SBB1 is on irrespective of FPS whenever the on/off controller is in its "On via Software" or "On via On/Off Controller" states

  MAX77650_setIP_SBB2(0b11);  //Limit output of SBB2 to 500mA
  MAX77650_setTV_SBB2(0b110010); //Set output Voltage of SBB2 to 3.3V
  MAX77650_setADE_SBB2(0b0); //Disable Active Discharge at SBB2 Output
  MAX77650_setEN_SBB2(0b110); //Enable SBB2 is on irrespective of FPS whenever the on/off controller is in its "On via Software" or "On via On/Off Controller" states

  //Initialization of PMIC-LEDs
  MAX77650_setLED_FS0(0b01);
  MAX77650_setINV_LED0(false);    //LED red: phase operation
  MAX77650_setBRT_LED0(0b00000);  //LED red: brightness
  MAX77650_setP_LED0(0b1111);     //LED red: LED period
  MAX77650_setD_LED0(0b1111);     //LED red: LED duty-cycle
  MAX77650_setLED_FS1(0b01);
  MAX77650_setINV_LED1(false);    //LED green: phase operation
  MAX77650_setBRT_LED1(0b00000);  //LED green: brightness
  MAX77650_setP_LED1(0b1111);     //LED green: LED period
  MAX77650_setD_LED1(0b1111);     //LED green: LED duty-cycle
  MAX77650_setLED_FS2(0b01);
  MAX77650_setINV_LED2(false);    //LED blue: phase operation
  MAX77650_setBRT_LED2(0b00000);  //LED blue: brightness
  MAX77650_setP_LED2(0b1111);     //LED blue: LED period
  MAX77650_setD_LED2(0b1111);     //LED blue: LED duty-cycle
  MAX77650_setEN_LED_MSTR(true);  //LEDs Master enable
}

void loop(void)
{
  for (i = 0; i < 33; i++) {
    MAX77650_setBRT_LED0(random(0, 32));
    MAX77650_setBRT_LED1(random(0, 32));
    MAX77650_setBRT_LED2(random(0, 32));
    delay(50);
  }
  Serial.println("");
  Serial.print("SIMO Buck-Boost Voltage Channel 0: ");
  Serial.print(MAX77650_getTV_SBB0() * 0.025 + 0.8);
  Serial.println("V");

  //Read SBB1 voltage is more complex. See MAX77650 programmers manual page 40 for more details.
  Serial.print("SIMO Buck-Boost Voltage Channel 1: ");
  if (MAX77650_getDIDM()) {
    float SBB1_voltage = 0;
    float lsb = 0.05;
    float base_0 = 2.40;
    float base_1 = 3.20;
    float base_2 = 4.00;
    float base_3 = 4.80;
    float code_1_0 = 0;
    byte code = MAX77650_getTV_SBB1();
    code_1_0 = code & 0b11;
    if (code_1_0 == 0b00)
      SBB1_voltage = base_0;
    else if (code_1_0 == 0b01)
      SBB1_voltage = base_1;
    else if (code_1_0 == 0b10)
      SBB1_voltage = base_2;
    else
      SBB1_voltage = base_3;
    float code_5_2 = (code >> 2);
    SBB1_voltage = SBB1_voltage + code_5_2 * lsb;
    Serial.print(SBB1_voltage);
  } else 
     Serial.print(MAX77650_getTV_SBB1() * 0.0125 + 0.8);
  
  Serial.println("V");
  Serial.print("SIMO Buck-Boost Voltage Channel 2: ");
  if (!(MAX77650_getDIDM())) Serial.print(MAX77650_getTV_SBB2() * 0.05 + 0.8);
  else Serial.print(MAX77650_getTV_SBB2() * 0.05 + 2.4); //See MAX77650 programmers manual page 43 chapter 9.4.9
  Serial.println("V");
}


