/*
Deep Sleep with External Wake Up
=====================================
This code displays how to use deep sleep with
an external trigger as a wake up source and how
to store data in RTC memory to use it over reboots

This code is under Public Domain License.

Hardware Connections
======================
Push Button to GPIO 33 pulled down with a 10K Ohm
resistor

NOTE:
======
Only RTC IO can be used as a source for external wake
source. They are pins: 0,2,4,12-15,25-27,32-39.

Author:
Pranav Cherukupalli <cherukupallip@gmail.com>
*/

//#include "FireBatteryLevel.h"
#include "pixels.h"

#include <driver/rtc_io.h>
#include <driver/adc.h>

#include <M5Stack.h>


#define BUTTON_PIN_BITMASK 0x4000000000 // GPIO38

//RTC_DATA_ATTR int bootCount = 0;
RTC_SLOW_ATTR int bootCount = 0;
//int bootCount = 0;
//FireNeopixels fnp;
//FireBatteryLevel bat1;


void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();
  M5.Lcd.setCursor(10, 60);
  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : M5.Lcd.printf("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : M5.Lcd.printf("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : M5.Lcd.printf("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : M5.Lcd.printf("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : M5.Lcd.printf("Wakeup caused by ULP program"); break;
    default : M5.Lcd.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
  
}



#define IP5306_ADDR 0x75
#define IP5306_REG_SYS_CTL0 0x00  // default 190
#define IP5306_REG_SYS_CTL1 0x01  // default 29
#define IP5306_REG_SYS_CTL2 0x02  // default 96


void setup() {
  uint8_t reg = 0;
  
  bootCount++;
  M5.begin();
  Wire.begin();
  
  M5.Lcd.setBrightness(100);
  M5.Lcd.setTextSize(2);

  M5.Lcd.setCursor(10, 20);
  M5.Lcd.printf("Boot cnt: %d", bootCount);
  M5.Lcd.setCursor(160, 20);
  //M5.Lcd.printf("BAT: %d", M5.Power.getBatteryLevel());
  print_wakeup_reason();

  Wire.beginTransmission(IP5306_ADDR);
  Wire.write(IP5306_REG_SYS_CTL2);
  //Wire.write(0b110111);
  Wire.write(96);//55);
  Wire.endTransmission();
  
  //read IP306 registers
  Wire.beginTransmission(IP5306_ADDR);
  Wire.write(IP5306_REG_SYS_CTL0);
  if (Wire.endTransmission(false) == 0 && Wire.requestFrom(IP5306_ADDR, 3)) {
    reg = Wire.read();
    M5.Lcd.setCursor(10, 120);
    M5.Lcd.printf("Reg: %d", reg);
    reg = Wire.read();
    M5.Lcd.setCursor(10, 140);
    M5.Lcd.printf("Reg: %d", reg);
    reg = Wire.read();
    M5.Lcd.setCursor(10, 160);
    M5.Lcd.printf("Reg: %d", reg);
  }
  
  
  delay(3000);
}

void deepSleep_simple() {
  M5.Power.begin();
  M5.Power.setWakeupButton(BUTTON_A_PIN);
  M5.Power.deepSleep(300000000);
}

void deepSleep_expert() {
  M5.Lcd.setBrightness(0);
  M5.Lcd.sleep();
  M5.Power.begin();
  M5.Power.setPowerBoostKeepOn(true);
  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK,ESP_EXT1_WAKEUP_ALL_LOW);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_39,0);
  esp_sleep_enable_timer_wakeup(40000000);
  esp_deep_sleep_start();  
}

void loop(){
  
  delay(3000);

  deepSleep_expert();
}
