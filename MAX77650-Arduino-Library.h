/**********************************************************************
* Copyright (C) 2017 Maxim Integrated Products, Inc., All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
* 
* Authors: 
* Ole Dreessen; ole.dreessen@maximintegrated.com
* 
* Revisions:
* 0.1.0       21.11.2017      initial code
* 0.2.0       27.11.2017      added functions
* 0.3.0       07.03.2018      bugfixes
* 0.4.0       14.03.2018      bugfixes: Interrupt registers are RC (read-clear) and cannot be read bit-by-bit; this is valid for register 0x00, 0x01 and 0x04; corrected wrong register-address in function read CID; cleaned init-function (deleted baseline-init - see examples)
* 1.0.0	   14.03.2018	     code-review and finished work
* 1.0.1	   22.03.2018      bugfix: fixed address of register in function MAX77650_getCID; added two more examples
* 
**********************************************************************/

#ifndef MAX77650_H
#define MAX77650_H

#include "Arduino.h"
#include <Wire.h>

/**********************************************************************
*@brief MAX77650 - The MAX77650/MAX77651 provide highly-integrated battery charging and 
*power supply solutions for low-power wearable applications where size and efficiency 
*are critical. Both devices feature a SIMO buck-boost regulator that provides three 
*independently programmable power rails from a single inductor to minimize total solution 
*size. A 150mA LDO provides ripple rejection for audio and other noise-sensitive applications. 
*A highly configurable linear charger supports a wide range of Li+ battery capacities and 
*includes battery temperature monitoring for additional safety (JEITA).
*
*The devices include other features such as current sinks for driving LED indicators and 
*an analog multiplexer that switches several internal voltage and current signals to an 
*external node for monitoring with an external ADC. A bidirectional I2C interface allows 
*for configuring and checking the status of the devices. An internal on/off controller 
*provides a controlled startup sequence for the regulators and provides supervisory 
*functionality when the devices are on. Numerous factory programmable options allow the 
*device to be tailored for many applications, enabling faster time to market.
*
*Ressources can be found at
*https://www.maximintegrated.com/en/products/power/battery-management/MAX77650.html
*https://www.maximintegrated.com/en/app-notes/index.mvp/id/6428
**********************************************************************/


// Project specific definitions *** adapt to your requirements! ***
#define PMIC_partnumber 0x00  //placed PMIC part-numer on the board: MAX77650 or MAX77651
#define MAX77650_CID 0x78     //OTP option
#define MAX77650_ADDR 0x48    //alternate ADDR is 0x40
#define MAX77650_debug false
#define MAX77650_I2C_port 2     //I2C port of the host µC

