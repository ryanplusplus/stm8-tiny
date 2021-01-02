/*!
 * @file
 * @brief
 */

#include "stm8s_flash.h"
#include "eeprom.h"

enum {
  size = FLASH_DATA_END_PHYSICAL_ADDRESS - FLASH_DATA_START_PHYSICAL_ADDRESS + 1
};

static inline void unlock_eeprom(void)
{
  FLASH->DUKR = FLASH_RASS_KEY2;
  FLASH->DUKR = FLASH_RASS_KEY1;
  while(!(FLASH->IAPSR & FLASH_FLAG_DUL)) {
  }
}

static inline void lock_eeprom(void)
{
  FLASH->IAPSR &= ~FLASH_FLAG_DUL;
}

static inline void write(uint16_t offset, const void* buffer, uint16_t buffer_size)
{
  for(uint16_t i = 0; i < buffer_size; i++) {
    *((uint8_t*)FLASH_DATA_START_PHYSICAL_ADDRESS + offset + i) = ((const uint8_t*)buffer)[i];
  }

  while(!(FLASH->IAPSR & FLASH_FLAG_EOP)) {
  }
}

uint16_t eeprom_size(void)
{
  return size;
}

const void* eeprom_read(uint16_t offset)
{
  return (const uint8_t*)FLASH_DATA_START_PHYSICAL_ADDRESS + offset;
}

void eeprom_write(uint16_t offset, const void* buffer, uint16_t buffer_size)
{
  unlock_eeprom();
  write(offset, buffer, buffer_size);
  lock_eeprom();
}
