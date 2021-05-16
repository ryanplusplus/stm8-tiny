/*!
 * @file
 * @brief
 */

#include "stm8s_clk.h"
#include "stm8s_i2c.h"
#include "i2c.h"
#include "tiny_utils.h"

#ifdef I2C

enum {
  mode_write = I2C_DIRECTION_TX,
  mode_read = I2C_DIRECTION_RX
};
typedef uint8_t mode_t;

static i_tiny_i2c_t self;

static inline void send_start_and_prepare_to_ack(void)
{
  I2C->CR2 = I2C_CR2_START | I2C_CR2_ACK;
  while(!(I2C->SR1 & I2C_SR1_SB)) {
  }
}

static inline void prepare_to_nack(void)
{
  I2C->CR2 &= ~I2C_CR2_ACK;
}

static inline void send_address(uint8_t address, mode_t mode)
{
  I2C->DR = (address << 1) | (mode & 0x01);
  while(!(I2C->SR1 & I2C_SR1_ADDR)) {
  }
  (void)I2C->SR3; // Clear address sent condition
}

static inline uint8_t receive_byte(void)
{
  while(!(I2C->SR1 & I2C_SR1_RXNE)) {
  }
  return I2C->DR;
}

static inline void write_byte(uint8_t byte)
{
  I2C->DR = byte;
  while(!(I2C->SR1 & I2C_SR1_TXE)) {
  }
}

static inline void send_stop(void)
{
  I2C->CR2 |= I2C_CR2_STOP;
  while(I2C->SR3 & I2C_SR3_MSL) {
  }
}

static inline void queue_stop(void)
{
  send_stop();
}

static bool write(
  i_tiny_i2c_t* self,
  uint8_t address,
  bool prepare_for_restart,
  const void* _buffer,
  uint16_t buffer_size)
{
  reinterpret(buffer, _buffer, const uint8_t*);
  (void)self;

  send_start_and_prepare_to_ack();
  send_address(address, mode_write);

  while(buffer_size--) {
    write_byte(*buffer++);
  }

  if(!prepare_for_restart) {
    send_stop();
  }

  return true;
}

static bool read(
  i_tiny_i2c_t* self,
  uint8_t address,
  bool prepare_for_restart,
  void* _buffer,
  uint16_t buffer_size)
{
  reinterpret(buffer, _buffer, uint8_t*);
  (void)self;

  send_start_and_prepare_to_ack();
  send_address(address, mode_read);

  while(buffer_size-- > 1) {
    *(buffer++) = receive_byte();
  }

  prepare_to_nack();

  if(!prepare_for_restart) {
    queue_stop();
  }

  *buffer = receive_byte();

  return true;
}

static void configure_peripheral(void)
{
  // Disable peripheral
  I2C->CR1 = 0;

  // Clear software reset
  I2C->CR2 = 0;

  // Un-gate clock for I2C
  CLK->PCKENR1 |= (1 << CLK_PERIPHERAL_I2C);

  // Set peripheral clock frequency to 16 MHz
  I2C->FREQR = 16;

  // Standard mode
  // SCL frequency = 1 / (2 * CCR * tMASTER) = 1 / (2 * 0x50 * 1/16,000,000) = 100 kHz
  I2C->CCRH = 0;
  I2C->CCRL = 0x50;

  // 7-bit addressing
  I2C->OARH = I2C_OARH_ADDCONF;

  // Maximum SCL rise time
  // In standard mode, maximum rise time is 1000 ns
  // The peripheral clock period is 62.5 ns (16 MHz)
  // 1000 / 62.5 = 16 => 16 + 1 = 17
  I2C->TRISER = 17;

  // Disable all interrupts
  I2C->ITR = 0;

  // Enable peripheral
  I2C->CR1 = I2C_CR1_PE;
}

static void reset(i_tiny_i2c_t* _self)
{
  (void)_self;
  I2C->CR2 = I2C_CR2_SWRST;
  configure_peripheral();
}

static const i_tiny_i2c_api_t api = { write, read, reset };

i_tiny_i2c_t* i2c_init(void)
{
  reset(NULL);
  self.api = &api;
  return &self;
}

#endif