//***** Begin MAX77650 Register Definitions *****
#define MAX77650_CNFG_GLBL_ADDR 0x10     //Global Configuration Register; Reset Value OTP                           
#define MAX77650_INT_GLBL_ADDR 0x00      //Interrupt; Reset Value 0x00                                              
#define MAX77650_INTM_GLBL_ADDR 0x06     //Interrupt Mask; Reset Value 0xFF                                         
#define MAX77650_STAT_GLBL_ADDR 0x05     //Global Status Register; Reset Value OTP                                  
#define MAX77650_ERCFLAG_ADDR 0x04       //Flags; Reset Value 0x00                                                  
#define MAX77650_CNFG_GPIO_ADDR 0x12     //GPIO Configuration Register; Reset Value 0x01                            
#define MAX77650_CID_ADDR 0x11           //Chip ID Register; Reset Value OTP; Read only                             
#define MAX77650_INT_CHG_ADDR 0x01       //Charger Interrupt Register; Reset Value 0x00; RC                         
#define MAX77650_INT_M_CHG_ADDR 0x07     //Charger Interrupt Mask Register; Reset Value 0xFF; Read/Write            
#define MAX77650_STAT_CHG_A_ADDR 0x02    //Charger Status Register A; Reset Value 0x00; Read only                   
#define MAX77650_STAT_CHG_B_ADDR 0x03    //Charger Status Register B; Reset Value 0x00; Read only                  
#define MAX77650_CNFG_CHG_A_ADDR 0x018   //Charger Configuration Register A; Reset Value 0x0F; Read/Write          
#define MAX77650_CNFG_CHG_B_ADDR 0x019   //Charger Configuration Register B; Reset Value OTP; Read/Write            
#define MAX77650_CNFG_CHG_C_ADDR 0x01A   //Charger Configuration Register C; Reset Value 0xF8; Read/Write           
#define MAX77650_CNFG_CHG_D_ADDR 0x01B   //Charger Configuration Register D; Reset Value 0x10; Read/Write           
#define MAX77650_CNFG_CHG_E_ADDR 0x01C   //Charger Configuration Register E; Reset Value 0x05; Read/Write           
#define MAX77650_CNFG_CHG_F_ADDR 0x01D   //Charger Configuration Register F; Reset Value 0x04; Read/Write           
#define MAX77650_CNFG_CHG_G_ADDR 0x01E   //Charger Configuration Register G; Reset Value 0x00; Read/Write           
#define MAX77650_CNFG_CHG_H_ADDR 0x01F   //Charger Configuration Register H; Reset Value 0x00; Read/Write           
#define MAX77650_CNFG_CHG_I_ADDR 0x020   //Charger Configuration Register I; Reset Value 0xF0; Read/Write           
#define MAX77650_CNFG_LDO_A_ADDR 0x38    //LDO Configuration Register A; Reset Value OTP; Read/Write                
#define MAX77650_CNFG_LDO_B_ADDR 0x39    //LDO Configuration Register B; Reset Value OTP; Read/Write                
#define MAX77650_CNFG_SBB_TOP_ADDR 0x28  //SIMO Buck-Boost Configuration Register; Reset Value OTP; Read/Write      
#define MAX77650_CNFG_SBB0_A_ADDR 0x29   //SIMO Buck-Boost 0 Configuration Register A; Reset Value OTP; Read/Write  
#define MAX77650_CNFG_SBB0_B_ADDR 0x2A   //SIMO Buck-Boost 0 Configuration Register B; Reset Value OTP; Read/Write  
#define MAX77650_CNFG_SBB1_A_ADDR 0x2B   //SIMO Buck-Boost 1 Configuration Register A; Reset Value OTP; Read/Write  
#define MAX77650_CNFG_SBB1_B_ADDR 0x2C   //SIMO Buck-Boost 1 Configuration Register B; Reset Value OTP; Read/Write  
#define MAX77650_CNFG_SBB2_A_ADDR 0x2D   //SIMO Buck-Boost 2 Configuration Register A; Reset Value OTP; Read/Write  
#define MAX77650_CNFG_SBB2_B_ADDR 0x2E   //SIMO Buck-Boost 2 Configuration Register B; Reset Value OTP; Read/Write  
#define MAX77650_CNFG_LED0_A_ADDR 0x40   //LED 0 Configuration Register A; Reset Value 0x04; Read/Write             
#define MAX77650_CNFG_LED0_B_ADDR 0x43   //LED 0 Configuration Register B; Reset Value 0x0F; Read/Write             
#define MAX77650_CNFG_LED1_A_ADDR 0x41   //LED 1 Configuration Register A; Reset Value 0x04; Read/Write            
#define MAX77650_CNFG_LED1_B_ADDR 0x44   //LED 1 Configuration Register B; Reset Value 0x0F; Read/Write             
#define MAX77650_CNFG_LED2_A_ADDR 0x42   //LED 2 Configuration Register A; Reset Value 0x04; Read/Write             
#define MAX77650_CNFG_LED2_B_ADDR 0x45   //LED 2 Configuration Register A; Reset Value 0x0F; Read/Write             
#define MAX77650_CNFG_LED_TOP_ADDR 0x46  //LED Configuration Register; Reset Value 0x0D; Read/Write
//***** End MAX77650 Register Definitions *****

boolean MAX77650_init(void);
int MAX77650_read_register(int);
byte MAX77650_write_register(int, int);

