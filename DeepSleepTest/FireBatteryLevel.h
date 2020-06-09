// ----------------------------------------------------------------------------
// Copyright (C) 2020, Tomasz Kuehn
//
// These coded instructions, statements, and computer programs are free
// for personal use.
//
// Maintainer: Tomasz Kuehn
//
// Description: M5Stack Fire battery charge read
// ----------------------------------------------------------------------------

#pragma once

#include <M5Stack.h>

class FireBatteryLevel {
  public:
    uint8_t getBatteryLevel() {
      Wire.beginTransmission(0x75);
      Wire.write(0x78);
      if (Wire.endTransmission(false) == 0
       && Wire.requestFrom(0x75, 1)) {
        switch (Wire.read() & 0xF0) {
        case 0xE0: return 25;
        case 0xC0: return 50;
        case 0x80: return 75;
        case 0x00: return 100;
        default: return 0;
        }
      }
      return -1;
    }
    void enableBatteryLevel() {
      Wire.begin();
    }
};
