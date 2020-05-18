/*!
 * @file
 * @brief
 */

#include "i2c.h"
#include "tiny_utils.h"

static i_tiny_i2c_t self;
static i_tiny_async_i2c_t* async_i2c;

typedef struct {
  volatile bool done;
  volatile bool success;
} context_t;

static void callback(void* _context, bool success) {
  reinterpret(context, _context, context_t*);
  context->success = success;
  context->done = true;
}

static bool write(
  i_tiny_i2c_t* self,
  uint8_t address,
  bool prepare_for_restart,
  const uint8_t* buffer,
  uint16_t buffer_size) {
  (void)self;
  context_t context = { false };

  tiny_async_i2c_write(
    async_i2c,
    address,
    prepare_for_restart,
    buffer,
    buffer_size,
    callback,
    &context);

  while(!context.done) {
  }

  return context.success;
}

static bool read(
  i_tiny_i2c_t* self,
  uint8_t address,
  bool prepare_for_restart,
  uint8_t* buffer,
  uint16_t buffer_size) {
  (void)self;
  context_t context = { false };

  tiny_async_i2c_read(
    async_i2c,
    address,
    prepare_for_restart,
    buffer,
    buffer_size,
    callback,
    &context);

  while(!context.done) {
  }

  return context.success;
}

static void reset(i_tiny_i2c_t* self) {
  (void)self;
  tiny_async_i2c_reset(async_i2c);
}

static const i_tiny_i2c_api_t api = { write, read, reset };

i_tiny_i2c_t* i2c_init(void) {
  async_i2c = async_i2c_init();
  self.api = &api;
  return &self;
}