//Status Register (STAT_GLBL) related reads
boolean MAX77650_getDIDM(void); //Returns Part Number; Return Value: 1=MAX77650; 2=MAX77651; -1=error reading DeviceID
boolean MAX77650_getLDO_DropoutDetector(void); //Returns whether the LDO is in Dropout or not; Return Value: 0=LDO is not in dropout; 1=LDO is in dropout
boolean MAX77650_getThermalAlarm1(void); //Returns Thermal Alarm (TJA1 limit); Return Value: 0=Tj<TJA1; 1=Tj>TJA1
boolean MAX77650_getThermalAlarm2(void); //Returns Therma2 Alarm (TJA2 limit); Return Value: 0=Tj<TJA2; 1=Tj>TJA2
boolean MAX77650_getDebounceStatusnEN0(void); //Returns Debounced Status of nEN input; Return Value: 0=nEN0 is not active; 1=nEN0 is active
boolean MAX77650_getDebounceStatusPWR_HLD(void); //Returns Debounced Status of PWR_HLD input; Return Value: 0=logic low; 1=logic high

//Flag Register (ERCFLAG) related reads
byte MAX77650_getERCFLAG(void); //Returns the ERCFLAG Register 0x04

//Register reads related to ChipID
byte MAX77650_getChipID(void); // Return Value: 1=MAX77650; 2=MAX77651; -1=error reading ChipID

//CID Register
byte MAX77650_getCID(void); // Returns the OTP programmed Chip Identification Code

//Charger Status Register (STAT_CHG_A and B) related reads
boolean MAX77650_getVCHGIN_MIN_STAT(void); //Returns Minimum Input Voltage Regulation Loop Status; Return Value: 0=no event; 1=The minimum CHGIN voltage regulation loop has engaged to regulate VCHGIN >= VCHGIN-MIN
boolean MAX77650_getICHGIN_LIM_STAT(void); //Returns Input Current Limit Loop Status; Return Value: 0=no event; 1=The CHGIN current limit loop has engaged to regulate ICHGIN <= ICHGIN-LIM
boolean MAX77650_getVSYS_MIN_STAT(void); //Returns Minimum System Voltage Regulation Loop Status; Return Value: 0=no event; 1=The minimum system voltage regulation loop is engaged to regulate VSYS >= VSYS-MIN
boolean MAX77650_getTJ_REG_STAT(void); //Returns Maximum Junction Temperature Regulation Loop Status; Return Value: 0=no event; 1=The maximum junction temperature regulation loop has engaged to regulate the junction temperature less than TJ-REG
byte MAX77650_getTHM_DTLS(void); //Returns Battery Temperature Details; Return Value: Battery Temperature Details
byte MAX77650_getCHG_DTLS(void); //Returns Charger Details
byte MAX77650_getCHGIN_DTLS(void); //Returns Charger Input Status Details
boolean MAX77650_getCHG(void); //Returns Quick Charger Status
boolean MAX77650_getTIME_SUS(void); //Returns Time Suspend Indicator

//LED related registers
boolean MAX77650_getCLK_64_S(void); //Returns 64Hz Clock Status
boolean MAX77650_getEN_LED_MSTR(void); //Returns Master LED Enable Bit
byte MAX77650_getLED_FS0(void); //Returns LED0 Full Scale Range
boolean MAX77650_getINV_LED0(void); //Returns LED0 Invert
byte MAX77650_getBRT_LED0(void); //Returns LED0 Brightness Control
byte MAX77650_getP_LED0(void); //Returns LED0 Period Settings
byte MAX77650_getD_LED0(void); //Returns LED0 On Duty-Cycle Settings
byte MAX77650_getLED_FS1(void); //Returns LED1 Full Scale Range
boolean MAX77650_getINV_LED1(void); //Returns LED1 Invert
byte MAX77650_getBRT_LED1(void); //Returns LED1 Brightness Control
byte MAX77650_getP_LED1(void); //Returns LED1 Period Settings
byte MAX77650_getD_LED1(void); //Returns LED1 On Duty-Cycle Settings
byte MAX77650_getLED_FS2(void); //Returns LED2 Full Scale Range
boolean MAX77650_getINV_LED2(void); //Returns LED2 Invert
byte MAX77650_getBRT_LED2(void); //Returns LED2 Brightness Control
byte MAX77650_getP_LED2(void); //Returns LED2 Period Settings
byte MAX77650_getD_LED2(void); //Returns LED2 On Duty-Cycle Settings

