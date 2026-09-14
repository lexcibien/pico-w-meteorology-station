/*-------------------------------------------------------------------------
RTC library

Written by Michael C. Miller.

I invest time and resources providing this open source code,
please support me by dontating (see https://github.com/Makuna/Rtc)

-------------------------------------------------------------------------
This file is part of the Makuna/Rtc library.

Rtc is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

Rtc is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with Rtc.  If not, see
<http://www.gnu.org/licenses/>.
-------------------------------------------------------------------------*/

#include <cstdint>
#include <hardware/gpio.h>
#include <pico/time.h>

#pragma once

// ThreeWire command Read/Write flag
const uint8_t THREEWIRE_READFLAG = 0x01;

class ThreeWire {
public:
  ThreeWire(uint8_t ioPin, uint8_t clkPin, uint8_t cePin) : _ioPin(ioPin), _clkPin(clkPin), _cePin(cePin) {}

  void begin() const { resetPins(); }

  void end() const { resetPins(); }

  void beginTransmission(uint8_t command) const {
    gpio_put(_cePin, false); // default, not enabled
    gpio_init(_cePin);
    gpio_set_dir(_cePin, GPIO_OUT != 0U);

    gpio_put(_clkPin, false); // default, clock low
    gpio_init(_clkPin);
    gpio_set_dir(_clkPin, GPIO_OUT != 0U);

    gpio_init(_ioPin);
    gpio_set_dir(_ioPin, GPIO_OUT != 0U);

    gpio_put(_cePin, true); // start the session
    sleep_us(4);            // tCC = 4us

    write(command, (command & THREEWIRE_READFLAG) == THREEWIRE_READFLAG);
  }

  void endTransmission() const{
    gpio_put(_cePin, false);
    sleep_us(4); // tCWH = 4us
  }

  void write(uint8_t value, bool isDataRequestCommand = false) const {
    for (uint8_t bit = 0; bit < 8; bit++) {
      gpio_put(_ioPin, (value & 0x01) != 0);
      sleep_us(1); // tDC = 200ns

      // clock up, data is read by DS1302
      gpio_put(_clkPin, true);
      sleep_us(1); // tCH = 1000ns, tCDH = 800ns

      // for the last bit before a read
      // Set IO line for input before the clock down
      if (bit == 7 && isDataRequestCommand) {
        gpio_init(_ioPin);
        gpio_set_dir(_ioPin, GPIO_IN != 0U);
      }

      gpio_put(_clkPin, false);
      sleep_us(1); // tCL=1000ns, tCDD=800ns

      value >>= 1;
    }
  }

  uint8_t read() const {
    uint8_t value = 0;

    for (uint8_t bit = 0; bit < 8; bit++) {
      // first bit is present on io pin, so only clock the other
      // bits
      value |= (static_cast<int>(gpio_get(_ioPin)) << bit);

      // Clock up, prepare for next
      gpio_put(_clkPin, true);
      sleep_us(1);

      // Clock down, value is ready after some time.
      gpio_put(_clkPin, false);
      sleep_us(1); // tCL=1000ns, tCDD=800ns
    }

    return value;
  }

private:
  uint8_t _ioPin;
  uint8_t _clkPin;
  uint8_t _cePin;

  void resetPins() const {
    // just making sure they are in a default low power use state
    // as required state is set when transmissions are started
    // three wire devices have internal pull downs so they will be low

    gpio_init(_clkPin);
    gpio_set_dir(_clkPin, GPIO_IN != 0U);
    gpio_init(_ioPin);
    gpio_set_dir(_ioPin, GPIO_IN != 0U);
    gpio_init(_cePin);
    gpio_set_dir(_cePin, GPIO_IN != 0U);
  }
};
