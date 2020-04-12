/*!
 * @file
 * @brief
 */

#ifndef adc1_h
#define adc1_h

#include "stm8s.h"
#include "hal/i_tiny_analog_input_group.h"

#ifdef ADC1

i_tiny_analog_input_group_t* adc1_init(void);

#endif

#endif
