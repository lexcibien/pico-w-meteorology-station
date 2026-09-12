/*!
 *  @file DHT.cpp
 *
 *  @mainpage DHT series of low cost temperature/humidity sensors.
 *
 *  @section intro_sec Introduction
 *
 *  This is a library for DHT series of low cost temperature/humidity sensors.
 *
 *  You must have Adafruit Unified Sensor Library library installed to use this
 * class.
 *
 *  Adafruit invests time and resources providing this open source code,
 *  please support Adafruit andopen-source hardware by purchasing products
 *  from Adafruit!
 *
 *  @section author Author
 *
 *  Written by Adafruit Industries.
 *
 *  @section license License
 *
 *  MIT license, all text above must be included in any redistribution
 */

#include "DHT.h"

#include <hardware/clocks.h>
#include <hardware/gpio.h>
#include <math.h>
#include <pico/time.h>

#include <cmath>

#define MIN_INTERVAL 2000 /**< min interval value */
#define TIMEOUT                                      \
  UINT32_MAX /**< Used programmatically for timeout. \
                   Not a timeout duration. Type: uint32_t. */

critical_section_t InterruptLock::my_crit_sec;

inline uint32_t microseconds_to_clock_cycles(uint32_t us) {
  // clock_get_Hz(clk_sys) returns the frequency in Hz (e.g., 125000000)
  uint32_t cycles_per_us = clock_get_hz(clk_sys) / 1000000L;
  return us * cycles_per_us;
}

/*!
 *  @brief  Instantiates a new DHT class
 *  @param  pin
 *          pin number that sensor is connected
 *  @param  type
 *          type of sensor
 *  @param  count
 *          number of sensors
 */
DHT::DHT(uint8_t pin, uint8_t type, uint8_t count)
    : _pin(pin), _type(type), _maxcycles(microseconds_to_clock_cycles(1000)) {
  (void)count; // Workaround to avoid compiler warning.
}

/*!
 *  @brief  Setup sensor pins and set pull timings
 *  @param  usec
 *          Optionally pass pull-up time (in microseconds) before DHT reading
 *starts. Default is 55 (see function declaration in DHT.h).
 */
void DHT::begin(uint8_t usec) {
  // set up the pins!
  gpio_init(_pin);
  gpio_set_dir(_pin, GPIO_IN != 0U);
  gpio_pull_up(_pin);
  // Using this value makes sure that to_ms_since_boot(get_absolute_time()) -
  // lastreadtime will be
  // >= MIN_INTERVAL right away. Note that this assignment wraps around,
  // but so will the subtraction.
  _lastreadtime = to_ms_since_boot(get_absolute_time()) - MIN_INTERVAL;
  DEBUG_PRINT("DHT max clock cycles: ");
  DEBUG_PRINTLN(_maxcycles, DEC);
  pullTime = usec;
}

/*!
 *  @brief  Read temperature
 *  @param  S
 *          Scale. Boolean value:
 *					- true = Fahrenheit
 *					- false = Celcius
 *  @param  force
 *          true if in force mode
 *	@return Temperature value in selected scale
 */
float DHT::readTemperature(bool S, bool force) {
  float f = NAN;

  if (read(force)) {
    switch (_type) {
      case DHT11:
        f = data[2];
        if ((data[3] & 0x80) != 0) {
          f = -1 - f;
        }
        f += (data[3] & 0x0f) * 0.1;
        if (S) {
          f = convertCtoF(f);
        }
        break;
      case DHT12:
        f = data[2];
        f += (data[3] & 0x0f) * 0.1F;
        if ((data[2] & 0x80) != 0.0F) {
          f *= -1;
        }
        if (S) {
          f = convertCtoF(f);
        }
        break;
      case DHT22:
      case DHT21:
      default:
        f = (static_cast<uint16_t>(data[2] & 0x7F)) << 8 | data[3];
        f *= 0.1;
        if ((data[2] & 0x80) != 0) {
          f *= -1;
        }
        if (S) {
          f = convertCtoF(f);
        }
        break;
    }
  }
  return f;
}

/*!
 *  @brief  Converts Celcius to Fahrenheit
 *  @param  c
 *					value in Celcius
 *	@return float value in Fahrenheit
 */
float DHT::convertCtoF(float c) {
  return (c * 1.8F) + 32;
}

/*!
 *  @brief  Converts Fahrenheit to Celcius
 *  @param  f
 *					value in Fahrenheit
 *	@return float value in Celcius
 */
float DHT::convertFtoC(float f) {
  return (f - 32) * 0.55555F;
}

/*!
 *  @brief  Read Humidity
 *  @param  force
 *					force read mode
 *	@return float value - humidity in percent
 */
