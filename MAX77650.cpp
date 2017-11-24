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
**********************************************************************/


#include "MAX77650.h"


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

boolean MAX77650_init(void){
    Wire2.begin();
}

int MAX77650_read_register(int ADDR){
  int retval = -1;   
  Wire2.beginTransmission(MAX77650_ADDR);
  Wire2.write(ADDR);
  Wire2.endTransmission();
  Wire2.requestFrom(MAX77650_ADDR,1);
  if (Wire2.available()<=1)
    retval = (Wire2.read());
}

byte MAX77650_write_register(int ADDR, int data){
  int retval = -1;   
  Wire2.beginTransmission(MAX77650_ADDR);
  //delay(100);           // To-Do: Do we need these delays here?!
  Wire2.write(ADDR); 
  //delay(100);
  Wire2.write(data); 
  //delay(100);
  Wire2.endTransmission();
}

boolean MAX77650_getDIDM(void){
  return ((MAX77650_read_register(MAX77650_STAT_GLBL_ADDR) >> 6) & 0b00000011);   
}

boolean MAX77650_getLDO_DropoutDetector(void){ //Returns wheather the LDO is in Dropout or not; Return Value: 0=LDO is not in dropout; 1=LDO is in dropout
  byte retval;
  retval = ((MAX77650_read_register(MAX77650_STAT_GLBL_ADDR) >> 5) & 0b00000001);  
  return retval;
}

boolean MAX77650_getThermalAlarm1(void){ //Returns Thermal Alarm (TJA1 limit); Return Value: 0=Tj<TJA1; 1=Tj>TJA1
  return ((MAX77650_read_register(MAX77650_STAT_GLBL_ADDR) >> 3) & 0b00000001);  
}

boolean MAX77650_getThermalAlarm2(void){ //Returns Therma2 Alarm (TJA2 limit); Return Value: 0=Tj<TJA2; 1=Tj>TJA2
  return ((MAX77650_read_register(MAX77650_STAT_GLBL_ADDR) >> 4) & 0b00000001);  
}

boolean MAX77650_getDebounceStatusnEN0(void){ //Returns Debounced Status of nEN input; Return Value: 0=nEN0 is not active; 1=nEN0 is active
  return ((MAX77650_read_register(MAX77650_STAT_GLBL_ADDR) >> 2) & 0b00000001);  
}

boolean MAX77650_getDebounceStatusPWR_HLD(void){ //Returns Debounced Status of PWR_HLD input; Return Value: 0=logic low; 1=logic high
  return ((MAX77650_read_register(MAX77650_STAT_GLBL_ADDR) >> 1) & 0b00000001);  
}

boolean MAX77650_getPWR_HLD_RST(void){ //Returns whether Reset Source was PWR_HLD; Return Value: 0=no event; 1=RESET Source: PWR_HLD
  return ((MAX77650_read_register(MAX77650_ERCFLAG_ADDR) >> 6) & 0b00000001);    
}

boolean MAX77650_getSFT_CRST_F(void){ //Returns whether Reset Source was CRST_F; Return Value: 0=no event; 1=RESET Source: Software Cold Reset
  return ((MAX77650_read_register(MAX77650_ERCFLAG_ADDR) >> 5) & 0b00000001);      
}

boolean MAX77650_getSFT_OFF_F(void){ //Returns whether Reset Source was OFF_F; Return Value: 0=no event; 1=RESET Source: Software Off Flag
  return ((MAX77650_read_register(MAX77650_ERCFLAG_ADDR) >> 4) & 0b00000001);    
}

boolean MAX77650_getMRST(void){ //Returns whether Reset Source was MRST; Return Value: 0=no event; 1=RESET Source: Manual Reset Timer
  return ((MAX77650_read_register(MAX77650_ERCFLAG_ADDR) >> 3) & 0b00000001);      
}

boolean MAX77650_getSYSUVLO(void){ //Returns whether Reset Source was SYSUVLO; Return Value: 0=no event; 1=RESET Source: System Undervoltage Lockout
  return ((MAX77650_read_register(MAX77650_ERCFLAG_ADDR) >> 2) & 0b00000001);      
}

boolean MAX77650_getSYSOVLO(void){ //Returns whether Reset Source was SYSOVLO; Return Value: 0=no event; 1=RESET Source: System Overvoltage Lockout
  return ((MAX77650_read_register(MAX77650_ERCFLAG_ADDR) >> 1) & 0b00000001);      
}

boolean MAX77650_getTOVLD(void){ //Returns whether Reset Source was TOVLD; Return Value: 0=no event; 1=RESET Source: Thermal Overload
  return (MAX77650_read_register(MAX77650_ERCFLAG_ADDR) & 0b00000001);      
}

byte MAX77650_getChipID(void){ // Return Value regaring OTP parts of the MAX77650
  return (MAX77650_read_register(MAX77650_ERCFLAG_ADDR) & 0b00001111);      
}

boolean MAX77650_VCHGIN_MIN_STAT(void){ //Returns Minimum Input Voltage Regulation Loop Status; Return Value: 0=no event; 1=The minimum CHGIN voltage regulation loop has engaged to regulate VCHGIN >= VCHGIN-MIN
  return ((MAX77650_read_register(MAX77650_STAT_CHG_A_ADDR) >> 6) & 0b00000001);
}

boolean MAX77650_getICHGIN_LIM_STAT(void){ //Returns Input Current Limit Loop Status; Return Value: 0=no event; 1=The CHGIN current limit loop has engaged to regulate ICHGIN <= ICHGIN-LIM
  return ((MAX77650_read_register(MAX77650_STAT_CHG_A_ADDR) >> 5) & 0b00000001);
}

boolean MAX77650_getVSYS_MIN_STAT(void){ //Returns Minimum System Voltage Regulation Loop Status; Return Value: 0=no event; 1=The minimum system voltage regulation loop is engaged to regulate VSYS >= VSYS-MIN
  return ((MAX77650_read_register(MAX77650_STAT_CHG_A_ADDR) >> 4) & 0b00000001);
}

boolean MAX77650_getTJ_REG_STAT(void){ //Returns Maximum Junction Temperature Regulation Loop Status; Return Value: 0=no event; 1=The maximum junction temperature regulation loop has engaged to regulate the junction temperature less than TJ-REG
  return ((MAX77650_read_register(MAX77650_STAT_CHG_A_ADDR) >> 3) & 0b00000001);
}

byte MAX77650_getTHM_DTLS(void){ //Returns Battery Temperature Details; Return Value: Battery Temperature Details
  return (MAX77650_read_register(MAX77650_STAT_CHG_A_ADDR) & 0b00000111);
}

byte MAX77650_getCHG_DTLS(void){ //Returns Charger Details
  return ((MAX77650_read_register(MAX77650_STAT_CHG_B_ADDR) >> 4) & 0b00001111);  
}

byte MAX77650_getCHGIN_DTLS(void){ //Returns CHGIN Status Details
  return ((MAX77650_read_register(MAX77650_STAT_CHG_B_ADDR) >> 2) & 0b00000011);  
}

boolean MAX77650_getCHG(void){ //Returns Quick Charger Status
  return ((MAX77650_read_register(MAX77650_STAT_CHG_B_ADDR) >> 1) & 0b00000001);    
}

boolean MAX77650_getTIME_SUS(void){ //Returns Time Suspend Indicator
  return (MAX77650_read_register(MAX77650_STAT_CHG_B_ADDR) & 0b00000001);    
}



byte MAX77650_getLED_FS0(void){ //Returns LED0 Full Scale Range
  return ((MAX77650_read_register(MAX77650_CNFG_LED0_A_ADDR) >> 6) & 0b00000011);      
}

