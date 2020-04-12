/*!
 * @file
 * @brief
 */

#ifndef async_spi_h
#define async_spi_h

#include <stdbool.h>
#include <stdint.h>
#include "stm8s.h"
#include "stm8s_itc.h"
#include "hal/i_tiny_async_spi.h"

#ifdef SPI

// Baud rate settings assume 16 MHz clock
enum {
  async_spi_baud_8mhz,
  async_spi_baud_4mhz,
  async_spi_baud_2mhz,
  async_spi_baud_1mhz,
  async_spi_baud_500khz,
  async_spi_baud_250khz,
  async_spi_baud_125khz,
  async_spi_baud_62500hz
};
typedef uint8_t async_spi_baud_t;

i_tiny_async_spi_t* async_spi_init(
  uint8_t cpol,
  uint8_t cpha,
  bool msb_first,
  async_spi_baud_t baud);

void async_spi_isr(void) __interrupt(ITC_IRQ_SPI);

#endif

#endif
