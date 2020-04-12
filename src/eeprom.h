/*!
 * @file
 * @brief
 */

#ifndef eeprom_h
#define eeprom_h

#include <stdint.h>

uint16_t eeprom_size(void);
const void* eeprom_read(uint16_t offset);
void eeprom_write(uint16_t offset, const void* buffer, uint16_t buffer_size);

#endif