boolean MAX77650_getINV_LED0(void){ //Returns LED0 Invert
  return ((MAX77650_read_register(MAX77650_CNFG_LED0_A_ADDR) >> 5) & 0b00000001);      
}

byte MAX77650_getBRT_LED0(void){ //Returns LED0 Brightness Control
  return (MAX77650_read_register(MAX77650_CNFG_LED0_A_ADDR) & 0b00011111);      
}

byte MAX77650_getP_LED0(void){ //Returns LED0 Period Settings
  return ((MAX77650_read_register(MAX77650_CNFG_LED0_B_ADDR) >> 4) & 0b00001111);      
}

byte MAX77650_getD_LED0(void){ //Returns LED0 On Duty-Cycle Settings
  return (MAX77650_read_register(MAX77650_CNFG_LED0_B_ADDR) & 0b00001111);     
}



byte MAX77650_getLED_FS1(void){ //Returns LED1 Full Scale Range
  return ((MAX77650_read_register(MAX77650_CNFG_LED1_A_ADDR) >> 6) & 0b00000011);      
}

boolean MAX77650_getINV_LED1(void){ //Returns LED1 Invert
  return ((MAX77650_read_register(MAX77650_CNFG_LED1_A_ADDR) >> 5) & 0b00000001);      
}

byte MAX77650_getBRT_LED1(void){ //Returns LED1 Brightness Control
  return (MAX77650_read_register(MAX77650_CNFG_LED1_A_ADDR) & 0b00011111);      
}

byte MAX77650_getP_LED1(void){ //Returns LED1 Period Settings
  return ((MAX77650_read_register(MAX77650_CNFG_LED1_B_ADDR) >> 4) & 0b00001111);      
}

byte MAX77650_getD_LED1(void){ //Returns LED1 On Duty-Cycle Settings
  return (MAX77650_read_register(MAX77650_CNFG_LED1_B_ADDR) & 0b00001111);     
}

byte MAX77650_getLED_FS2(void){ //Returns LED0 Full Scale Range
  return ((MAX77650_read_register(MAX77650_CNFG_LED2_A_ADDR) >> 6) & 0b00000011);      
}

boolean MAX77650_getINV_LED2(void){ //Returns LED2 Invert
  return ((MAX77650_read_register(MAX77650_CNFG_LED2_A_ADDR) >> 5) & 0b00000001);      
}

byte MAX77650_getBRT_LED2(void){ //Returns LED2 Brightness Control
  return (MAX77650_read_register(MAX77650_CNFG_LED2_A_ADDR) & 0b00011111);      
}

byte MAX77650_getP_LED2(void){ //Returns LED2 Period Settings
  return ((MAX77650_read_register(MAX77650_CNFG_LED2_B_ADDR) >> 4) & 0b00001111);      
}

byte MAX77650_getD_LED2(void){ //Returns LED2 On Duty-Cycle Settings
  return (MAX77650_read_register(MAX77650_CNFG_LED2_B_ADDR) & 0b00001111);     
}

boolean MAX77650_getBOK(void){ //Returns Main Bias Okay Status Bit
  return ((MAX77650_read_register(MAX77650_CNFG_GLBL_ADDR) >> 6) & 0b00000001);        
}

boolean MAX77650_getSBIA_LPM(void){ //Returns Main Bias Low-Power Mode software request
  return ((MAX77650_read_register(MAX77650_CNFG_GLBL_ADDR) >> 5) & 0b00000001);        
}

boolean MAX77650_getSBIA_EN(void){ //Returns Main Bias Enable Software Request
  return ((MAX77650_read_register(MAX77650_CNFG_GLBL_ADDR) >> 4) & 0b00000001);        
}

boolean MAX77650_getnEN_MODE(void){ //Returns nEN Input (ONKEY) Default Configuration Mode
  return ((MAX77650_read_register(MAX77650_CNFG_GLBL_ADDR) >> 3) & 0b00000001);          
}

boolean MAX77650_getDBEN_nEN(void){ //Returns Debounce Timer Enable for the nEN Pin
  return ((MAX77650_read_register(MAX77650_CNFG_GLBL_ADDR) >> 2) & 0b00000001);          
}

byte MAX77650_getSFT_RST(void){ //Returns Software Reset Functions.
  return (MAX77650_read_register(MAX77650_CNFG_GLBL_ADDR) & 0b00000011);          
}

boolean MAX77650_getDOD_R(void){ //Returns LDO Dropout Detector Rising Interrupt
   return ((MAX77650_read_register(MAX77650_INT_GLBL_ADDR) >> 6) & 0b00000001);          
}

boolean MAX77650_getTJAL2_R(void){ //Returns Thermal Alarm 2 Rising Interrupt
   return ((MAX77650_read_register(MAX77650_INT_GLBL_ADDR) >> 5) & 0b00000001);  
}

boolean MAX77650_getTJAL1_R(void){ //Returns Thermal Alarm 1 Rising Interrupt
   return ((MAX77650_read_register(MAX77650_INT_GLBL_ADDR) >> 4) & 0b00000001);  
}

boolean MAX77650_getnEN_R(void){ //Returns nEN Rising Interrupt
   return ((MAX77650_read_register(MAX77650_INT_GLBL_ADDR) >> 3) & 0b00000001);  
}

boolean MAX77650_getnEN_F(void){ //Returns nEN Falling Interrupt
   return ((MAX77650_read_register(MAX77650_INT_GLBL_ADDR) >> 2) & 0b00000001);  
}

boolean MAX77650_getGPI_R(void){ //Returns GPI Rising Interrupt
   return ((MAX77650_read_register(MAX77650_INT_GLBL_ADDR) >> 1) & 0b00000001);
}

boolean MAX77650_getGPI_F(void){ //Returns GPI Falling Interrupt
   return (MAX77650_read_register(MAX77650_INT_GLBL_ADDR) & 0b00000001);  
}

byte MAX77650_getINT_M_GLBL(void){ //Returns Global Interrupt Mask Register
   return (MAX77650_read_register(MAX77650_INTM_GLBL_ADDR) & 0b01111111);  
}

boolean MAX77650_getDBEN_GPI(void){ //Returns General Purpose Input Debounce Timer Enable
   return ((MAX77650_read_register(MAX77650_CNFG_GPIO_ADDR) >> 4) & 0b00000001);  
}
boolean MAX77650_getDO(void){ //Returns General Purpose Output Data Output
   return ((MAX77650_read_register(MAX77650_CNFG_GPIO_ADDR) >> 3) & 0b00000001);  
}
boolean MAX77650_getDRV(void){ //Returns General Purpose Output Driver Type
   return ((MAX77650_read_register(MAX77650_CNFG_GPIO_ADDR) >> 2) & 0b00000001);  
}
boolean MAX77650_getDI(void){ //Returns GPIO Digital Input Value
   return ((MAX77650_read_register(MAX77650_CNFG_GPIO_ADDR) >> 1) & 0b00000001);  
}
boolean MAX77650_getDIR(void){ //Returns GPIO Direction
   return (MAX77650_read_register(MAX77650_CNFG_GPIO_ADDR) & 0b00000001);  
}

