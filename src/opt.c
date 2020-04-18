/*!
 * @file
 * @brief
 */

#include "stm8s_flash.h"
#include "opt.h"

static inline void unlock_opt(void) {
  FLASH->DUKR = FLASH_RASS_KEY2;
  FLASH->DUKR = FLASH_RASS_KEY1;
  while(!(FLASH->IAPSR & FLASH_FLAG_DUL)) {
  }
  FLASH->CR2 |= FLASH_CR2_OPT;
  FLASH->NCR2 &= ~FLASH_NCR2_NOPT;
}

static inline void lock_opt(void) {
  FLASH->IAPSR &= ~FLASH_FLAG_DUL;
}

static volatile uint8_t* address(uint8_t which) {
  if(which == 0) {
    return &OPT->OPT0;
  }
  else {
    return &OPT->OPT1 + (which - 1) * 2;
  }
}

static inline void write(uint8_t which, uint8_t value) {
  *address(which) = value;
  *(address(which) + 1) = ~value;

  while(!(FLASH->IAPSR & FLASH_FLAG_EOP)) {
  }
}

uint8_t opt_read(uint8_t which) {
  return *address(which);
}

void opt_write(uint8_t which, uint8_t value) {
  unlock_opt();
  write(which, value);
  lock_opt();
}
