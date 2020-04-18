/*!
 * @file
 * @brief
 */

#include "stm8s_clk.h"
#include "spi.h"

#ifdef SPI

static i_tiny_spi_t self;

static inline void wait_for_tx_empty(void) {
  while(!(SPI->SR & SPI_SR_TXE)) {
  }
}

static inline void wait_for_rx_not_empty(void) {
  while(!(SPI->SR & SPI_SR_RXNE)) {
  }
}

static void transfer(
  i_tiny_spi_t* _self,
  const uint8_t* write_buffer,
  uint8_t* read_buffer,
  uint16_t buffer_size) {
  (void)_self;

  for(uint16_t i = 0; i < buffer_size; i++) {
    wait_for_tx_empty();
    SPI->DR = write_buffer ? write_buffer[i] : 0;

    if(read_buffer) {
      wait_for_rx_not_empty();
      read_buffer[i] = SPI->DR;
    }
  }
}

static const i_tiny_spi_api_t api = { transfer };

i_tiny_spi_t* spi_init(uint8_t cpol, uint8_t cpha, bool msb_first, spi_baud_t baud) {
  // Un-gate clock for SPI
  CLK->PCKENR1 |= (1 << CLK_PERIPHERAL_SPI);

  // Enable peripheral in master mode
  SPI->CR1 = SPI_CR1_SPE | SPI_CR1_MSTR | (baud << 3);

  // Software slave select mode
  SPI->CR2 = SPI_CR2_SSM | SPI_CR2_SSI;

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
