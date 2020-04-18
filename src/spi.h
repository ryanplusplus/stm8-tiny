/*!
 * @file
 * @brief
 */

#ifndef spi_h
#define spi_h

#include <stdbool.h>
#include <stdint.h>
#include "stm8s.h"
#include "stm8s_itc.h"
#include "hal/i_tiny_spi.h"

#ifdef SPI

// Baud rate settings assume 16 MHz clock
enum {
  spi_baud_8mhz,
  spi_baud_4mhz,
  spi_baud_2mhz,
  spi_baud_1mhz,
  spi_baud_500khz,
  spi_baud_250khz,
  spi_baud_125khz,
  spi_baud_62500hz
};
typedef uint8_t spi_baud_t;

i_tiny_spi_t* spi_init(
  uint8_t cpol,
  uint8_t cpha,
  bool msb_first,
  spi_baud_t baud);

#endif

#endif