//Global Configuration register 0x10
boolean MAX77650_getBOK(void); //Returns Main Bias Okay Status Bit
boolean MAX77650_getSBIA_LPM(void); //Returns Main Bias Low-Power Mode software request
boolean MAX77650_getSBIA_EN(void); //Returns Main Bias Enable Software Request
boolean MAX77650_getnEN_MODE(void); //Returns nEN Input (ONKEY) Default Configuration Mode
boolean MAX77650_getDBEN_nEN(void); //Returns Debounce Timer Enable for the nEN Pin
byte MAX77650_getSFT_RST(void); //Returns Software Reset Functions.

//Global Interrupt Status Register 0x00
byte MAX77650_getINT_GLBL(void); //Returns Interrupt Status Register 0x00

//Global Interrupt Mask Register 0x06
byte MAX77650_getINT_M_GLBL(void); //Returns Global Interrupt Mask Register

//GPIO Configuration Register
boolean MAX77650_getDBEN_GPI(void); //Returns General Purpose Input Debounce Timer Enable
boolean MAX77650_getDO(void); //Returns General Purpose Output Data Output
boolean MAX77650_getDRV(void); //Returns General Purpose Output Driver Type
boolean MAX77650_getDI(void); //Returns GPIO Digital Input Value
boolean MAX77650_getDIR(void); //Returns GPIO Direction

//Charger Interrupt Status Register 0x01
byte MAX77650_getINT_CHG(void); //Returns Charger Interrupt Status Register 0x01

//Charger Interrupt Mask Register 0x07
byte MAX77650_getINT_M_CHG(void); //Returns Global Interrupt Mask Register

//Charger Configuration Register A 0x18
byte MAX77650_getTHM_HOT(void); //Returns the VHOT JEITA Temperature Threshold
byte MAX77650_getTHM_WARM(void); //Returns the VWARM JEITA Temperature Threshold
byte MAX77650_getTHM_COOL(void); //Returns the VCOOL JEITA Temperature Threshold
byte MAX77650_getTHM_COLD(void); //Returns the VCOLD JEITA Temperature Threshold

//Charger Configuration Register B 0x19
byte MAX77650_getVCHGIN_MIN(void); //Returns the Minimum CHGIN regulation voltage (VCHGIN-MIN)
byte MAX77650_getICHGIN_LIM(void); //Returns the CHGIN Input Current Limit (ICHGIN-LIM)
boolean MAX77650_getI_PQ(void); //Returns the prequalification charge current (IPQ) as a percentage of IFAST-CHG
boolean MAX77650_getCHG_EN(void); //Returns Charger Enable

//Charger Configuration Register C 0x1A
byte MAX77650_getCHG_PQ(void); //Returns the Battery prequalification voltage threshold (VPQ)
byte MAX77650_getI_TERM(void); //Returns the Charger Termination Current (ITERM). I_TERM[1:0] sets the charger termination current as a percentage of the fast charge current IFAST-CHG.
byte MAX77650_getT_TOPOFF(void); //Returns the Topoff timer value (tTO)

//Charger Configuration Register D 0x1B
byte MAX77650_getTJ_REG(void); //Returns the die junction temperature regulation point, TJ-REG
byte MAX77650_getVSYS_REG(void); //Returns the System voltage regulation (VSYS-REG)

//Charger Configuration Register E 0x1C
byte MAX77650_getCHG_CC(void); //Returns the fast-charge constant current value, IFAST-CHG.
byte MAX77650_getT_FAST_CHG(void); //Returns the fast-charge safety timer, tFC.

//Charger Configuration Register F 0x1D
byte MAX77650_getCHG_CC_JEITA(void); //Returns the IFAST-CHG_JEITA for when the battery is either cool or warm as defined by the TCOOL and TWARM temperature thresholds. This register is a don't care if the battery temperature is normal.
byte MAX77650_getTHM_EN(void); //Returns the Thermistor enable bit

//Charger Configuration Register G 0x1E
byte MAX77650_getCHG_CV(void); //Returns the fast-charge battery regulation voltage, VFAST-CHG.
boolean MAX77650_getUSBS(void); //this bit places CHGIN in USB suspend mode