//Charger Interrupt Status Register 0x01
boolean MAX77650_getSYS_CNFG_I(void){ //Returns System voltage configuration error interrupt
   return ((MAX77650_read_register(MAX77650_CNFG_GPIO_ADDR) >> 6) & 0b00000001);    
}
boolean MAX77650_getSYS_CTRL_I(void){ //Returns Minimum System Voltage Regulation-loop related interrupt
   return ((MAX77650_read_register(MAX77650_CNFG_GPIO_ADDR) >> 5) & 0b00000001);    
}
boolean MAX77650_getCHGIN_CTRL_I(void){ //Returns CHGIN control-loop related interrupt
   return ((MAX77650_read_register(MAX77650_CNFG_GPIO_ADDR) >> 4) & 0b00000001);    
}
boolean MAX77650_getnTJ_REG_I(void){ //Returns Die junction temperature regulation interrupt
   return ((MAX77650_read_register(MAX77650_CNFG_GPIO_ADDR) >> 3) & 0b00000001);    
}
boolean MAX77650_getCHGIN_I(void){ //Returns CHGIN related interrupt
   return ((MAX77650_read_register(MAX77650_CNFG_GPIO_ADDR) >> 2) & 0b00000001);    
}
boolean MAX77650_getCHG_I(void){ //Returns Charger related interrupt
   return ((MAX77650_read_register(MAX77650_CNFG_GPIO_ADDR) >> 1) & 0b00000001);    
}
boolean MAX77650_getTHM_I(void){ //Returns Thermistor related interrupt
   return (MAX77650_read_register(MAX77650_CNFG_GPIO_ADDR) & 0b00000001);    
}

//Charger Interrupt Mask Register 0x07
byte MAX77650_getINT_M_CHG(void){ //Returns Global Interrupt Mask Register
   return (MAX77650_read_register(MAX77650_INT_M_CHG_ADDR) & 0b01111111);  
}

//Charger Configuration Register A 0x18
byte MAX77650_getTHM_HOT(void){ //Returns the VHOT JEITA Temperature Threshold
  return ((MAX77650_read_register(MAX77650_CNFG_CHG_A_ADDR) >> 6) & 0b00000011);  
}
byte MAX77650_getTHM_WARM(void){ //Returns the VWARM JEITA Temperature Threshold
  return ((MAX77650_read_register(MAX77650_CNFG_CHG_A_ADDR) >> 4) & 0b00000011);
}
byte MAX77650_getTHM_COOL(void){ //Returns the VCOOL JEITA Temperature Threshold
  return ((MAX77650_read_register(MAX77650_CNFG_CHG_A_ADDR) >> 2) & 0b00000011);
}
byte MAX77650_getTHM_COLD(void){ //Returns the VCOLD JEITA Temperature Threshold
  return (MAX77650_read_register(MAX77650_CNFG_CHG_A_ADDR) & 0b00000011);
}

//Charger Configuration Register B 0x19
byte MAX77650_getVCHGIN_MIN(void){ //Returns the Minimum CHGIN regulation voltage (VCHGIN-MIN)
  return ((MAX77650_read_register(MAX77650_CNFG_CHG_B_ADDR) >> 5) & 0b00000111);  
}
byte MAX77650_getICHGIN_LIM(void){ //Returns the CHGIN Input Current Limit (ICHGIN-LIM)
  return ((MAX77650_read_register(MAX77650_CNFG_CHG_B_ADDR) >> 2) & 0b00000111);  
}
boolean MAX77650_getI_PQ(void){ //Returns the prequalification charge current (IPQ) as a percentage of IFAST-CHG
  return ((MAX77650_read_register(MAX77650_CNFG_CHG_B_ADDR) >> 1) & 0b00000001);  
}
boolean MAX77650_getCHG_EN(void){ //Returns Charger Enable
  return (MAX77650_read_register(MAX77650_CNFG_CHG_B_ADDR) & 0b00000001);
}

//Charger Configuration Register C 0x1A
byte MAX77650_getCHG_PQ(void){ //Returns the Battery prequalification voltage threshold (VPQ)
  return ((MAX77650_read_register(MAX77650_CNFG_CHG_C_ADDR) >> 5) & 0b00000111);    
}
byte MAX77650_getI_TERM(void){ //Returns the Charger Termination Current (ITERM). I_TERM[1:0] sets the charger termination current as a percentage of the fast charge current IFAST-CHG.
  return ((MAX77650_read_register(MAX77650_CNFG_CHG_C_ADDR) >> 3) & 0b00000011);    
}
byte MAX77650_getT_TOPOFF(void){ //Returns the Topoff timer value (tTO)
  return (MAX77650_read_register(MAX77650_CNFG_CHG_C_ADDR) & 0b00000111);    
}

//Charger Configuration Register D 0x1B
byte MAX77650_getTJ_REG(void){ //Returns the die junction temperature regulation point, TJ-REG
  return ((MAX77650_read_register(MAX77650_CNFG_CHG_D_ADDR) >> 5) & 0b00000111);    
}
byte MAX77650_getVSYS_REG(void){ //Returns the System voltage regulation (VSYS-REG)
  return (MAX77650_read_register(MAX77650_CNFG_CHG_D_ADDR) & 0b00011111);   
}

//Charger Configuration Register E 0x1C
byte MAX77650_getCHG_CC(void){ //Returns the fast-charge constant current value, IFAST-CHG.
  return ((MAX77650_read_register(MAX77650_CNFG_CHG_E_ADDR) >> 2) & 0b00111111);    
}
byte MAX77650_getT_FAST_CHG(void){ //Returns the fast-charge safety timer, tFC.
  return (MAX77650_read_register(MAX77650_CNFG_CHG_E_ADDR) & 0b00000011);  
}

//Charger Configuration Register F 0x1D
byte MAX77650_getCHG_CC_JEITA(void){ //Returns the IFAST-CHG_JEITA for when the battery is either cool or warm as defined by the TCOOL and TWARM temperature thresholds. This register is a don't care if the battery temperature is normal.
  return ((MAX77650_read_register(MAX77650_CNFG_CHG_F_ADDR) >> 2) & 0b00111111);  
}
byte MAX77650_getTHM_EN(void){ //Returns the Thermistor enable bit
  return ((MAX77650_read_register(MAX77650_CNFG_CHG_F_ADDR) >> 1) & 0b00000001);  
}

//Charger Configuration Register G 0x1E
byte MAX77650_getCHG_CV(void){ //Returns the fast-charge battery regulation voltage, VFAST-CHG.
  return ((MAX77650_read_register(MAX77650_CNFG_CHG_G_ADDR) >> 2) & 0b00111111);  
}
boolean MAX77650_getUSBS(void){ //this bit places CHGIN in USB suspend mode
  return ((MAX77650_read_register(MAX77650_CNFG_CHG_G_ADDR) >> 1) & 0b00000001);  
}

//Charger Configuration Register H 0x1F
byte MAX77650_getCHG_CV_JEITA(void){ //Returns the modified VFAST-CHG for when the battery is either cool or warm as defined by the TCOOL and TWARM temperature thresholds. This register is a don't care if the battery temperature is normal.
  return ((MAX77650_read_register(MAX77650_CNFG_CHG_H_ADDR) >> 2) & 0b00111111);  
}

//Charger Configuration Register I 0x20
byte MAX77650_getIMON_DISCHG_SCALE(void){ //Returns the Selects the battery discharge current full-scale current value This 4-bit configuration starts at 7.5mA and ends at 300mA.
  return ((MAX77650_read_register(MAX77650_CNFG_CHG_I_ADDR) >> 4) & 0b00001111);  
}
byte MAX77650_getMUX_SEL(void){ //Returns the analog channel to connect to AMUX configuration
  return (MAX77650_read_register(MAX77650_CNFG_CHG_I_ADDR) & 0b00001111);  
}

//LDO Configuration Register A 0x38
byte MAX77650_getTV_LDO(void){ //Returns the LDO Target Output Voltage
  return (MAX77650_read_register(MAX77650_CNFG_LDO_A_ADDR) & 0b01111111);    
}

