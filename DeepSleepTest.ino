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
#include <M5Stack.h>
#include "pixels.h"

#define BUTTON_PIN_BITMASK 0x8000000000 // 2^33 in hex

RTC_DATA_ATTR int bootCount = 0;
RTC_SLOW_ATTR int bootCount1 = 0;
FireNeopixels fnp;


void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();
  M5.Lcd.setCursor(10, 50);
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

void setup(){
  M5.begin();
  fnp.leds[0] = CRGB(100, 0, 0);

  fnp.update();
  
  M5.Lcd.setBrightness(10);
  //M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(GREEN , BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(10, 20);
  M5.Lcd.printf("Boot cnt: %d", bootCount1);

  //Increment boot number and print it every reboot
  ++bootCount1;

  //Print the wakeup reason for ESP32
  print_wakeup_reason();

  delay(3000);
  fnp.leds[0] = CRGB(0, 0, 10);

  fnp.update();

  /*
  First we configure the wake up source
  We set our ESP32 to wake up for an external trigger.
  There are two types for ESP32, ext0 and ext1 .
  ext0 uses RTC_IO to wakeup thus requires RTC peripherals
  to be on while ext1 uses RTC Controller so doesnt need
  peripherals to be powered on.
  Note that using internal pullups/pulldowns also requires
  RTC peripherals to be turned on.
  */
  //esp_sleep_enable_ext0_wakeup(GPIO_NUM_33,1); //1 = High, 0 = Low
  //esp_sleep_enable_ext0_wakeup(GPIO_NUM_39,0); //1 = High, 0 = Low
  esp_sleep_enable_timer_wakeup(5000000);  // 5s
  
  //If you were to use ext1, you would use it like
  //esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK,ESP_EXT1_WAKEUP_ANY_HIGH);
  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK,ESP_EXT1_WAKEUP_ALL_LOW);
  //Go to sleep now
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_XTAL, ESP_PD_OPTION_OFF);
  esp_deep_sleep_start();
 
}

void loop(){
  //This is not going to be called
}
