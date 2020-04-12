/*!
 * @file
 * @brief
 */

#ifndef opt_h
#define opt_h

#include <stdint.h>

uint8_t opt_read(uint8_t which);
void opt_write(uint8_t which, uint8_t value);

#endif