float DHT::readHumidity(bool force) {
  float f = NAN;
  if (read(force)) {
    switch (_type) {
      case DHT11:
      case DHT12:
        f = data[0] + (data[1] * 0.1F);
        break;
      case DHT22:
      case DHT21:
      default:
        f = (static_cast<uint16_t>(data[0])) << 8 | data[1];
        f *= 0.1;
        break;
    }
  }
  return f;
}

/*!
 *  @brief  Compute Heat Index
 *          Simplified version that reads temp and humidity from sensor
 *  @param  isFahrenheit
 * 					true if fahrenheit, false if celcius
 *(default true)
 *	@return float heat index
 */
float DHT::computeHeatIndex(bool isFahrenheit) {
  float hi = computeHeatIndex(readTemperature(isFahrenheit), readHumidity(),
                              isFahrenheit);
  return hi;
}

/*!
 *  @brief  Compute Heat Index
 *  				Using both Rothfusz and Steadman's equations
 *					(http://www.wpc.ncep.noaa.gov/html/heatindex_equation.shtml)
 *  @param  temperature
 *          temperature in selected scale
 *  @param  percentHumidity
 *          humidity in percent
 *  @param  isFahrenheit
 * 					true if fahrenheit, false if celcius
 *	@return float heat index
 */
float DHT::computeHeatIndex(float temperature, float percentHumidity,
                            bool isFahrenheit) {
  float hi = NAN;

  if (!isFahrenheit) {
    temperature = convertCtoF(temperature);
  }

  hi = 0.5F * (temperature + 61.0F + ((temperature - 68.0F) * 1.2F) +
               (percentHumidity * 0.094F));

  if (hi > 79) {
    hi = -42.379F + (2.04901523F * temperature) +
         (10.14333127F * percentHumidity) +
         (-0.22475541F * temperature * percentHumidity) +
         (-0.00683783F * pow(temperature, 2)) +
         (-0.05481717F * pow(percentHumidity, 2)) +
         (0.00122874F * pow(temperature, 2) * percentHumidity) +
         (0.00085282F * temperature * pow(percentHumidity, 2)) +
         (-0.00000199F * pow(temperature, 2) * pow(percentHumidity, 2));

    if ((percentHumidity < 13) && (temperature >= 80.0) &&
        (temperature <= 112.0)) {
      hi -= ((13.0F - percentHumidity) * 0.25F) *
            sqrt((17.0F - abs(temperature - 95.0F)) * 0.05882F);

    } else if ((percentHumidity > 85.0F) && (temperature >= 80.0F) &&
               (temperature <= 87.0F)) {
      hi += ((percentHumidity - 85.0F) * 0.1F) * ((87.0F - temperature) * 0.2F);
    }
  }

  return isFahrenheit ? hi : convertFtoC(hi);
}

/*!
 *  @brief  Read value from sensor or return last one from less than two
 *seconds.
 *  @param  force
 *          true if using force mode
 *	@return float value
 */
