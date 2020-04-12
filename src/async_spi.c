/*!
 * @file
 * @brief
 */

#include "stm8s_clk.h"
#include "async_spi.h"

#ifdef SPI

static struct {
  i_tiny_async_spi_t interface;
  void* context;
  const uint8_t* write;
  uint8_t* read;
  uint16_t count;
  uint16_t offset;
  tiny_async_spi_callback_t callback;
} self;

void async_spi_isr(void) __interrupt(ITC_IRQ_SPI) {
  if(self.read) {
    self.read[self.offset] = SPI->DR;
  }

  self.offset++;

  if(self.offset >= self.count) {
    // Disable interrupts
    SPI->ICR = 0;

    self.callback(self.context);
  }
  else {
    SPI->DR = self.write[self.offset];
  }
}

static void transfer(
  i_tiny_async_spi_t* _self,
  const uint8_t* write_buffer,
  uint8_t* read_buffer,
  uint16_t buffer_size,
  tiny_async_spi_callback_t callback,
  void* context) {
  (void)_self;

  self.callback = callback;
  self.context = context;
  self.write = write_buffer;
  self.read = read_buffer;
  self.count = buffer_size;
  self.offset = 0;

  // Enable RX interrupt
  SPI->ICR = SPI_ICR_RXEI;

  if(self.write) {
    SPI->DR = self.write[0];
  }
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

  self.interface.api = &api;

  return &self.interface;
}

#endif