//LDO Configuration Register B 0x39
boolean MAX77650_getADE_LDO(void){ //Returns LDO active-Discharge Enable
  return ((MAX77650_read_register(MAX77650_CNFG_LDO_B_ADDR) >> 3) & 0b00000001);    
}
byte MAX77650_getEN_LDO(void){ //Enable Control for LDO.
  return (MAX77650_read_register(MAX77650_CNFG_LDO_B_ADDR) & 0b00000111);    
}

//SIMO Buck-Boost Global Configuration Register 0x28
boolean MAX77650_getMRT_OTP(void){ //Returns Manual Reset Time Configuration
  return ((MAX77650_read_register(MAX77650_CNFG_SBB_TOP_ADDR) >> 6) & 0b00000001);  
}
boolean MAX77650_getSBIA_LPM_DEF(void){ //Returns Default voltage of the SBIA_LPM bit
  return ((MAX77650_read_register(MAX77650_CNFG_SBB_TOP_ADDR) >> 5) & 0b00000001);  
}
boolean MAX77650_getDBNC_nEN_DEF(void){ //Returns Default Value of the DBNC_nEN bit
  return ((MAX77650_read_register(MAX77650_CNFG_SBB_TOP_ADDR) >> 4) & 0b00000001);  
}
byte MAX77650_getDRV_SBB(void){ //Returns the SIMO Buck-Boost (all channels) Drive Strength Trim.
  return (MAX77650_read_register(MAX77650_CNFG_SBB_TOP_ADDR) & 0b00000011);  
}

//SIMO Buck-Boost 0 Configuration Register A 0x29
byte MAX77650_getIP_SBB0(void){ //Returns the SIMO Buck-Boost Channel 1 Peak Current Limit
  return ((MAX77650_read_register(MAX77650_CNFG_SBB0_A_ADDR) >> 6) & 0b00000011);  
}
byte MAX77650_getTV_SBB0(void){ //Returns the SIMO Buck-Boost Channel 0 Target Output Voltage
  return (MAX77650_read_register(MAX77650_CNFG_SBB0_A_ADDR) & 0b00111111);  
}

//SIMO Buck-Boost 0 Configuration Register B 0x2A
boolean MAX77650_getADE_SBB0(void){ //Returns SIMO Buck-Boost Channel 0 Active-Discharge Enable
  return ((MAX77650_read_register(MAX77650_CNFG_SBB0_B_ADDR) >> 3) & 0b00000001);  
}
byte MAX77650_getEN_SBB0(void){ //Returns the Enable Control for SIMO Buck-Boost Channel 0.
  return (MAX77650_read_register(MAX77650_CNFG_SBB0_B_ADDR) & 0b00000111);  
}

//SIMO Buck-Boost 1 Configuration Register A 0x2B
byte MAX77650_getIP_SBB1(void){ //Returns the SIMO Buck-Boost Channel 1 Peak Current Limit
  return ((MAX77650_read_register(MAX77650_CNFG_SBB1_A_ADDR) >> 6) & 0b00000011);  
}
byte MAX77650_getTV_SBB1(void){ //Returns the SIMO Buck-Boost Channel 1 Target Output Voltage
  return (MAX77650_read_register(MAX77650_CNFG_SBB1_A_ADDR) & 0b00111111);    
}

//SIMO Buck-Boost 1 Configuration Register B 0x2C
boolean MAX77650_getADE_SBB1(void){ //Returns SIMO Buck-Boost Channel 1 Active-Discharge Enable
  return ((MAX77650_read_register(MAX77650_CNFG_SBB1_B_ADDR) >> 3) & 0b00000001);   
}
byte MAX77650_getEN_SBB1(void){ //Returns the Enable Control for SIMO Buck-Boost Channel 1.
  return (MAX77650_read_register(MAX77650_CNFG_SBB1_B_ADDR) & 0b00000111);  
}

//SIMO Buck-Boost 2 Configuration Register A 0x2D
byte MAX77650_getIP_SBB2(void){ //Returns the SIMO Buck-Boost Channel 2 Peak Current Limit
  return ((MAX77650_read_register(MAX77650_CNFG_SBB2_A_ADDR) >> 6) & 0b00000011);  
}
byte MAX77650_getTV_SBB2(void){ //Returns the SIMO Buck-Boost Channel 2 Target Output Voltage
  return (MAX77650_read_register(MAX77650_CNFG_SBB2_A_ADDR) & 0b00111111);  
}

//SIMO Buck-Boost 2 Configuration Register B 0x2E
boolean MAX77650_getADE_SBB2(void){ //Returns SIMO Buck-Boost Channel 2 Active-Discharge Enable
  return ((MAX77650_read_register(MAX77650_CNFG_SBB2_B_ADDR) >> 3) & 0b00000001);  
}
byte MAX77650_getEN_SBB2(void){ //Returns the Enable Control for SIMO Buck-Boost Channel 2.
  return (MAX77650_read_register(MAX77650_CNFG_SBB2_B_ADDR) & 0b00000111);  
}

boolean MAX77650_getCLK_64_S(void){ //Returns 64Hz Clock Status
  return ((MAX77650_read_register(MAX77650_CNFG_LED_TOP_ADDR) >> 1) & 0b00000001);  
}
boolean MAX77650_getEN_LED_MSTR(void){ //Returns Master LED Enable Bit
  return (MAX77650_read_register(MAX77650_CNFG_LED_TOP_ADDR) & 0b00000001);  
}


//CID Register
byte MAX77650_getCID(void){ // Returns the OTP programmed Chip Identification Code
  return (MAX77650_read_register(MAX77650_CNFG_LED_TOP_ADDR) & 0b00001111); 
}


//***** Register writes *****

boolean MAX77650_setTV_LDO(byte target_val){ //Sets the LDO Target Output Voltage
  MAX77650_write_register(MAX77650_CNFG_LDO_A_ADDR,((MAX77650_read_register(MAX77650_CNFG_LDO_A_ADDR) & 0b10000000) | ((target_val & 0b01111111) << 0)));
  if(MAX77650_getTV_LDO()==target_val)
   return true;
  else
   return false; 
}

//LDO Configuration Register B 0x39
boolean MAX77650_setADE_LDO(byte target_val){ //Sets LDO active-Discharge Enable
  MAX77650_write_register(MAX77650_CNFG_LDO_B_ADDR,((MAX77650_read_register(MAX77650_CNFG_LDO_B_ADDR) & 0b11110111) | ((target_val & 0b00000001) << 3)));
  if(MAX77650_getADE_LDO()==target_val)
   return true;
  else
   return false; 
}

boolean MAX77650_setEN_LDO(byte target_val){ //Enable LDO
  MAX77650_write_register(MAX77650_CNFG_LDO_B_ADDR,((MAX77650_read_register(MAX77650_CNFG_LDO_B_ADDR) & 0b11111000) | ((target_val & 0b00000111) << 0)));
  if(MAX77650_getEN_LDO()==target_val)
   return true;
  else
   return false; 
}