//Charger Configuration Register H 0x1F
byte MAX77650_getCHG_CV_JEITA(void); //Returns the modified VFAST-CHG for when the battery is either cool or warm as defined by the TCOOL and TWARM temperature thresholds. This register is a don't care if the battery temperature is normal.

//Charger Configuration Register I 0x20
byte MAX77650_getIMON_DISCHG_SCALE(void); //Returns the Selects the battery discharge current full-scale current value This 4-bit configuration starts at 7.5mA and ends at 300mA.
byte MAX77650_getMUX_SEL(void); //Returns the analog channel to connect to AMUX configuration

//LDO Configuration Register A 0x38
byte MAX77650_getTV_LDO(void); //Returns the LDO Target Output Voltage

//LDO Configuration Register B 0x39
boolean MAX77650_getADE_LDO(void); //Returns LDO active-Discharge Enable
byte MAX77650_getEN_LDO(void); //Enable Control for LDO.

//SIMO Buck-Boost Global Configuration Register 0x28
boolean MAX77650_getMRT_OTP(void); //Returns Manual Reset Time Configuration
boolean MAX77650_getSBIA_LPM_DEF(void); //Returns Default voltage of the SBIA_LPM bit
boolean MAX77650_getDBNC_nEN_DEF(void); //Returns Default Value of the DBNC_nEN bit
byte MAX77650_getDRV_SBB(void); //Returns the SIMO Buck-Boost (all channels) Drive Strength Trim.

//SIMO Buck-Boost 0 Configuration Register A 0x29
byte MAX77650_getIP_SBB0(void); //Returns the SIMO Buck-Boost Channel 0 Peak Current Limit
byte MAX77650_getTV_SBB0(void); //Returns the SIMO Buck-Boost Channel 0 Target Output Voltage

//SIMO Buck-Boost 0 Configuration Register B 0x2A
boolean MAX77650_getADE_SBB0(void); //Returns SIMO Buck-Boost Channel 0 Active-Discharge Enable
byte MAX77650_getEN_SBB0(void); //Returns the Enable Control for SIMO Buck-Boost Channel 0.

//SIMO Buck-Boost 1 Configuration Register A 0x2B
byte MAX77650_getIP_SBB1(void); //Returns the SIMO Buck-Boost Channel 1 Peak Current Limit
byte MAX77650_getTV_SBB1(void); //Returns the SIMO Buck-Boost Channel 1 Target Output Voltage

//SIMO Buck-Boost 1 Configuration Register B 0x2C
boolean MAX77650_getADE_SBB1(void); //Returns SIMO Buck-Boost Channel 1 Active-Discharge Enable
byte MAX77650_getEN_SBB1(void); //Returns the Enable Control for SIMO Buck-Boost Channel 1.

//SIMO Buck-Boost 2 Configuration Register A 0x2D
byte MAX77650_getIP_SBB2(void); //Returns the SIMO Buck-Boost Channel 2 Peak Current Limit
byte MAX77650_getTV_SBB2(void); //Returns the SIMO Buck-Boost Channel 2 Target Output Voltage

//SIMO Buck-Boost 2 Configuration Register B 0x2E
boolean MAX77650_getADE_SBB2(void); //Returns SIMO Buck-Boost Channel 2 Active-Discharge Enable
byte MAX77650_getEN_SBB2(void); //Returns the Enable Control for SIMO Buck-Boost Channel 2.


//***** Register writes *****

//LDO Configuration Register A 0x38
boolean MAX77650_setTV_LDO(byte); //Sets the LDO Target Output Voltage

//LDO Configuration Register B 0x39
boolean MAX77650_setADE_LDO(byte); //Sets LDO active-Discharge Enable
boolean MAX77650_setEN_LDO(byte); //Enable LDO

//Charger Configuration Register A 0x18
boolean MAX77650_setTHM_HOT(byte); //Sets the VHOT JEITA Temperature Threshold
boolean MAX77650_setTHM_WARM(byte); //Sets the VWARM JEITA Temperature Threshold
boolean MAX77650_setTHM_COOL(byte); //Sets the VCOOL JEITA Temperature Threshold
boolean MAX77650_setTHM_COLD(byte); //Sets the VCOLD JEITA Temperature Threshold

