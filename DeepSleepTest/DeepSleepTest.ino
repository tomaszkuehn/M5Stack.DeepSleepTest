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

//#include <esp_wifi.h>
//#include <esp_bt.h>
#include <driver/rtc_io.h>
#include <driver/adc.h>

#include <M5Stack.h>


#define BUTTON_PIN_BITMASK 0x8000000000 // 2^33 in hex

//RTC_DATA_ATTR int bootCount = 0;
RTC_SLOW_ATTR int bootCount = 0;
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
#define IP5306_REG_SYS_CTL0 0x00
#define IP5306_REG_SYS_CTL1 0x01
#define IP5306_REG_SYS_CTL2 0x02


void setup() {
  bootCount++;
  M5.begin();
  //Wire.begin();
  
  M5.Lcd.setBrightness(100);
  M5.Lcd.setTextSize(2);

  M5.Lcd.setCursor(10, 20);
  M5.Lcd.printf("Boot cnt: %d", bootCount);
  M5.Lcd.setCursor(160, 20);
  //M5.Lcd.printf("BAT: %d", M5.Power.getBatteryLevel());
  print_wakeup_reason();
  

  M5.Power.begin();
  M5.Power.setWakeupButton(BUTTON_A_PIN);
  //M5.Power.setPowerBoostKeepOn(true);
  /*Wire.beginTransmission(IP5306_ADDR);
  Wire.write(IP5306_REG_SYS_CTL0);
  //Wire.write(0b110111);
  Wire.write(190);//55);*/
  // 0x37 = 0b110111 TCALL example
  /*
  [1] Boost EN (default 1) [EXM note: if 0 ESP32 will not boot from battery]
  [1] Charger EN (1) [EXM note: did not observe difference]
  [1] Reserved (1) [EXM note: did not observe difference]
  [1] Insert load auto power EN (1) [EXM note: did not observe difference]
  [1] BOOST output normally open ( 1) [EXM note: if 0 will shutdown when
  ESP32 sleeps after 32s]
  [1] Key off EN: (0) [EXM note: could not detect difference]
  */ 
  /*Wire.write(IP5306_REG_SYS_CTL1);
  Wire.write(29);
  Wire.write(IP5306_REG_SYS_CTL2);
  Wire.write(100);*/
  //Wire.write(0x1D); // Set HEX:1D DEC:29 BIN:11111
  /*
  [1] Turn off boost control signal selection: short press twice
  [1] Switch WLED flashlight control signal selection: short press twice
  [1] Short press switch boost: disabled
  [0] Whether to turn on Boost after VIN is pulled out: opened
  [1] Batlow 3.0V Low Power Shutdown EN: enabled
  
  M5.Lcd.printf("I2C updated\n");
  */
}

void loop(){
  
  delay(3000);

  M5.Power.deepSleep(60000000);
  //esp_sleep_enable_timer_wakeup(60000000);
  //esp_deep_sleep_start();
}