//Charger Configuration Register A 0x18
boolean MAX77650_setTHM_HOT(byte target_val){ //Returns the VHOT JEITA Temperature Threshold
  MAX77650_write_register(MAX77650_CNFG_CHG_A_ADDR,((MAX77650_read_register(MAX77650_CNFG_CHG_A_ADDR) & 0b00111111) | ((target_val & 0b00000011) << 6)));
  if(MAX77650_getTHM_HOT()==target_val)
   return true;
  else
   return false;   
}
boolean MAX77650_setTHM_WARM(byte target_val){ //Returns the VWARM JEITA Temperature Threshold
  MAX77650_write_register(MAX77650_CNFG_CHG_A_ADDR,((MAX77650_read_register(MAX77650_CNFG_CHG_A_ADDR) & 0b11001111) | ((target_val & 0b00000011) << 4)));
  if(MAX77650_getTHM_WARM()==target_val)
   return true;
  else
   return false;     
}
boolean MAX77650_setTHM_COOL(byte target_val){ //Returns the VCOOL JEITA Temperature Threshold
  MAX77650_write_register(MAX77650_CNFG_CHG_A_ADDR,((MAX77650_read_register(MAX77650_CNFG_CHG_A_ADDR) & 0b11110011) | ((target_val & 0b00000011) << 2)));
  if(MAX77650_getTHM_COOL()==target_val)
   return true;
  else
   return false;       
}
boolean MAX77650_setTHM_COLD(byte target_val){ //Returns the VCOLD JEITA Temperature Threshold
  MAX77650_write_register(MAX77650_CNFG_CHG_A_ADDR,((MAX77650_read_register(MAX77650_CNFG_CHG_A_ADDR) & 0b11111100) | ((target_val & 0b00000011) << 0)));
  if(MAX77650_getTHM_COLD()==target_val)
   return true;
  else
   return false;       
}

//Charger Configuration Register B 0x19
boolean MAX77650_setVCHGIN_MIN(byte target_val){ //Returns the Minimum CHGIN regulation voltage (VCHGIN-MIN)
  MAX77650_write_register(MAX77650_CNFG_CHG_B_ADDR,((MAX77650_read_register(MAX77650_CNFG_CHG_B_ADDR) & 0b00011111) | ((target_val & 0b00000111) << 5)));
  if(MAX77650_getVCHGIN_MIN()==target_val)
   return true;
  else
   return false;       
}
boolean MAX77650_setICHGIN_LIM(byte target_val){ //Returns the CHGIN Input Current Limit (ICHGIN-LIM)
  MAX77650_write_register(MAX77650_CNFG_CHG_B_ADDR,((MAX77650_read_register(MAX77650_CNFG_CHG_B_ADDR) & 0b11100011) | ((target_val & 0b00000111) << 2)));
  if(MAX77650_getICHGIN_LIM()==target_val)
   return true;
  else
   return false;  
}
boolean MAX77650_setI_PQ(boolean target_val){ //Returns the prequalification charge current (IPQ) as a percentage of IFAST-CHG
  MAX77650_write_register(MAX77650_CNFG_CHG_B_ADDR,((MAX77650_read_register(MAX77650_CNFG_CHG_B_ADDR) & 0b11111101) | ((target_val & 0b00000001) << 1)));
  if(MAX77650_getI_PQ()==target_val)
   return true;
  else
   return false;
}
boolean MAX77650_setCHG_EN(boolean target_val){ //Returns Charger Enable
  MAX77650_write_register(MAX77650_CNFG_CHG_B_ADDR,((MAX77650_read_register(MAX77650_CNFG_CHG_B_ADDR) & 0b11111110) | ((target_val & 0b00000001) << 0)));
  if(MAX77650_getCHG_EN()==target_val)
   return true;
  else
   return false;
}

//Charger Configuration Register C 0x1A
boolean MAX77650_setCHG_PQ(byte target_val){ //Returns the Battery prequalification voltage threshold (VPQ)
  MAX77650_write_register(MAX77650_CNFG_CHG_C_ADDR,((MAX77650_read_register(MAX77650_CNFG_CHG_C_ADDR) & 0b00011111) | ((target_val & 0b00000111) << 5)));
  if(MAX77650_getCHG_PQ()==target_val)
   return true;
  else
   return false;  
}
boolean MAX77650_setI_TERM(byte target_val){ //Returns the Charger Termination Current (ITERM). I_TERM[1:0] sets the charger termination current as a percentage of the fast charge current IFAST-CHG.
  MAX77650_write_register(MAX77650_CNFG_CHG_C_ADDR,((MAX77650_read_register(MAX77650_CNFG_CHG_C_ADDR) & 0b11100111) | ((target_val & 0b00000011) << 3)));
  if(MAX77650_getI_TERM()==target_val)
   return true;
  else
   return false;  
}
boolean MAX77650_setT_TOPOFF(byte target_val){ //Returns the Topoff timer value (tTO)
  MAX77650_write_register(MAX77650_CNFG_CHG_C_ADDR,((MAX77650_read_register(MAX77650_CNFG_CHG_C_ADDR) & 0b11111000) | ((target_val & 0b00000111) << 0)));
  if(MAX77650_getT_TOPOFF()==target_val)
   return true;
  else
   return false;  
}

//Charger Configuration Register D 0x1B
boolean MAX77650_setTJ_REG(byte target_val){ //Returns the die junction temperature regulation point, TJ-REG
  MAX77650_write_register(MAX77650_CNFG_CHG_D_ADDR,((MAX77650_read_register(MAX77650_CNFG_CHG_D_ADDR) & 0b00011111) | ((target_val & 0b00000111) << 5)));
  if(MAX77650_getTJ_REG()==target_val)
   return true;
  else
   return false;  
}
boolean MAX77650_setVSYS_REG(byte target_val){ //Returns the System voltage regulation (VSYS-REG)
  MAX77650_write_register(MAX77650_CNFG_CHG_D_ADDR,((MAX77650_read_register(MAX77650_CNFG_CHG_D_ADDR) & 0b11100000) | ((target_val & 0b00011111) << 0)));
  if(MAX77650_getVSYS_REG()==target_val)
   return true;
  else
   return false;  
}

//Charger Configuration Register E 0x1C
boolean MAX77650_setCHG_CC(byte target_val){ //Returns the fast-charge constant current value, IFAST-CHG.
  MAX77650_write_register(MAX77650_CNFG_CHG_E_ADDR,((MAX77650_read_register(MAX77650_CNFG_CHG_E_ADDR) & 0b00000011) | ((target_val & 0b00111111) << 2)));
  if(MAX77650_getCHG_CC()==target_val)
   return true;
  else
   return false;  
}
boolean MAX77650_setT_FAST_CHG(byte target_val){ //Returns the fast-charge safety timer, tFC.
  MAX77650_write_register(MAX77650_CNFG_CHG_E_ADDR,((MAX77650_read_register(MAX77650_CNFG_CHG_E_ADDR) & 0b11111100) | ((target_val & 0b00000011) << 0)));
  if(MAX77650_getT_FAST_CHG()==target_val)
   return true;
  else
   return false;  
}

//Charger Configuration Register F 0x1D
boolean MAX77650_setCHG_CC_JEITA(byte target_val){ //Returns the IFAST-CHG_JEITA for when the battery is either cool or warm as defined by the TCOOL and TWARM temperature thresholds. This register is a don't care if the battery temperature is normal.
  MAX77650_write_register(MAX77650_CNFG_CHG_F_ADDR,((MAX77650_read_register(MAX77650_CNFG_CHG_F_ADDR) & 0b00000011) | ((target_val & 0b00111111) << 2)));
  if(MAX77650_getCHG_CC_JEITA()==target_val)
   return true;
  else
   return false;    
}
boolean MAX77650_setTHM_EN(boolean target_val){ //Returns the Thermistor enable bit
  MAX77650_write_register(MAX77650_CNFG_CHG_F_ADDR,((MAX77650_read_register(MAX77650_CNFG_CHG_F_ADDR) & 0b11111101) | ((target_val & 0b00000001) << 1)));
  if(MAX77650_getTHM_EN()==target_val)
   return true;
  else
   return false;    
}

