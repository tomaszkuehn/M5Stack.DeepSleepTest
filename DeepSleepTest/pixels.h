// ----------------------------------------------------------------------------
// Copyright (C) 2020, Tomasz Kuehn
//
// These coded instructions, statements, and computer programs are free
// for personal use.
//
// Maintainer: Tomasz Kuehn
//
// Description: Neopixels M5Stack Fire interface
// ----------------------------------------------------------------------------

#pragma once

#include <FastLED.h>
#include <M5Stack.h>

#define M5STACK_FIRE_NUM_LEDS 10
#define M5STACK_FIRE_NEO_DATA_PIN 15

class FireNeopixels: public CFastLED {
  public:
    FireNeopixels() {
      FastLED.addLeds<WS2812B, M5STACK_FIRE_NEO_DATA_PIN, GRB>(leds, M5STACK_FIRE_NUM_LEDS);
      for(int i = 0; i < M5STACK_FIRE_NUM_LEDS; i++) {
        leds[i] = 0;
      }
    }
    void update() {
      FastLED.show();
    }
    void off() {
      memcpy(leds_cp, leds, sizeof(leds));
      FastLED.clearData();
      FastLED.show();
    }
    void on() {
      memcpy(leds, leds_cp, sizeof(leds));
      FastLED.show();
    }
    CRGB leds   [M5STACK_FIRE_NUM_LEDS]; // array of leds
  private:
    CRGB leds_cp[M5STACK_FIRE_NUM_LEDS];
};
