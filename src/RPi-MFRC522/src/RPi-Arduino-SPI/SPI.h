/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@arduino.cc>
 * Copyright (c) 2014 by Paul Stoffregen <paul@pjrc.com> (Transaction API)
 * Copyright (c) 2014 by Matthijs Kooijman <matthijs@stdin.nl> (SPISettings AVR)
 * Copyright (c) 2014 by Andrew J. Kroll <xxxajk@gmail.com> (atomicity fixes)
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef _SPI_H_INCLUDED
#define _SPI_H_INCLUDED

#include <cstddef>

#include "bcm2835.h"

// SPI_HAS_TRANSACTION means SPI has beginTransaction(), endTransaction(),
// usingInterrupt(), and SPISetting(clock, bitOrder, dataMode)
#define SPI_HAS_TRANSACTION 1

// SPI_ATOMIC_VERSION means that SPI has atomicity fixes and what version.
// This way when there is a bug fix you can check this define to alert users
// of your code if it uses better version of this library.
// This also implies everything that SPI_HAS_TRANSACTION as documented above is
// available too.
#define SPI_ATOMIC_VERSION 1

// Uncomment this line to add detection of mismatched begin/end transactions.
// A mismatch occurs if other libraries fail to use SPI.endTransaction() for
// each SPI.beginTransaction().  Connect an LED to this pin.  The LED will turn
// on if any mismatch is ever detected.
//#define SPI_TRANSACTION_MISMATCH_LED 5

#ifndef LSBFIRST
#define LSBFIRST BCM2835_SPI_BIT_ORDER_LSBFIRST
#endif
#ifndef MSBFIRST
#define MSBFIRST BCM2835_SPI_BIT_ORDER_MSBFIRST
#endif

#define SPI_CLOCK_DIV1 BCM2835_SPI_CLOCK_DIVIDER_1
#define SPI_CLOCK_DIV2 BCM2835_SPI_CLOCK_DIVIDER_2
#define SPI_CLOCK_DIV4 BCM2835_SPI_CLOCK_DIVIDER_4
#define SPI_CLOCK_DIV8 BCM2835_SPI_CLOCK_DIVIDER_8
#define SPI_CLOCK_DIV16 BCM2835_SPI_CLOCK_DIVIDER_16
#define SPI_CLOCK_DIV32 BCM2835_SPI_CLOCK_DIVIDER_32
#define SPI_CLOCK_DIV64 BCM2835_SPI_CLOCK_DIVIDER_64
#define SPI_CLOCK_DIV128 BCM2835_SPI_CLOCK_DIVIDER_128
#define SPI_CLOCK_DIV256 BCM2835_SPI_CLOCK_DIVIDER_256
#define SPI_CLOCK_DIV512 BCM2835_SPI_CLOCK_DIVIDER_512
#define SPI_CLOCK_DIV1024 BCM2835_SPI_CLOCK_DIVIDER_1024
#define SPI_CLOCK_DIV2048 BCM2835_SPI_CLOCK_DIVIDER_2048
#define SPI_CLOCK_DIV4096 BCM2835_SPI_CLOCK_DIVIDER_4096
#define SPI_CLOCK_DIV8192 BCM2835_SPI_CLOCK_DIVIDER_8192
#define SPI_CLOCK_DIV16384 BCM2835_SPI_CLOCK_DIVIDER_16384
#define SPI_CLOCK_DIV32768 BCM2835_SPI_CLOCK_DIVIDER_32768
#define SPI_CLOCK_DIV65536 BCM2835_SPI_CLOCK_DIVIDER_65536

#define SPI_MODE0 BCM2835_SPI_MODE0
#define SPI_MODE1 BCM2835_SPI_MODE1
#define SPI_MODE2 BCM2835_SPI_MODE2
#define SPI_MODE3 BCM2835_SPI_MODE3

class SPISettings {
 public:
  SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) {
    _clock = clock;
    _bitOrder = bitOrder;
    _dataMode = dataMode;
  }
  SPISettings() {
    _clock = SPI_CLOCK_DIV64;
    _bitOrder = MSBFIRST;
    _dataMode = SPI_MODE0;
  }

 private:
  uint8_t _clock;
  uint8_t _bitOrder;
  uint8_t _dataMode;
  friend class SPIClass;
};

class SPIClass {
 public:
  SPIClass() { begin(); }

  ~SPIClass() { end(); }

  // Initialize the SPI library
  inline static void begin() { bcm2835_spi_begin(); }

  // Before using SPI.transfer() or asserting chip select pins,
  // this function is used to gain exclusive access to the SPI bus
  // and configure the correct settings.
  inline static void beginTransaction(SPISettings settings) {
    bcm2835_spi_setBitOrder(settings._bitOrder);
    bcm2835_spi_setDataMode(settings._dataMode);
    bcm2835_spi_setClockDivider(settings._clock);
    bcm2835_spi_chipSelect(BCM2835_SPI_CS_NONE);
  }

  // Write to the SPI bus (MOSI pin) and also receive (MISO pin)
  inline static uint8_t transfer(uint8_t data) {
    return bcm2835_spi_transfer(data);
  }

  inline static void transfer(void *buf, size_t count) {
    bcm2835_spi_transfern((char *)buf, count);
  }
  // After performing a group of transfers and releasing the chip select
  // signal, this function allows others to access the SPI bus
  inline static void endTransaction(void) {}

  // Disable the SPI bus
  inline static void end() { bcm2835_spi_end(); }
};

extern SPIClass SPI;

#endif