bool DHT::read(bool force) {
  // Check if sensor was read less than two seconds ago and return early
  // to use last reading.
  uint32_t currenttime = to_ms_since_boot(get_absolute_time());
  if (!force && ((currenttime - _lastreadtime) < MIN_INTERVAL)) {
    return _lastresult; // return last correct measurement
  }
  _lastreadtime = currenttime;

  // Reset 40 bits of received data to zero.
  data[0] = data[1] = data[2] = data[3] = data[4] = 0;

#if defined(ESP8266)
  yield(); // Handle WiFi / reset software watchdog
#endif

  // Send start signal.  See DHT datasheet for full signal diagram:
  //   http://www.adafruit.com/datasheets/Digital%20humidity%20and%20temperature%20sensor%20AM2302.pdf

  // Go into high impedence state to let pull-up raise data line level and
  // start the reading process.
  gpio_init(_pin);
  gpio_set_dir(_pin, GPIO_IN != 0U);
  gpio_pull_up(_pin);
  sleep_ms(1);

  // First set data line low for a period according to sensor type
  gpio_init(_pin);
  gpio_set_dir(_pin, GPIO_OUT != 0U);
  gpio_put(_pin, false);
  switch (_type) {
    case DHT22:
    case DHT21:
      sleep_us(1100); // data sheet says "at least 1ms"
      break;
    case DHT11:
    default:
      sleep_ms(20); // data sheet says at least 18ms, 20ms just to be safe
      break;
  }

  uint32_t cycles[80];
  {
    // End the start signal by setting data line high for 40 microseconds.
    gpio_init(_pin);
    gpio_set_dir(_pin, GPIO_IN != 0U);
    gpio_pull_up(_pin);

    // Delay a moment to let sensor pull data line low.
    sleep_us(pullTime);

    // Now start reading the data line to get the value from the DHT sensor.

    // Turn off interrupts temporarily because the next sections
    // are timing critical and we don't want any interruptions.
    InterruptLock lock;

    // First expect a low signal for ~80 microseconds followed by a high signal
    // for ~80 microseconds again.
    if (expectPulse(false) == TIMEOUT) {
      DEBUG_PRINTLN(F("DHT timeout waiting for start signal low pulse."));
      _lastresult = false;
      return _lastresult;
    }
    if (expectPulse(false) == TIMEOUT) {
      DEBUG_PRINTLN(F("DHT timeout waiting for start signal high pulse."));
      _lastresult = false;
      return _lastresult;
    }

    // Now read the 40 bits sent by the sensor.  Each bit is sent as a 50
    // microsecond low pulse followed by a variable length high pulse.  If the
    // high pulse is ~28 microseconds then it's a 0 and if it's ~70 microseconds
    // then it's a 1.  We measure the cycle count of the initial 50us low pulse
    // and use that to compare to the cycle count of the high pulse to determine
    // if the bit is a 0 (high state cycle count < low state cycle count), or a
    // 1 (high state cycle count > low state cycle count). Note that for speed
    // all the pulses are read into a array and then examined in a later step.
    for (int i = 0; i < 80; i += 2) {
      cycles[i] = expectPulse(false);
      cycles[i + 1] = expectPulse(true);
    }
  } // Timing critical code is now complete.

  // Inspect pulses and determine which ones are 0 (high state cycle count < low
  // state cycle count), or 1 (high state cycle count > low state cycle count).
  for (int i = 0; i < 40; ++i) {
    uint32_t lowCycles = cycles[2 * i];
    uint32_t highCycles = cycles[(2 * i) + 1];
    if ((lowCycles == TIMEOUT) || (highCycles == TIMEOUT)) {
      DEBUG_PRINTLN(F("DHT timeout waiting for pulse."));
      _lastresult = false;
      return _lastresult;
    }
    data[i / 8] <<= static_cast<uint8_t>(1);
    // Now compare the low and high cycle times to see if the bit is a 0 or 1.
    if (highCycles > lowCycles) {
      // High cycles are greater than 50us low cycle count, must be a 1.
      data[i / 8] |= static_cast<uint8_t>(1);
    }
    // Else high cycles are less than (or equal to, a weird case) the 50us low
    // cycle count so this must be a zero.  Nothing needs to be changed in the
    // stored data.
  }

  DEBUG_PRINTLN(F("Received from DHT:"));
  DEBUG_PRINT(data[0], HEX);
  DEBUG_PRINT(F(", "));
  DEBUG_PRINT(data[1], HEX);
  DEBUG_PRINT(F(", "));
  DEBUG_PRINT(data[2], HEX);
  DEBUG_PRINT(F(", "));
  DEBUG_PRINT(data[3], HEX);
  DEBUG_PRINT(F(", "));
  DEBUG_PRINT(data[4], HEX);
  DEBUG_PRINT(F(" =? "));
  DEBUG_PRINTLN((data[0] + data[1] + data[2] + data[3]) & 0xFF, HEX);

  // Check we read 40 bits and that the checksum matches.
  if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
    _lastresult = true;
    return _lastresult;
  }
  DEBUG_PRINTLN(F("DHT checksum failure!"));
  _lastresult = false;
  return _lastresult;
}

// Expect the signal line to be at the specified level for a period of time and
// return a count of loop cycles spent at that level (this cycle count can be
// used to compare the relative time of two pulses).  If more than a millisecond
// ellapses without the level changing then the call fails with a 0 response.
// This is adapted from Arduino's pulseInLong function (which is only available
// in the very latest IDE versions):
//   https://github.com/arduino/Arduino/blob/master/hardware/arduino/avr/cores/arduino/wiring_pulse.c
uint32_t DHT::expectPulse(bool level) const {
// F_CPU is not be known at compile time on platforms such as STM32F103.
// The preprocessor seems to evaluate it to zero in that case.
#if (F_CPU > 16000000L) || (F_CPU == 0L)
  uint32_t count = 0;
#else
  uint16_t count = 0; // To work fast enough on slower AVR boards
#endif
// On AVR platforms use direct GPIO port access as it's much faster and better
// for catching pulses that are 10's of microseconds in length:
#ifdef __AVR
  uint8_t portState = level ? _bit : 0;
  while ((*portInputRegister(_port) & _bit) == portState) {
    if (count++ >= _maxcycles) {
      return TIMEOUT; // Exceeded timeout, fail.
    }
  }
// Otherwise fall back to using digitalRead (this seems to be necessary on
// ESP8266 right now, perhaps bugs in direct port access functions?).
#else
  while (gpio_get(_pin) == level) {
    if (count++ >= _maxcycles) {
      return TIMEOUT; // Exceeded timeout, fail.
    }
  }
#endif

  return count;
}