//Charger Configuration Register G 0x1E
boolean MAX77650_setCHG_CV(byte target_val){ //Returns the fast-charge battery regulation voltage, VFAST-CHG.
  MAX77650_write_register(MAX77650_CNFG_CHG_G_ADDR,((MAX77650_read_register(MAX77650_CNFG_CHG_G_ADDR) & 0b00000011) | ((target_val & 0b00111111) << 2)));
  if(MAX77650_getCHG_CV()==target_val)
   return true;
  else
   return false;    
}
boolean MAX77650_setUSBS(boolean target_val){ //this bit places CHGIN in USB suspend mode
  MAX77650_write_register(MAX77650_CNFG_CHG_G_ADDR,((MAX77650_read_register(MAX77650_CNFG_CHG_G_ADDR) & 0b11111101) | ((target_val & 0b00000001) << 1)));
  if(MAX77650_getUSBS()==target_val)
   return true;
  else
   return false;    
}

//Charger Configuration Register H 0x1F
boolean MAX77650_setCHG_CV_JEITA(byte target_val){ //Returns the modified VFAST-CHG for when the battery is either cool or warm as defined by the TCOOL and TWARM temperature thresholds. This register is a don't care if the battery temperature is normal.
  MAX77650_write_register(MAX77650_CNFG_CHG_H_ADDR,((MAX77650_read_register(MAX77650_CNFG_CHG_H_ADDR) & 0b00000011) | ((target_val & 0b00111111) << 2)));
  if(MAX77650_getCHG_CV_JEITA()==target_val)
   return true;
  else
   return false;    
}

//Charger Configuration Register I 0x20
boolean MAX77650_setIMON_DISCHG_SCALE(byte target_val){ //Returns the Selects the battery discharge current full-scale current value This 4-bit configuration starts at 7.5mA and ends at 300mA.
  MAX77650_write_register(MAX77650_CNFG_CHG_I_ADDR,((MAX77650_read_register(MAX77650_CNFG_CHG_I_ADDR) & 0b00001111) | ((target_val & 0b00001111) << 4)));
  if(MAX77650_getIMON_DISCHG_SCALE()==target_val)
   return true;
  else
   return false;    
}
boolean MAX77650_setMUX_SEL(byte target_val){ //Returns the analog channel to connect to AMUX configuration
  MAX77650_write_register(MAX77650_CNFG_CHG_I_ADDR,((MAX77650_read_register(MAX77650_CNFG_CHG_I_ADDR) & 0b11110000) | ((target_val & 0b00001111) << 0)));
  if(MAX77650_getMUX_SEL()==target_val)
   return true;
  else
   return false;    
}

//SIMO Buck-Boost Global Configuration Register 0x28
boolean MAX77650_setMRT_OTP(boolean target_val){ //Returns Manual Reset Time Configuration
  MAX77650_write_register(MAX77650_CNFG_SBB_TOP_ADDR,((MAX77650_read_register(MAX77650_CNFG_SBB_TOP_ADDR) & 0b10111111) | ((target_val & 0b00000001) << 6)));
  if(MAX77650_getMRT_OTP()==target_val)
   return true;
  else
   return false;    
}
boolean MAX77650_setSBIA_LPM_DEF(boolean target_val){ //Returns Default voltage of the SBIA_LPM bit
  MAX77650_write_register(MAX77650_CNFG_SBB_TOP_ADDR,((MAX77650_read_register(MAX77650_CNFG_SBB_TOP_ADDR) & 0b11011111) | ((target_val & 0b00000001) << 5)));
  if(MAX77650_getSBIA_LPM_DEF()==target_val)
   return true;
  else
   return false;  
}
boolean MAX77650_setDBNC_nEN_DEF(boolean target_val){ //Returns Default Value of the DBNC_nEN bit
  MAX77650_write_register(MAX77650_CNFG_SBB_TOP_ADDR,((MAX77650_read_register(MAX77650_CNFG_SBB_TOP_ADDR) & 0b11101111) | ((target_val & 0b00000001) << 4)));
  if(MAX77650_getDBNC_nEN_DEF()==target_val)
   return true;
  else
   return false;  
}
boolean MAX77650_setDRV_SBB(byte target_val){ //Returns the SIMO Buck-Boost (all channels) Drive Strength Trim.
  MAX77650_write_register(MAX77650_CNFG_SBB_TOP_ADDR,((MAX77650_read_register(MAX77650_CNFG_SBB_TOP_ADDR) & 0b11111100) | ((target_val & 0b00000011) << 0)));
  if(MAX77650_getDRV_SBB()==target_val)
   return true;
  else
   return false;  
}

//SIMO Buck-Boost 0 Configuration Register A 0x29
boolean MAX77650_setIP_SBB0(byte target_val){ //Returns the SIMO Buck-Boost Channel 0 Peak Current Limit
  MAX77650_write_register(MAX77650_CNFG_SBB0_A_ADDR,((MAX77650_read_register(MAX77650_CNFG_SBB0_A_ADDR) & 0b00111111) | ((target_val & 0b00000011) << 6)));
  if(MAX77650_getIP_SBB0()==target_val)
   return true;
  else
   return false;  
}
boolean MAX77650_setTV_SBB0(byte target_val){ //Returns the SIMO Buck-Boost Channel 0 Target Output Voltage
  MAX77650_write_register(MAX77650_CNFG_SBB0_A_ADDR,((MAX77650_read_register(MAX77650_CNFG_SBB0_A_ADDR) & 0b11000000) | ((target_val & 0b00111111) << 0)));
  if(MAX77650_getTV_SBB0()==target_val)
   return true;
  else
   return false;    
}

//SIMO Buck-Boost 0 Configuration Register B 0x2A
boolean MAX77650_setADE_SBB0(boolean target_val){ //Returns SIMO Buck-Boost Channel 0 Active-Discharge Enable
  MAX77650_write_register(MAX77650_CNFG_SBB0_B_ADDR,((MAX77650_read_register(MAX77650_CNFG_SBB0_B_ADDR) & 0b11110111) | ((target_val & 0b00000001) << 3)));
  if(MAX77650_getADE_SBB0()==target_val)
   return true;
  else
   return false;    
}
boolean MAX77650_setEN_SBB0(byte target_val){ //Returns the Enable Control for SIMO Buck-Boost Channel 0.
  MAX77650_write_register(MAX77650_CNFG_SBB0_B_ADDR,((MAX77650_read_register(MAX77650_CNFG_SBB0_B_ADDR) & 0b11111000) | ((target_val & 0b00000111) << 0)));
  if(MAX77650_getEN_SBB0()==target_val)
   return true;
  else
   return false;    
}

//SIMO Buck-Boost 1 Configuration Register A 0x2B
boolean MAX77650_setIP_SBB1(byte target_val){ //Returns the SIMO Buck-Boost Channel 1 Peak Current Limit
  MAX77650_write_register(MAX77650_CNFG_SBB1_A_ADDR,((MAX77650_read_register(MAX77650_CNFG_SBB1_A_ADDR) & 0b00111111) | ((target_val & 0b00000011) << 6)));
  if(MAX77650_getIP_SBB1()==target_val)
   return true;
  else
   return false;  
}
boolean MAX77650_setTV_SBB1(byte target_val){ //Returns the SIMO Buck-Boost Channel 1 Target Output Voltage
  MAX77650_write_register(MAX77650_CNFG_SBB1_A_ADDR,((MAX77650_read_register(MAX77650_CNFG_SBB1_A_ADDR) & 0b11000000) | ((target_val & 0b00111111) << 0)));
  if(MAX77650_getTV_SBB1()==target_val)
   return true;
  else
   return false;    
}

