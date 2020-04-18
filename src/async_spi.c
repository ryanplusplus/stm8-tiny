/*!
 * @file
 * @brief
 */

#include "stm8s_clk.h"
#include "async_spi.h"

#ifdef SPI

static i_tiny_async_spi_t self;
static void* context;
static const uint8_t* write_buffer;
static uint8_t* read_buffer;
static uint16_t count;
static uint16_t offset;
static tiny_async_spi_callback_t callback;

void async_spi_isr(void) __interrupt(ITC_IRQ_SPI) {
  if(read_buffer) {
    read_buffer[offset] = SPI->DR;
  }
  else {
    uint8_t dummy = SPI->DR;
  }

  offset++;

  if(offset >= count) {
    // Disable interrupts
    SPI->ICR = 0;

    callback(context);
  }
  else {
    SPI->DR = write_buffer ? write_buffer[offset] : 0;
  }
}

static void transfer(
  i_tiny_async_spi_t* _self,
  const uint8_t* _write_buffer,
  uint8_t* _read_buffer,
  uint16_t _buffer_size,
  tiny_async_spi_callback_t _callback,
  void* _context) {
  (void)_self;

  callback = _callback;
  context = _context;
  write_buffer = _write_buffer;
  read_buffer = _read_buffer;
  count = _buffer_size;
  offset = 0;

  // Enable RX interrupt
  SPI->ICR = SPI_ICR_RXEI;

  SPI->DR = write_buffer ? write_buffer[0] : 0;
}

static const i_tiny_async_spi_api_t api = { transfer };

i_tiny_async_spi_t* async_spi_init(uint8_t cpol, uint8_t cpha, bool msb_first, async_spi_baud_t baud) {
  // Un-gate clock for SPI
  CLK->PCKENR1 |= (1 << CLK_PERIPHERAL_SPI);

  // Enable peripheral in master mode
  SPI->CR1 = SPI_CR1_SPE | SPI_CR1_MSTR | (baud << 3);

  if(!msb_first) {
    SPI->CR1 |= SPI_CR1_LSBFIRST;
  }

  if(cpol) {
    SPI->CR1 |= SPI_CR1_CPOL;
  }

  if(cpha) {
    SPI->CR1 |= SPI_CR1_CPHA;
  }

  self.api = &api;

  return &self;
}

#endif
