/*!
 * @file
 * @brief
 *
 * Good references:
 * - https://lujji.github.io/blog/bare-metal-programming-stm8/#I2C
 * - https://blog.mark-stevens.co.uk/2015/05/stm8s-i2c-master-devices/
 */

#include <stddef.h>
#include "stm8s_clk.h"
#include "stm8s_i2c.h"
#include "async_i2c.h"

#ifdef I2C

enum {
  mode_write = I2C_DIRECTION_TX,
  mode_write_with_restart = 0x10 + I2C_DIRECTION_TX,
  mode_read = I2C_DIRECTION_RX,
  mode_read_with_restart = 0x10 + I2C_DIRECTION_RX,
};
typedef uint8_t mode_t;

static i_tiny_async_i2c_t self;
static mode_t mode;
static uint8_t address;
static union {
  const uint8_t* write;
  uint8_t* read;
} buffer;
static uint16_t buffer_size;
static uint16_t buffer_offset;
static tiny_async_i2c_callback_t callback;
static void* context;

static void reset(i_tiny_async_i2c_t* _self);

static void finish(void)
{
  // Disable all interrupts
  I2C->ITR = 0;
  callback(context, true);
}

void async_i2c_isr(void) __interrupt(ITC_IRQ_I2C)
{
  volatile uint8_t dummy;

  // In the case of a restart, TXE and BTF will still be set until start is sent
  // so we can't really trust SR1 while start is pending
  if(I2C->CR2 & I2C_CR2_START) {
    return;
  }

  // Start condition generated
  if(I2C->SR1 & I2C_SR1_SB) {
    // Clear start condition by reading SR1
    dummy = I2C->SR1;

    // Send the slave address and R/W bit
    I2C->DR = (address << 1) | (mode & 0x01);

    return;
  }

  // Address sent
  if(I2C->SR1 & I2C_SR1_ADDR) {
    if(mode == mode_read) {
      if(buffer_size == 1) {
        I2C->CR2 &= ~I2C_CR2_ACK;

        // Clear address sent event by reading SR1 and then SR3
        dummy = I2C->SR1;
        dummy = I2C->SR3;

        I2C->CR2 |= I2C_CR2_STOP;
      }
      else if(buffer_size == 2) {
        // Clear address sent event by reading SR1 and then SR3
        dummy = I2C->SR1;
        dummy = I2C->SR3;

        I2C->CR2 |= I2C_CR2_POS;
        I2C->CR2 &= ~I2C_CR2_ACK;
      }
      else {
        // Clear address sent event by reading SR1 and then SR3
        dummy = I2C->SR1;
        dummy = I2C->SR3;
      }
    }
    else {
      // Clear address sent event by reading SR1 and then SR3
      dummy = I2C->SR1;
      dummy = I2C->SR3;
    }

    return;
  }

  // Transmit buffer is empty
  if(I2C->SR1 & I2C_SR1_TXE) {
    if(buffer_offset < buffer_size) {
      I2C->DR = buffer.write[buffer_offset++];
    }
    else {
      if(mode == mode_write) {
        I2C->CR2 = I2C_CR2_STOP;
      }

      finish();
    }

    return;
  }

  // Byte transfer finished
  if(I2C->SR1 & I2C_SR1_BTF) {
    if(buffer_size == 2) {
      I2C->CR2 |= I2C_CR2_STOP;

      buffer.read[buffer_offset++] = I2C->DR;
      buffer.read[buffer_offset++] = I2C->DR;

      finish();
      return;
    }
    else if(buffer_size > 2) {
      if((buffer_size - buffer_offset) > 3) {
        buffer.read[buffer_offset++] = I2C->DR;
        return;
      }
      else if((buffer_size - buffer_offset) == 3) {
        // Re-enable buffer interrupts so that we can receive the last byte using
        // RXNE
        I2C->ITR |= I2C_ITR_ITBUFEN;

        I2C->CR2 &= ~I2C_CR2_ACK;

        // Timing is sensitive here so read into temporaries
        volatile uint8_t byte1 = I2C->DR;
        I2C->CR2 |= I2C_CR2_STOP;
        volatile uint8_t byte2 = I2C->DR;

        buffer.read[buffer_offset++] = byte1;
        buffer.read[buffer_offset++] = byte2;

        return;
      }
    }
  }

  // Receive buffer is not empty
  if(I2C->SR1 & I2C_SR1_RXNE) {
    if(buffer_size == 1) {
      buffer.read[0] = I2C->DR;
      finish();
      return;
    }
    else if(buffer_size == 2) {
      return;
    }
    else if((buffer_size - buffer_offset) == 1) {
      buffer.read[buffer_offset++] = I2C->DR;
      finish();
      return;
    }
  }

  // If we're still here something is wrong so let's reset and tell the client
  reset(NULL);
  callback(context, false);
}

static void wait_for_stop_condition_to_be_sent(void)
{
  while((I2C->CR2 & I2C_CR2_STOP) && (I2C->SR3 & I2C_SR3_MSL)) {
  }
}

static void write(
  i_tiny_async_i2c_t* _self,
  uint8_t _address,
  bool _prepare_for_restart,
  const uint8_t* _buffer,
  uint16_t _buffer_size,
  tiny_async_i2c_callback_t _callback,
  void* _context)
{
  (void)_self;

  address = _address;
  buffer.write = _buffer;
  buffer_size = _buffer_size;
  buffer_offset = 0;
  mode = _prepare_for_restart ? mode_write_with_restart : mode_write;
  callback = _callback;
  context = _context;

  wait_for_stop_condition_to_be_sent();

  I2C->CR2 = I2C_CR2_START;

  I2C->ITR = I2C_ITR_ITBUFEN | I2C_ITR_ITEVTEN | I2C_ITR_ITERREN;
}

static void read(
  i_tiny_async_i2c_t* _self,
  uint8_t _address,
  bool _prepare_for_restart,
  uint8_t* _buffer,
  uint16_t _buffer_size,
  tiny_async_i2c_callback_t _callback,
  void* _context)
{
  (void)_self;

  address = _address;
  buffer.read = _buffer;
  buffer_size = _buffer_size;
  buffer_offset = 0;
  mode = _prepare_for_restart ? mode_read_with_restart : mode_read;
  callback = _callback;
  context = _context;

  wait_for_stop_condition_to_be_sent();

  I2C->CR2 = I2C_CR2_START | I2C_CR2_ACK;

  // We use BTF when reading 2+ bytes
  if(buffer_size > 2) {
    I2C->ITR = I2C_ITR_ITEVTEN | I2C_ITR_ITERREN;
  }
  else {
    I2C->ITR = I2C_ITR_ITBUFEN | I2C_ITR_ITEVTEN | I2C_ITR_ITERREN;
  }
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

static void reset(i_tiny_async_i2c_t* _self)
{
  (void)_self;
  I2C->CR2 = I2C_CR2_SWRST;
  configure_peripheral();
}

static const i_tiny_async_i2c_api_t api = { write, read, reset };

i_tiny_async_i2c_t* async_i2c_init(void)
{
  reset(NULL);
  self.api = &api;
  return &self;
}

#endif
