/*!
 * @file
 * @brief
 */

#ifndef interrupts_h
#define interrupts_h

#include <stdint.h>
#include "stm8s.h"

#define interrupts_enable enableInterrupts
#define interrupts_disable disableInterrupts
#define interrupts_wait_for_interrupt wfi

uint8_t interrupts_save(void);
void interrupts_restore(uint8_t state);

#endif
