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

#include "FireBatteryLevel.h"
#include "pixels.h"

#include <M5Stack.h>
#include <WiFi.h>


#define BUTTON_PIN_BITMASK 0x8000000000 // 2^33 in hex

//RTC_DATA_ATTR int bootCount = 0;
RTC_SLOW_ATTR int bootCount = 0;
FireNeopixels fnp;
FireBatteryLevel bat1;


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

void setup(){
  
  fnp.leds[0] = CRGB(100, 0, 0);
  fnp.update();
  bootCount++;
  
  if(1) { //esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_TIMER) { // == ESP_SLEEP_WAKEUP_EXT1) {
    bat1.enableBatteryLevel();
    
    M5.begin();
    
    M5.Lcd.setBrightness(10);
    //M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(GREEN , BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(10, 20);
    M5.Lcd.printf("Cnt: %d", bootCount);
    M5.Lcd.setCursor(200, 20);
    M5.Lcd.printf("BAT: %d", bat1.getBatteryLevel());
  
  
    //Print the wakeup reason for ESP32
    print_wakeup_reason();

    WiFi.begin("NCC-1701N", "");
    //connects in about 1sec
    while (WiFi.status() != WL_CONNECTED) {
        delay(10);
    }
    M5.Lcd.setCursor(10, 120);
    //shows only the important part of IP address
    //ping this addres from a computer on the same network and see when it wakes up
    M5.Lcd.printf("IP: %d .. %d",WiFi.localIP()[0], WiFi.localIP()[3]);
    delay(3000);
  }
  
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
  esp_sleep_enable_timer_wakeup(30000000);  // 30s
  
  //If you were to use ext1, you would use it like
  //esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK,ESP_EXT1_WAKEUP_ANY_HIGH);
  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK,ESP_EXT1_WAKEUP_ALL_LOW);
  //Go to sleep now
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
  //esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_XTAL, ESP_PD_OPTION_OFF);
  esp_deep_sleep_start();
 
}

void loop(){
  //This is not going to be called
}
