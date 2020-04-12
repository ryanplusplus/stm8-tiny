/*!
 * @file
 * @brief
 */

#ifndef adc2_h
#define adc2_h

#include "stm8s.h"
#include "hal/i_tiny_analog_input_group.h"

#ifdef ADC2

i_tiny_analog_input_group_t* adc2_init(void);

#endif

#endif
