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

void setup(void)
{
  //Configure the Power-Management (Power-Hold)
  pinMode(MAX77650_PHLD, OUTPUT);          //configure pin as output
  digitalWrite(MAX77650_PHLD, HIGH);       //set output to HIGH to hold the power-on state

  Serial.begin(115200);
  MAX77650_init();

  //Baseline Initialization following rules printed in MAX77650 Programmres Guide Chapter 4 Page 5
  MAX77650_setSBIA_LPM(false);    //Set Main Bias to normal Mode
  MAX77650_setnEN_MODE(false);    //set on/off-button to push-button
  MAX77650_setDBEN_nEN(true);    //set on/off-button to push-button
  //Values for NTC beta=3800K; Battery-values are for 1s 303759 with 600mAh
  MAX77650_setTHM_COLD(2);   //0°C
  MAX77650_setTHM_COOL(3);   //15°C
  MAX77650_setTHM_WARM(2);   //45°C
  MAX77650_setTHM_HOT(3);    //60°C
  MAX77650_setVCHGIN_MIN(0b101); //
  MAX77650_setICHGIN_LIM(0b0); //475mA (ICHGIN_LIM_DEF=1 set by OTP)
  MAX77650_setI_PQ(false);   //10%
  MAX77650_setCHG_PQ(7);        //3.0V
  MAX77650_setI_TERM(3);        //15%
  MAX77650_setT_TOPOFF(0);      //0 minutes
  MAX77650_setTJ_REG(0);        //60°C Tj
  MAX77650_setVSYS_REG(0x10);   //Vsys=4.5V
  MAX77650_setCHG_CC(0b111111);     //300mA
  MAX77650_setT_FAST_CHG(1);    //3h
  MAX77650_setCHG_CC_JEITA(0x3f); //300mA

  MAX77650_setTHM_EN(true);     //enable the thermistor monitoring
  MAX77650_setCHG_CV(0x18);     //4.20V
  MAX77650_setUSBS(false);      //USBS not suspended
  MAX77650_setCHG_CV_JEITA(0x10); //4.0V
  MAX77650_setIMON_DISCHG_SCALE(0x0A); //300mA
  MAX77650_setMUX_SEL(0);    //AMUX=off
  MAX77650_setCHG_EN(true);  //enable the Charger
  MAX77650_setADE_SBB0(false);
  MAX77650_setADE_SBB1(false);
  MAX77650_setADE_SBB1(false);
  MAX77650_setDRV_SBB(0b00);
  MAX77650_setIP_SBB0(0b00);
  MAX77650_setIP_SBB1(0b00);
  MAX77650_setIP_SBB2(0b00);
  MAX77650_setEN_SBB2(0b110);
  MAX77650_setINT_M_GLBL(0x0);
  MAX77650_setINT_M_CHG(0x0);

  //Switch off the analog multiplexer
  MAX77650_setMUX_SEL(0b0000);

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

float get_CHG_Input_Voltage() {
  //Measure the Charger Input Voltage
  MAX77650_setMUX_SEL(0b0001);
  delay(5);
  float retval = (analogRead(52) * 7.5 / 1024 / 1.041666); //1.041666 is the correction value from 1.25V Vamux fullscale (MAX77650datasheet page 62) to 1.2V internal ADC reference of the MAX32620FTHR. Please change to your settings!
  //Set the multiplexer back to off to save energy
  MAX77650_setMUX_SEL(0b0000);
  return retval;
}

float get_CHG_Input_Current() {
  //Measure the Charger Input Current
  MAX77650_setMUX_SEL(0b0010);
  delay(5);
  float retval = (analogRead(52) * 475 / 1024 / 1.041666);
  MAX77650_setMUX_SEL(0b0000);
  return retval;
}

float get_Batt_Voltage() {
  //Measure the Battery Voltage
  MAX77650_setMUX_SEL(0b0011);
  delay(5);
  float retval = (analogRead(52) * 4.6 / 1024 / 1.041666);
  MAX77650_setMUX_SEL(0b0000);
  return retval;
}

float get_Batt_Charge_Current() {
  float retval = 0.00;
  if ((MAX77650_getCHG()) || ((MAX77650_getCHG_DTLS() > 0) && (MAX77650_getCHG_DTLS() < 8)) || (MAX77650_getCHGIN_DTLS() == 0b11)) {
    MAX77650_setMUX_SEL(0b0100);
    delay(5);
    int chg = MAX77650_getCHG_CC();
    if (chg >= 0x27) chg = 0x27;  //0x27 reflects 300mA (highest I_charge); see MAX77650 programmers manual chapter 9.2.11
    retval = analogRead(52) * ( chg * 7.5 + 7.5) / 1024 / 1.041666;   //formula regarding MAX77650 programmers manual page 25 CHG_CC;
  }
  MAX77650_setMUX_SEL(0b0000);
  return retval;
}

float get_Batt_Discharge_Current() {
  //Measure the Battery Discharge-Current
  MAX77650_setMUX_SEL(0b0110);
  delay(5);
  float v_null = analogRead(52);
  MAX77650_setMUX_SEL(0b0101);
  delay(5);
  int v_amux = analogRead(52);
  int dischg[] = {8.2, 40.5, 72.3, 103.4, 134.1, 164.1, 193.7, 222.7, 251.2, 279.3, 300.0, 300.0, 300.0, 300.0, 300.0, 300.0};  //see MAX77650 programmers manual chapter 9.2.17
  return (((v_amux - v_null) / (1024 - v_null)) * (dischg[MAX77650_getIMON_DISCHG_SCALE()]) / 1.041666); //formula regarding MAX77650 programmers manual page 31 IMON_DISCHG_SCALE
}

void loop(void)
{
  MAX77650_setBRT_LED0(31);
  MAX77650_setBRT_LED1(31);
  MAX77650_setBRT_LED2(31);
  delay(700);

  Serial.print("Charger Input Voltage: ");
  Serial.print(get_CHG_Input_Voltage());
  Serial.println("V");

  Serial.print("Charger Input Current: ");
  Serial.print(get_CHG_Input_Current());
  Serial.println("mA");

  Serial.print("Battery Voltage: ");
  Serial.print(get_Batt_Voltage());
  Serial.println("V");

  Serial.print("Battery Charge Current: ");
  Serial.print(get_Batt_Charge_Current());
  Serial.println("mA");

  Serial.print("Battery Discharge Current: ");
  Serial.print(get_Batt_Discharge_Current());
  Serial.println("mA");
}


