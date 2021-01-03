/*!
 * @file
 * @brief
 */

#include "stm8s.h"
#include "neopixel_h_template.h"

#ifndef neopixel_port
#error "neopixel_port must be defined"
#endif

#ifndef neopixel_pin
#error "neopixel_pin must be defined"
#endif

#define port neopixel_concat(GPIO, neopixel_concat(neopixel_port, _BaseAddress))
#define pin #neopixel_pin

static void send_byte(uint8_t byte)
{
  (void)byte;

  // clang-format off
  __asm
    disable_interrupts$:
      ld    a, #0
      jrnm  save_interrupt_state$
      ld    a, #1
    save_interrupt_state$:
      push  a
      sim

    send_byte$:
      ld    a, (0x04, sp)   ; a <- byte
      ldw   y, #8

    send_bit$:
      bset  port, pin
      sll   a               ; roll left, setting carry if msb(a) == 1
      jrc   send_1$         ; if carry not set

    send_0$:
      nop
      nop
      nop
      nop
      bres  port, pin
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
      bres  port, pin
      nop
      nop
      decw  y
      jrne  send_bit$

    done$:
    restore_interrupt_state$:
      pop a
      tnz a
      jreq exit$
      rim

    exit$:
  __endasm;
  // clang-format on
}

void neopixel_concat(neopixel_api_name, _write)(const neopixel_concat(neopixel_api_name, _color_t) * data, uint16_t count)
{
  for(uint16_t i = 0; i < count; i++) {
    for(uint8_t j = 0; j < sizeof(data[0]); j++) {
      send_byte(((const uint8_t*)data)[i * sizeof(data[0]) + j]);
    }
  }
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
