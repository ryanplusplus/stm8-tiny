/*!
 * @file
 * @brief
 */

#ifndef tim3_pwm_h
#define tim3_pwm_h

#include "stm8s.h"
#include "hal/i_tiny_pwm.h"

#ifdef TIM3

i_tiny_pwm_t* tim3_pwm_channel1_init(void);
i_tiny_pwm_t* tim3_pwm_channel2_init(void);

#endif

#endif
