/*!
 * @file
 * @brief
 */

#ifndef async_i2c_h
#define async_i2c_h

#include "stm8s.h"
#include "stm8s_itc.h"
#include "hal/i_tiny_async_i2c.h"

#ifdef I2C

i_tiny_async_i2c_t* async_i2c_init(void);

void async_i2c_isr(void) __interrupt(ITC_IRQ_I2C);

#endif

#endif