//SIMO Buck-Boost 1 Configuration Register B 0x2C
boolean MAX77650_setADE_SBB1(boolean target_val){ //Returns SIMO Buck-Boost Channel 1 Active-Discharge Enable
  MAX77650_write_register(MAX77650_CNFG_SBB1_B_ADDR,((MAX77650_read_register(MAX77650_CNFG_SBB1_B_ADDR) & 0b11110111) | ((target_val & 0b00000001) << 3)));
  if(MAX77650_getADE_SBB1()==target_val)
   return true;
  else
   return false;    
}
boolean MAX77650_setEN_SBB1(byte target_val){ //Returns the Enable Control for SIMO Buck-Boost Channel 1.
  MAX77650_write_register(MAX77650_CNFG_SBB1_B_ADDR,((MAX77650_read_register(MAX77650_CNFG_SBB1_B_ADDR) & 0b11111000) | ((target_val & 0b00000111) << 0)));
  if(MAX77650_getEN_SBB1()==target_val)
   return true;
  else
   return false;    
}

//SIMO Buck-Boost 2 Configuration Register A 0x2D
boolean MAX77650_setIP_SBB2(byte target_val){ //Returns the SIMO Buck-Boost Channel 2 Peak Current Limit
  MAX77650_write_register(MAX77650_CNFG_SBB2_A_ADDR,((MAX77650_read_register(MAX77650_CNFG_SBB2_A_ADDR) & 0b00111111) | ((target_val & 0b00000011) << 6)));
  if(MAX77650_getIP_SBB2()==target_val)
   return true;
  else
   return false;  
}
boolean MAX77650_setTV_SBB2(byte target_val){ //Returns the SIMO Buck-Boost Channel 2 Target Output Voltage
  MAX77650_write_register(MAX77650_CNFG_SBB2_A_ADDR,((MAX77650_read_register(MAX77650_CNFG_SBB2_A_ADDR) & 0b11000000) | ((target_val & 0b00111111) << 0)));
  if(MAX77650_getTV_SBB2()==target_val)
   return true;
  else
   return false;    
}

//SIMO Buck-Boost 2 Configuration Register B 0x2E
boolean MAX77650_setADE_SBB2(boolean target_val){ //Returns SIMO Buck-Boost Channel 2 Active-Discharge Enable
  MAX77650_write_register(MAX77650_CNFG_SBB2_B_ADDR,((MAX77650_read_register(MAX77650_CNFG_SBB2_B_ADDR) & 0b11110111) | ((target_val & 0b00000001) << 3)));
  if(MAX77650_getADE_SBB2()==target_val)
   return true;
  else
   return false;    
}
boolean MAX77650_setEN_SBB2(byte target_val){ //Returns the Enable Control for SIMO Buck-Boost Channel 2.
  MAX77650_write_register(MAX77650_CNFG_SBB2_B_ADDR,((MAX77650_read_register(MAX77650_CNFG_SBB2_B_ADDR) & 0b11111000) | ((target_val & 0b00000111) << 0)));
  if(MAX77650_getEN_SBB2()==target_val)
   return true;
  else
   return false;    
}

//LED related registers
boolean MAX77650_setEN_LED_MSTR(boolean target_val){ //Returns Master LED Enable Bit
  MAX77650_write_register(MAX77650_CNFG_LED_TOP_ADDR,((MAX77650_read_register(MAX77650_CNFG_LED_TOP_ADDR) & 0b11111110) | ((target_val & 0b00000001) << 0)));
  if(MAX77650_getEN_LED_MSTR()==target_val)
   return true;
  else
   return false;    
}

//Register LED0
boolean MAX77650_setLED_FS0(byte target_val){ //Returns LED0 Full Scale Range
  MAX77650_write_register(MAX77650_CNFG_LED0_A_ADDR,((MAX77650_read_register(MAX77650_CNFG_LED0_A_ADDR) & 0b00111111) | ((target_val & 0b00000011) << 6)));
  if(MAX77650_getLED_FS0()==target_val)
   return true;
  else
   return false;    
}
boolean MAX77650_setINV_LED0(boolean target_val){ //Returns LED0 Invert
  MAX77650_write_register(MAX77650_CNFG_LED0_A_ADDR,((MAX77650_read_register(MAX77650_CNFG_LED0_A_ADDR) & 0b11011111) | ((target_val & 0b00000001) << 5)));
  if(MAX77650_getINV_LED0()==target_val)
   return true;
  else
   return false;    
}
boolean MAX77650_setBRT_LED0(byte target_val){ //Returns LED0 Brightness Control
  MAX77650_write_register(MAX77650_CNFG_LED0_A_ADDR,((MAX77650_read_register(MAX77650_CNFG_LED0_A_ADDR) & 0b11100000) | ((target_val & 0b00011111) << 0)));
  if(MAX77650_getBRT_LED0()==target_val)
   return true;
  else
   return false;    
}
boolean MAX77650_setP_LED0(byte target_val){ //Returns LED0 Period Settings
  MAX77650_write_register(MAX77650_CNFG_LED0_B_ADDR,((MAX77650_read_register(MAX77650_CNFG_LED0_B_ADDR) & 0b00001111) | ((target_val & 0b00001111) << 4)));
  if(MAX77650_getP_LED0()==target_val)
   return true;
  else
   return false;    
}
boolean MAX77650_setD_LED0(byte target_val){ //Returns LED0 On Duty-Cycle Settings
  MAX77650_write_register(MAX77650_CNFG_LED0_B_ADDR,((MAX77650_read_register(MAX77650_CNFG_LED0_B_ADDR) & 0b11110000) | ((target_val & 0b00001111) << 0)));
  if(MAX77650_getD_LED0()==target_val)
   return true;
  else
   return false;    
}

//Register LED1
boolean MAX77650_setLED_FS1(byte target_val){ //Returns LED1 Full Scale Range
  MAX77650_write_register(MAX77650_CNFG_LED1_A_ADDR,((MAX77650_read_register(MAX77650_CNFG_LED1_A_ADDR) & 0b00111111) | ((target_val & 0b00000011) << 6)));
  if(MAX77650_getLED_FS1()==target_val)
   return true;
  else
   return false;    
}
boolean MAX77650_setINV_LED1(boolean target_val){ //Returns LED1 Invert
  MAX77650_write_register(MAX77650_CNFG_LED1_A_ADDR,((MAX77650_read_register(MAX77650_CNFG_LED1_A_ADDR) & 0b11011111) | ((target_val & 0b00000001) << 5)));
  if(MAX77650_getINV_LED1()==target_val)
   return true;
  else
   return false;    
}
boolean MAX77650_setBRT_LED1(byte target_val){ //Returns LED1 Brightness Control
  MAX77650_write_register(MAX77650_CNFG_LED1_A_ADDR,((MAX77650_read_register(MAX77650_CNFG_LED1_A_ADDR) & 0b11100000) | ((target_val & 0b00011111) << 0)));
  if(MAX77650_getBRT_LED1()==target_val)
   return true;
  else
   return false;    
}
boolean MAX77650_setP_LED1(byte target_val){ //Returns LED1 Period Settings
  MAX77650_write_register(MAX77650_CNFG_LED1_B_ADDR,((MAX77650_read_register(MAX77650_CNFG_LED1_B_ADDR) & 0b00001111) | ((target_val & 0b00001111) << 4)));
  if(MAX77650_getP_LED1()==target_val)
   return true;
  else
   return false;    
}
boolean MAX77650_setD_LED1(byte target_val){ //Returns LED1 On Duty-Cycle Settings
  MAX77650_write_register(MAX77650_CNFG_LED1_B_ADDR,((MAX77650_read_register(MAX77650_CNFG_LED1_B_ADDR) & 0b11110000) | ((target_val & 0b00001111) << 0)));
  if(MAX77650_getD_LED1()==target_val)
   return true;
  else
   return false;    
}

