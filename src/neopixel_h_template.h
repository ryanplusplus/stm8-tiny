/*!
 * @file
 * @brief
 */

#include <stdint.h>

#define neopixel_byte_order_grb -1
#define neopixel_byte_order_rgb -2
#define neopixel_byte_order_rgbw -3

#ifndef neopixel_api_name
#error "neopixel_api_name must be defined"
#endif

#ifndef neopixel_byte_order
#error "neopixel_byte_order must be defined"
#endif

#define _neopixel_concat(x, y) x##y
#define neopixel_concat(x, y) _neopixel_concat(x, y)

#if neopixel_byte_order == neopixel_byte_order_grb
typedef struct {
  uint8_t g;
  uint8_t r;
  uint8_t b;
} neopixel_concat(neopixel_api_name, _color_t);
#elif neopixel_byte_order == neopixel_byte_order_rgb
typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} neopixel_concat(neopixel_api_name, _color_t);
#elif neopixel_byte_order == neopixel_byte_order_rgbw
typedef struct {
  uint8_t g;
  uint8_t r;
  uint8_t b;
  uint8_t w;
} neopixel_concat(neopixel_api_name, _color_t);
#else
#error "neopixel_byte_order must be neopixel_byte_order_grb, neopixel_byte_order_rgb, or neopixel_byte_order_rgbw"
#endif

void neopixel_concat(neopixel_api_name, _init)(void);
void neopixel_concat(neopixel_api_name, _write)(const neopixel_concat(neopixel_api_name, _color_t) * data, uint16_t count);
void neopixel_concat(neopixel_api_name, _write_all)(const neopixel_concat(neopixel_api_name, _color_t) * data, uint16_t count);
void neopixel_concat(neopixel_api_name, _reset)(void);
