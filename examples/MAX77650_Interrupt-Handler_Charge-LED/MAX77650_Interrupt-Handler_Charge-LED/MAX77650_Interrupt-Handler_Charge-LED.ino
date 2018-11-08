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
  0.1.0       07.03.2018      initial code
  1.0.0       13.03.2018      bugfixes


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

#define MAX32620_red_LED P2_4
#define MAX32620_green_LED P2_5
#define MAX32620_blue_LED P2_6
#define MAX32620_boot_switch_pin P2_7
#define MAX32620_INT P2_3
#define MAX77650_PHLD P2_2

#define MAX77650_debug true

// Globals
int i = 0;
boolean PMIC_INT = false;
int UART_last_millies = 0;
int LED_last_millies = 0;
int MAX77650_GLBL_INT_REG = 0;
int MAX77650_CHG_INT_REG = 0;
boolean LED_setup = false;

void setup(void)
{
  //Configure the Power-Management (Power-Hold)
  pinMode(MAX77650_PHLD, OUTPUT);          //configure pin as output
  digitalWrite(MAX77650_PHLD, HIGH);       //set output to HIGH to hold the power-on state

  Serial.begin(115200);
  MAX77650_init();

  //Baseline Initialization following rules printed in MAX77650 Programmres Guide Chapter 4 Page 5
  if (MAX77650_debug) Serial.print("Set Main Bias to normal Mode: ");
    if (MAX77650_setSBIA_LPM(false)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");    //Set Main Bias to normal Mode
  if (MAX77650_debug) Serial.print("Set On/Off-Button to push-button-mode: ");
    if (MAX77650_setnEN_MODE(false)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");    //set on/off-button to push-button
  if (MAX77650_debug) Serial.print("Set nEN input debounce time to 30ms: ");
    if (MAX77650_setDBEN_nEN(true)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");    //set on/off-button to push-button
  if (MAX77650_debug) Serial.print("Comparing part-numbers: ");
    if (MAX77650_getDIDM() == PMIC_partnumber) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");     //checking partnumbers
  if (MAX77650_debug) Serial.print("Checking OTP options: ");
    if (MAX77650_getCID() != MAX77650_CID) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");     //checking OTP options
  //Values for NTC beta=3800K; Battery-values are for 1s 303759 with 600mAh
  if (MAX77650_debug) Serial.print("Set the VCOLD JEITA Temperature Threshold to 0°C: ");
    if (MAX77650_setTHM_COLD(2)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");   //0°C
  if (MAX77650_debug) Serial.print("Set the VCOOL JEITA Temperature Threshold to 15°C: ");
    if (MAX77650_setTHM_COOL(3)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");   //15°C
  if (MAX77650_debug) Serial.print("Set the VWARM JEITA Temperature Threshold to 45°C: ");
    if (MAX77650_setTHM_WARM(2)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");   //45°C
  if (MAX77650_debug) Serial.print("Set the VHOT JEITA Temperature Threshold to 60°C: ");
    if (MAX77650_setTHM_HOT(3)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");    //60°C
  if (MAX77650_debug) Serial.print("Set CHGIN regulation voltage to 4.00V: ");
    if (MAX77650_setVCHGIN_MIN(0)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed"); //
  if (MAX77650_debug) Serial.print("Set CHGIN Input Current Limit to 380mA: ");
    if (MAX77650_setICHGIN_LIM(3)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed"); //380mA
  if (MAX77650_debug) Serial.print("Set the prequalification charge current to 10%: ");
    if (MAX77650_setI_PQ(false)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");   //10%
  if (MAX77650_debug) Serial.print("Set Battery prequalification voltage threshold to 3.0V: ");
    if (MAX77650_setCHG_PQ(7)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");        //3.0V
  if (MAX77650_debug) Serial.print("Set Charger Termination Current to 15% of of fast charge current: ");
    if (MAX77650_setI_TERM(3)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");        //15%
  if (MAX77650_debug) Serial.print("Set Topoff timer value to 0 minutes: ");
    if (MAX77650_setT_TOPOFF(0)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");      //0 minutes
  if (MAX77650_debug) Serial.print("Set the die junction temperature regulation point to 60°C: ");
    if (MAX77650_setTJ_REG(0)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");        //60°C Tj
  if (MAX77650_debug) Serial.print("Set System voltage regulation to 4.50V: ");
    if (MAX77650_setVSYS_REG(0x10)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");   //Vsys=4.5V
  if (MAX77650_debug) Serial.print("Set the fast-charge constant current value to 300mA: ");
    if (MAX77650_setCHG_CC(0x3f)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");     //300mA
  if (MAX77650_debug) Serial.print("Set the fast-charge safety timer to 5h: ");
    if (MAX77650_setT_FAST_CHG(2)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");    //5h
  if (MAX77650_debug) Serial.print("Set IFAST-CHG_JEITA to 300mA: ");
    if (MAX77650_setCHG_CC_JEITA(0x3f)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed"); //300mA
  if (MAX77650_debug) Serial.print("Set Thermistor enable bit: ");
    if (MAX77650_setTHM_EN(false)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");     //disable the thermistor monitoring
  if (MAX77650_debug) Serial.print("Set fast-charge battery regulation voltage to 4.20V: ");
    if (MAX77650_setCHG_CV(0x18)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");     //4.20V
  if (MAX77650_debug) Serial.print("Set USB not in power down: ");
    if (MAX77650_setUSBS(false)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");      //USBS not suspended
  if (MAX77650_debug) Serial.print("Set the modified VFAST-CHG to 4.00V: ");
    if (MAX77650_setCHG_CV_JEITA(0x10)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed"); //4.0V
  if (MAX77650_debug) Serial.print("Selects the battery discharge current full-scale current value to 300mA: ");
    if (MAX77650_setIMON_DISCHG_SCALE(0x0A)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed"); //300mA
  if (MAX77650_debug) Serial.print("Disable the analog MUX output: ");
    if (MAX77650_setMUX_SEL(0)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");    //AMUX=off
  if (MAX77650_debug) Serial.print("Set the Charger to Enable: ");
    if (MAX77650_setCHG_EN(true)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");  //enable the Charger
  if (MAX77650_debug) Serial.print("Disable SIMO Buck-Boost Channel 0 Active-Discharge: ");
    if (MAX77650_setADE_SBB0(false)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");
  if (MAX77650_debug) Serial.print("Disable SIMO Buck-Boost Channel 1 Active-Discharge: ");
    if (MAX77650_setADE_SBB1(false)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");
  if (MAX77650_debug) Serial.print("Disable SIMO Buck-Boost Channel 2 Active-Discharge: ");
    if (MAX77650_setADE_SBB1(false)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");
  if (MAX77650_debug) Serial.print("Set SIMO Buck-Boost to maximum drive strength: ");
    if (MAX77650_setDRV_SBB(0b00)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");
  if (MAX77650_debug) Serial.print("Set SIMO Buck-Boost Channel 0 Peak Current Limit to 500mA: ");
    if (MAX77650_setIP_SBB0(0b00)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");
  if (MAX77650_debug) Serial.print("Set SIMO Buck-Boost Channel 1 Peak Current Limit to 500mA: ");
    if (MAX77650_setIP_SBB1(0b00)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");
  if (MAX77650_debug) Serial.print("Set SIMO Buck-Boost Channel 2 Peak Current Limit to 500mA: ");
    if (MAX77650_setIP_SBB2(0b00)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");
  if (MAX77650_debug) Serial.print("Set SIMO Buck-Boost Channel 2 to on while in stand-by-mode: ");
    if (MAX77650_setEN_SBB2(0b110)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");
  if (MAX77650_debug) Serial.print("Initialize Global Interrupt Mask Register: ");
    if (MAX77650_setINT_M_GLBL(0b00000000)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");
  if (MAX77650_debug) Serial.print("Initialize Charger Interrupt Mask Register: ");
    if (MAX77650_setINT_M_CHG(0b00000000)) if (MAX77650_debug) Serial.println("okay"); else if (MAX77650_debug) Serial.println("failed");

  //Initialization of PMIC-LEDs
  MAX77650_setLED_FS0(0b00);
  MAX77650_setINV_LED0(false);    //LED red: phase operation
  MAX77650_setBRT_LED0(0b11111);  //LED red: brightness
  MAX77650_setP_LED0(0b1111);     //LED red: LED period
  MAX77650_setD_LED0(0b1111);     //LED red: LED duty-cycle
  MAX77650_setLED_FS1(0b00);
  MAX77650_setINV_LED1(false);    //LED green: phase operation
  MAX77650_setBRT_LED1(0b11111);  //LED green: brightness
  MAX77650_setP_LED1(0b1111);     //LED green: LED period
  MAX77650_setD_LED1(0b1111);     //LED green: LED duty-cycle
  MAX77650_setLED_FS2(0b00);
  MAX77650_setINV_LED2(false);    //LED blue: phase operation
  MAX77650_setBRT_LED2(0b11111);  //LED blue: brightness
  MAX77650_setP_LED2(0b1111);     //LED blue: LED period
  MAX77650_setD_LED2(0b1111);     //LED blue: LED duty-cycle
  MAX77650_setEN_LED_MSTR(true);  //LEDs Master enable

  //setup GPIO
  pinMode(MAX32620_red_LED, OUTPUT);
  digitalWrite(MAX32620_red_LED, HIGH);
  pinMode(MAX32620_green_LED, OUTPUT);
  digitalWrite(MAX32620_green_LED, HIGH);
  pinMode(MAX32620_blue_LED, OUTPUT);
  digitalWrite(MAX32620_blue_LED, HIGH);
  pinMode(MAX32620_boot_switch_pin, INPUT_PULLUP);
  pinMode(MAX32620_INT, INPUT_PULLUP);

  //setup Interrupts
  attachInterrupt(MAX32620_INT, PMIC_IRQ_handler, FALLING);    //enable interrupt
  MAX77650_getINT_GLBL();  //read global interrupt register to clear it
  MAX77650_getINT_CHG();   //read charger interrupt register to clear it

  //run the PMIC LED set only once after start-up
  PMIC_INT = true;
  LED_setup = true;
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

float get_CHG_Input_Current() {
  //Measure the Charger Input Current
  MAX77650_setMUX_SEL(0b0010);
  delay(5);
  float retval = (analogRead(52) * 475 / 1024 / 1.041666);
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

float get_Batt_Voltage() {
  //Measure the Battery Voltage
  MAX77650_setMUX_SEL(0b0011);
  delay(5);
  float retval = (analogRead(52) * 4.6 / 1024 / 1.041666);
  MAX77650_setMUX_SEL(0b0000);
  return retval;
}

void PMIC_IRQ_handler(void) {
  PMIC_INT = true;
}

void loop(void)
{
  //provide heard-beat of the system using the red LED wired to the µC
  if (millis() >= LED_last_millies + 50) {
    digitalWrite(MAX32620_red_LED, HIGH);
  }
  if (millis() >= LED_last_millies + 950) {
    digitalWrite(MAX32620_red_LED, LOW);
    LED_last_millies = millis();
  }

  if ((millis() >= UART_last_millies + 3000) && (MAX77650_debug)) {
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

    UART_last_millies = millis();
  }

  if (PMIC_INT) {
    PMIC_INT = false;

    //handling the interrupts from the system
    MAX77650_GLBL_INT_REG = MAX77650_getINT_GLBL();  //read global interrupt register to clear it
    if (MAX77650_GLBL_INT_REG & 0b00000001) {        //GPI Falling Interrupt
      if (MAX77650_debug) Serial.println("GPI Falling Interrupt");
    }
    if (MAX77650_GLBL_INT_REG & 0b00000010) {        //GPI Rising Interrupt
      if (MAX77650_debug) Serial.println("GPI Rising Interrupt");
    }
    if (MAX77650_GLBL_INT_REG & 0b00000100) {        //nEN Falling Interrupt
      if (MAX77650_debug) Serial.println("nEN Falling Interrupt");
    }
    if (MAX77650_GLBL_INT_REG & 0b00001000) {        //nEN Rising Interrupt
      if (MAX77650_debug) Serial.println("nEN Rising Interrupt");
    }
    if (MAX77650_GLBL_INT_REG & 0b00010000) {        //Thermal Alarm 1 Rising Interrupt
      if (MAX77650_debug) Serial.println("Thermal Alarm 1 Rising Interrupt");
    }
    if (MAX77650_GLBL_INT_REG & 0b00100000) {        //Thermal Alarm 2 Rising Interrupt
      if (MAX77650_debug) Serial.println("Thermal Alarm 2 Rising Interrupt");
    }
    if (MAX77650_GLBL_INT_REG & 0b01000000) {        //LDO Dropout Detector Rising Interrupt
      if (MAX77650_debug) Serial.println("LDO Dropout Detector Rising Interrupt");
    }

    //handling the interrupts from the charger
    MAX77650_CHG_INT_REG = MAX77650_getINT_CHG();    //read charger interrupt register to clear it
    if (MAX77650_CHG_INT_REG & 0b00000001) {        //System voltage configuration error interrupt
      if (MAX77650_debug) Serial.println("System voltage configuration error interrupt");
    }
    if (MAX77650_CHG_INT_REG & 0b00000010) {        //Minimum System Voltage Regulation-loop related interrupt. This interrupt signals achange in the status bit VSYS_MIN_STAT.
      if (MAX77650_debug) Serial.println("Minimum System Voltage Regulation-loop related interrupt. This interrupt signals achange in the status bit VSYS_MIN_STAT.");
    }
    if (MAX77650_CHG_INT_REG & 0b00000100) {        //CHGIN control-loop related interrupt. This bit asserts when the input reaches currentlimit (ICHGIN-LIM) or VCHGIN falls below VCHGIN_MIN
      if (MAX77650_debug) Serial.println("CHGIN control-loop related interrupt. This bit asserts when the input reaches currentlimit (ICHGIN-LIM) or VCHGIN falls below VCHGIN_MIN");
    }
    if (MAX77650_CHG_INT_REG & 0b00001000) {        //Die junction temperature regulation interrupt. This bit asserts when the die temperature(TJ) exceeds TJ-REG. This interrupt signals a change in the status bit TJ_REG_STAT.
      if (MAX77650_debug) Serial.println("Die junction temperature regulation interrupt. This bit asserts when the die temperature(TJ) exceeds TJ-REG. This interrupt signals a change in the status bit TJ_REG_STAT.");
    }
    if (MAX77650_CHG_INT_REG & 0b00010000) {        //CHGIN control-loop related interrupt. This bit asserts when the input reaches currentlimit (ICHGIN-LIM) or VCHGIN falls below VCHGIN_MIN
      if (MAX77650_debug) Serial.println("CHGIN control-loop related interrupt. This bit asserts when the input reaches currentlimit (ICHGIN-LIM) or VCHGIN falls below VCHGIN_MIN");
    }
    if (MAX77650_CHG_INT_REG & 0b00100000) {        //Minimum System Voltage Regulation-loop related interrupt. This interrupt signals a
      if (MAX77650_debug) Serial.println("Minimum System Voltage Regulation-loop related interrupt. This interrupt signals a");
    }
    if (MAX77650_CHG_INT_REG & 0b01000000) {        //System voltage configuration error interrupt
      if (MAX77650_debug) Serial.println("System voltage configuration error interrupt");
    }

    if ((MAX77650_CHG_INT_REG) && 0b00000010 || LED_setup) {      //only execute if bit 1 in charger interrupt register is set: CHG_DTLS have changed
      LED_setup = false;
      //per default alle LEDs ausschalten
      MAX77650_setLED_FS0(0);
      MAX77650_setLED_FS1(0);
      MAX77650_setLED_FS2(0);
      switch (MAX77650_getCHG_DTLS())  {
        case 0x0: //Charger is off
          if (MAX77650_debug) Serial.println("Charger is off");
          break;
        case 0x1: //Charger is in prequalification mode
          MAX77650_setLED_FS0(0x1);
          if (MAX77650_debug) Serial.println("Charger is in prequalification mode");
          break;
        case 0x2: //Charger is in fast-charge constant-current (CC) mode
          MAX77650_setLED_FS0(0x1);
          if (MAX77650_debug) Serial.println("Charger is in fast-charge constant-current (CC) mode");
          break;
        case 0x3: //Charger is in JEITA modified fast-charge constant-current mode
          MAX77650_setLED_FS0(0x1);
          if (MAX77650_debug) Serial.println("Charger is in JEITA modified fast-charge constant-current mode");
          break;
        case 0x4: //Charger is in fast-charge constant-voltage (CV) mode
          MAX77650_setLED_FS0(0x1);
          if (MAX77650_debug) Serial.println("Charger is in fast-charge constant-voltage (CV) mode");
          break;
        case 0x5: //Charger is in JEITA modified fast-charge constant-voltage mode
          MAX77650_setLED_FS0(0x1);
          if (MAX77650_debug) Serial.println("Charger is in JEITA modified fast-charge constant-voltage mode");
          break;
        case 0x6: //Charger is in  top-off mode
          MAX77650_setLED_FS0(0x1);
          if (MAX77650_debug) Serial.println("Charger is in  top-off mode");
          break;
        case 0x7: //Charger is in JEITA modified top-off mode
          MAX77650_setLED_FS0(0x1);
          if (MAX77650_debug) Serial.println("Charger is in JEITA modified top-off mode");
          break;
        case 0x8: //Charger is done
          MAX77650_setLED_FS1(0x1);
          if (MAX77650_debug) Serial.println("Charger is done");
          break;
        case 0x9: //Charger is JEITA modified done (done was entered through the JEITAmodified fast-charge states)
          MAX77650_setLED_FS1(0x1);
          if (MAX77650_debug) Serial.println("Charger is JEITA modified done (done was entered through the JEITAmodified fast-charge states)");
          break;
        case 0xA: //Charger is suspended due to a prequalification timer fault
          if (MAX77650_debug) Serial.println("Charger is suspended due to a prequalification timer fault");
          break;
        case 0xB: //Charger is suspended due to a fast-charge timer fault
          if (MAX77650_debug) Serial.println("Charger is suspended due to a fast-charge timer fault");
          break;
        case 0xC: //Charger is suspended due to a battery temperature fault
          if (MAX77650_debug) Serial.println("Charger is suspended due to a battery temperature fault");
          break;
        default:
          break;
      }
    }
  }
}