//Register LED2
boolean MAX77650_setLED_FS2(byte target_val){ //Returns LED2 Full Scale Range
  MAX77650_write_register(MAX77650_CNFG_LED2_A_ADDR,((MAX77650_read_register(MAX77650_CNFG_LED2_A_ADDR) & 0b00111111) | ((target_val & 0b00000011) << 6)));
  if(MAX77650_getLED_FS2()==target_val)
   return true;
  else
   return false;    
}
boolean MAX77650_setINV_LED2(boolean target_val){ //Returns LED2 Invert
  MAX77650_write_register(MAX77650_CNFG_LED2_A_ADDR,((MAX77650_read_register(MAX77650_CNFG_LED2_A_ADDR) & 0b11011111) | ((target_val & 0b00000001) << 5)));
  if(MAX77650_getINV_LED2()==target_val)
   return true;
  else
   return false;    
}
boolean MAX77650_setBRT_LED2(byte target_val){ //Returns LED2 Brightness Control
  MAX77650_write_register(MAX77650_CNFG_LED2_A_ADDR,((MAX77650_read_register(MAX77650_CNFG_LED2_A_ADDR) & 0b11100000) | ((target_val & 0b00011111) << 0)));
  if(MAX77650_getBRT_LED2()==target_val)
   return true;
  else
   return false;    
}
boolean MAX77650_setP_LED2(byte target_val){ //Returns LED2 Period Settings
  MAX77650_write_register(MAX77650_CNFG_LED2_B_ADDR,((MAX77650_read_register(MAX77650_CNFG_LED2_B_ADDR) & 0b00001111) | ((target_val & 0b00001111) << 4)));
  if(MAX77650_getP_LED2()==target_val)
   return true;
  else
   return false;    
}
boolean MAX77650_setD_LED2(byte target_val){ //Returns LED2 On Duty-Cycle Settings
  MAX77650_write_register(MAX77650_CNFG_LED2_B_ADDR,((MAX77650_read_register(MAX77650_CNFG_LED2_B_ADDR) & 0b11110000) | ((target_val & 0b00001111) << 0)));
  if(MAX77650_getD_LED2()==target_val)
   return true;
  else
   return false;    
}

//Global Configuration register 0x10
boolean MAX77650_setBOK(boolean target_val){ //Returns Main Bias Okay Status Bit
  MAX77650_write_register(MAX77650_CNFG_GLBL_ADDR,((MAX77650_read_register(MAX77650_CNFG_GLBL_ADDR) & 0b10111111) | ((target_val & 0b00000001) << 6)));
  if(MAX77650_getBOK()==target_val)
   return true;
  else
   return false;  
}
boolean MAX77650_setSBIA_LPM(boolean target_val){ //Returns Main Bias Low-Power Mode software request
  MAX77650_write_register(MAX77650_CNFG_GLBL_ADDR,((MAX77650_read_register(MAX77650_CNFG_GLBL_ADDR) & 0b11011111) | ((target_val & 0b00000001) << 5)));
  if(MAX77650_getSBIA_LPM()==target_val)
   return true;
  else
   return false;  
}
boolean MAX77650_setSBIA_EN(boolean target_val){ //Returns Main Bias Enable Software Request
  MAX77650_write_register(MAX77650_CNFG_GLBL_ADDR,((MAX77650_read_register(MAX77650_CNFG_GLBL_ADDR) & 0b11101111) | ((target_val & 0b00000001) << 4)));
  if(MAX77650_getSBIA_EN()==target_val)
   return true;
  else
   return false;  
}
boolean MAX77650_setnEN_MODE(boolean target_val){ //Returns nEN Input (ONKEY) Default Configuration Mode
  MAX77650_write_register(MAX77650_CNFG_GLBL_ADDR,((MAX77650_read_register(MAX77650_CNFG_GLBL_ADDR) & 0b11110111) | ((target_val & 0b00000001) << 3)));
  if(MAX77650_getnEN_MODE()==target_val)
   return true;
  else
   return false;  
}
boolean MAX77650_setDBEN_nEN(boolean target_val){ //Returns Debounce Timer Enable for the nEN Pin
  MAX77650_write_register(MAX77650_CNFG_GLBL_ADDR,((MAX77650_read_register(MAX77650_CNFG_GLBL_ADDR) & 0b11111011) | ((target_val & 0b00000001) << 2)));
  if(MAX77650_getDBEN_nEN()==target_val)
   return true;
  else
   return false;  
}
boolean MAX77650_setSFT_RST(byte target_val){ //Returns Software Reset Functions.
  MAX77650_write_register(MAX77650_CNFG_GLBL_ADDR,((MAX77650_read_register(MAX77650_CNFG_GLBL_ADDR) & 0b11111100) | ((target_val & 0b00000011) << 0)));
  if(MAX77650_getSFT_RST()==target_val)
   return true;
  else
   return false;  
}

//Global Interrupt Mask Register 0x06
boolean MAX77650_setINT_M_GLBL(byte target_val){ //Sets Global Interrupt Mask Register
  MAX77650_write_register(MAX77650_INTM_GLBL_ADDR,((MAX77650_read_register(MAX77650_INTM_GLBL_ADDR) & 0b01111111) | ((target_val & 0b01111111) << 0)));
  if(MAX77650_getINT_M_GLBL()==target_val)
   return true;
  else
   return false;   
}

//GPIO Configuration Register
boolean MAX77650_setDBEN_GPI(boolean target_val){ //Returns General Purpose Input Debounce Timer Enable
  MAX77650_write_register(MAX77650_CNFG_GPIO_ADDR,((MAX77650_read_register(MAX77650_CNFG_GPIO_ADDR) & 0b11101111) | ((target_val & 0b00000001) << 4)));
  if(MAX77650_getDBEN_GPI()==target_val)
   return true;
  else
   return false;
}
boolean MAX77650_setDO(boolean target_val){ //Returns General Purpose Output Data Output
  MAX77650_write_register(MAX77650_CNFG_GPIO_ADDR,((MAX77650_read_register(MAX77650_CNFG_GPIO_ADDR) & 0b11110111) | ((target_val & 0b00000001) << 3)));
  if(MAX77650_getDO()==target_val)
   return true;
  else
   return false;
}
boolean MAX77650_setDRV(boolean target_val){ //Returns General Purpose Output Driver Type
  MAX77650_write_register(MAX77650_CNFG_GPIO_ADDR,((MAX77650_read_register(MAX77650_CNFG_GPIO_ADDR) & 0b11111011) | ((target_val & 0b00000001) << 2)));
  if(MAX77650_getDRV()==target_val)
   return true;
  else
   return false;
}
boolean MAX77650_setDI(boolean target_val){ //Returns GPIO Digital Input Value
  MAX77650_write_register(MAX77650_CNFG_GPIO_ADDR,((MAX77650_read_register(MAX77650_CNFG_GPIO_ADDR) & 0b11111101) | ((target_val & 0b00000001) << 1)));
  if(MAX77650_getDI()==target_val)
   return true;
  else
   return false;
}
boolean MAX77650_setDIR(boolean target_val){ //Returns GPIO Direction
  MAX77650_write_register(MAX77650_CNFG_GPIO_ADDR,((MAX77650_read_register(MAX77650_CNFG_GPIO_ADDR) & 0b11111110) | ((target_val & 0b00000001) << 0)));
  if(MAX77650_getDIR()==target_val)
   return true;
  else
   return false;
}

//Charger Interrupt Mask Register 0x07
boolean MAX77650_setINT_M_CHG(byte target_val){ //Sets Global Interrupt Mask Register
  MAX77650_write_register(MAX77650_INT_M_CHG_ADDR,((MAX77650_read_register(MAX77650_INT_M_CHG_ADDR) & 0b11111110) | ((target_val & 0b00000001) << 0)));
  if(MAX77650_getINT_M_CHG()==target_val)
   return true;
  else
   return false;
}