//Charger Configuration Register B 0x19
boolean MAX77650_setVCHGIN_MIN(byte); //Sets the Minimum CHGIN regulation voltage (VCHGIN-MIN)
boolean MAX77650_setICHGIN_LIM(byte); //Sets the CHGIN Input Current Limit (ICHGIN-LIM)
boolean MAX77650_setI_PQ(boolean); //Sets the prequalification charge current (IPQ) as a percentage of IFAST-CHG
boolean MAX77650_setCHG_EN(boolean); //Sets Charger Enable

//Charger Configuration Register C 0x1A
boolean MAX77650_setCHG_PQ(byte); //Sets the Battery prequalification voltage threshold (VPQ)
boolean MAX77650_setI_TERM(byte); //Sets the Charger Termination Current (ITERM). I_TERM[1:0] sets the charger termination current as a percentage of the fast charge current IFAST-CHG.
boolean MAX77650_setT_TOPOFF(byte); //Sets the Topoff timer value (tTO)

//Charger Configuration Register D 0x1B
boolean MAX77650_setTJ_REG(byte); //Sets the die junction temperature regulation point, TJ-REG
boolean MAX77650_setVSYS_REG(byte); //Sets the System voltage regulation (VSYS-REG)

//Charger Configuration Register E 0x1C
boolean MAX77650_setCHG_CC(byte); //Sets the fast-charge constant current value, IFAST-CHG.
boolean MAX77650_setT_FAST_CHG(byte); //Sets the fast-charge safety timer, tFC.

//Charger Configuration Register F 0x1D
boolean MAX77650_setCHG_CC_JEITA(byte); //Sets the IFAST-CHG_JEITA for when the battery is either cool or warm as defined by the TCOOL and TWARM temperature thresholds. This register is a don't care if the battery temperature is normal.
boolean MAX77650_setTHM_EN(boolean); //Sets the Thermistor enable bit

//Charger Configuration Register G 0x1E
boolean MAX77650_setCHG_CV(byte); //Sets the fast-charge battery regulation voltage, VFAST-CHG.
boolean MAX77650_setUSBS(boolean); //this bit places CHGIN in USB suspend mode

//Charger Configuration Register H 0x1F
boolean MAX77650_setCHG_CV_JEITA(byte); //Sets the modified VFAST-CHG for when the battery is either cool or warm as defined by the TCOOL and TWARM temperature thresholds. This register is a don't care if the battery temperature is normal.

//Charger Configuration Register I 0x20
boolean MAX77650_setIMON_DISCHG_SCALE(byte); //Sets the Selects the battery discharge current full-scale current value This 4-bit configuration starts at 7.5mA and ends at 300mA.
boolean MAX77650_setMUX_SEL(byte); //Sets the analog channel to connect to AMUX configuration

//SIMO Buck-Boost Global Configuration Register 0x28
boolean MAX77650_setMRT_OTP(boolean); //Sets Manual Reset Time Configuration
boolean MAX77650_setSBIA_LPM_DEF(boolean); //Sets Default voltage of the SBIA_LPM bit
boolean MAX77650_setDBNC_nEN_DEF(boolean); //Sets Default Value of the DBNC_nEN bit
boolean MAX77650_setDRV_SBB(byte); //Sets the SIMO Buck-Boost (all channels) Drive Strength Trim.

//SIMO Buck-Boost 0 Configuration Register A 0x29
boolean MAX77650_setIP_SBB0(byte); //Sets the SIMO Buck-Boost Channel 0 Peak Current Limit
boolean MAX77650_setTV_SBB0(byte); //Sets the SIMO Buck-Boost Channel 0 Target Output Voltage

//SIMO Buck-Boost 0 Configuration Register B 0x2A
boolean MAX77650_setADE_SBB0(boolean); //Sets SIMO Buck-Boost Channel 0 Active-Discharge Enable
boolean MAX77650_setEN_SBB0(byte); //Sets the Enable Control for SIMO Buck-Boost Channel 0.

