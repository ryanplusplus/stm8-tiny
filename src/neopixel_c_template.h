/*!
 * @file
 * @brief
 */

#include "stm8s.h"
#include "interrupts.h"
#include "neopixel_h_template.h"

#ifndef neopixel_port
#error "neopixel_port must be defined"
#endif

#ifndef neopixel_pin
#error "neopixel_pin must be defined"
#endif

#define port_address neopixel_concat(GPIO, neopixel_concat(neopixel_port, _BaseAddress))
#define pin #neopixel_pin

static void send_byte(uint8_t byte)
{
  (void)byte;

  // clang-format off
  __asm
    send_byte$:
      ld    a, (0x03, sp)   ; a <- byte
      ldw   y, #8

    send_bit$:
      bset  port_address, pin
      sll   a               ; roll left, setting carry if msb(a) == 1
      jrc   send_1$         ; if carry not set

    send_0$:
      nop
      nop
      nop
      nop
      bres  port_address, pin
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      decw  y
      jrne  send_bit$
      jra   done$

    send_1$:
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      bres  port_address, pin
      nop
      nop
      decw  y
      jrne  send_bit$

    done$:
  __endasm;
  // clang-format on
}

void neopixel_concat(neopixel_api_name, _init)(void)
{
  neopixel_concat(GPIO, neopixel_port)->CR1 |= (1 << pin);
  neopixel_concat(GPIO, neopixel_port)->DDR |= (1 << pin);
}

void neopixel_concat(neopixel_api_name, _write)(const neopixel_concat(neopixel_api_name, _color_t) * data, uint16_t count)
{
  uint8_t state = interrupts_save();

  for(uint16_t i = 0; i < count; i++) {
    for(uint8_t j = 0; j < sizeof(data[0]); j++) {
      send_byte(((const uint8_t*)data)[i * sizeof(data[0]) + j]);
    }
  }

  interrupts_restore(state);
}

void neopixel_concat(neopixel_api_name, _write_all)(const neopixel_concat(neopixel_api_name, _color_t) * data, uint16_t count)
{
  uint8_t state = interrupts_save();

  for(uint16_t i = 0; i < count; i++) {
    for(uint8_t j = 0; j < sizeof(data[0]); j++) {
      send_byte(((const uint8_t*)data)[j]);
    }
  }

  interrupts_restore(state);
}

void neopixel_concat(neopixel_api_name, _reset)(void)
{
  // clang-format off
  __asm
    reset_neopixel$:
      ldw   y, #400

    loop$:
      decw  y
      jrne  loop$
  __endasm;
  // clang-format on
}