//SIMO Buck-Boost 1 Configuration Register A 0x2B
boolean MAX77650_setIP_SBB1(byte); //Sets the SIMO Buck-Boost Channel 1 Peak Current Limit
boolean MAX77650_setTV_SBB1(byte); //Sets the SIMO Buck-Boost Channel 1 Target Output Voltage

//SIMO Buck-Boost 1 Configuration Register B 0x2C
boolean MAX77650_setADE_SBB1(boolean); //Sets SIMO Buck-Boost Channel 1 Active-Discharge Enable
boolean MAX77650_setEN_SBB1(byte); //Sets the Enable Control for SIMO Buck-Boost Channel 1.

//SIMO Buck-Boost 2 Configuration Register A 0x2D
boolean MAX77650_setIP_SBB2(byte); //Sets the SIMO Buck-Boost Channel 2 Peak Current Limit
boolean MAX77650_setTV_SBB2(byte); //Sets the SIMO Buck-Boost Channel 2 Target Output Voltage

//SIMO Buck-Boost 2 Configuration Register B 0x2E
boolean MAX77650_setADE_SBB2(boolean); //Sets SIMO Buck-Boost Channel 2 Active-Discharge Enable
boolean MAX77650_setEN_SBB2(byte); //Sets the Enable Control for SIMO Buck-Boost Channel 2.

//LED related registers
boolean MAX77650_setEN_LED_MSTR(boolean); //Sets Master LED Enable Bit
boolean MAX77650_setLED_FS0(byte); //Sets LED0 Full Scale Range
boolean MAX77650_setINV_LED0(boolean); //Sets LED0 Invert
boolean MAX77650_setBRT_LED0(byte); //Sets LED0 Brightness Control
boolean MAX77650_setP_LED0(byte); //Sets LED0 Period Settings
boolean MAX77650_setD_LED0(byte); //Sets LED0 On Duty-Cycle Settings
boolean MAX77650_setLED_FS1(byte); //Sets LED1 Full Scale Range
boolean MAX77650_setINV_LED1(boolean); //Sets LED1 Invert
boolean MAX77650_setBRT_LED1(byte); //Sets LED1 Brightness Control
boolean MAX77650_setP_LED1(byte); //Sets LED1 Period Settings
boolean MAX77650_setD_LED1(byte); //Sets LED1 On Duty-Cycle Settings
boolean MAX77650_setLED_FS2(byte); //Sets LED2 Full Scale Range
boolean MAX77650_setINV_LED2(boolean); //Sets LED2 Invert
boolean MAX77650_setBRT_LED2(byte); //Sets LED2 Brightness Control
boolean MAX77650_setP_LED2(byte); //Sets LED2 Period Settings
boolean MAX77650_setD_LED2(byte); //Sets LED2 On Duty-Cycle Settings

//Global Configuration register 0x10
boolean MAX77650_setBOK(boolean); //Sets Main Bias Okay Status Bit
boolean MAX77650_setSBIA_LPM(boolean); //Sets Main Bias Low-Power Mode software request
boolean MAX77650_setSBIA_EN(boolean); //Sets Main Bias Enable Software Request
boolean MAX77650_setnEN_MODE(boolean); //Sets nEN Input (ONKEY) Default Configuration Mode
boolean MAX77650_setDBEN_nEN(boolean); //Sets Debounce Timer Enable for the nEN Pin
boolean MAX77650_setSFT_RST(byte); //Sets Software Reset Functions.

//Global Interrupt Mask Register 0x06
boolean MAX77650_setINT_M_GLBL(byte); //Sets Global Interrupt Mask Register

//GPIO Configuration Register
boolean MAX77650_setDBEN_GPI(boolean); //Sets General Purpose Input Debounce Timer Enable
boolean MAX77650_setDO(boolean); //Sets General Purpose Output Data Output
boolean MAX77650_setDRV(boolean); //Sets General Purpose Output Driver Type
boolean MAX77650_setDI(boolean); //Sets GPIO Digital Input Value
boolean MAX77650_setDIR(boolean); //Sets GPIO Direction

//Charger Interrupt Mask Register 0x07
boolean MAX77650_setINT_M_CHG(byte); //Sets Global Interrupt Mask Register


#endif


