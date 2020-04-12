/*!
 * @file
 * @brief
 */

#ifndef tim2_pwm_h
#define tim2_pwm_h

#include "stm8s.h"
#include "hal/i_tiny_pwm.h"

#ifdef TIM2

i_tiny_pwm_t* tim2_pwm_channel1_init(void);
i_tiny_pwm_t* tim2_pwm_channel2_init(void);
i_tiny_pwm_t* tim2_pwm_channel3_init(void);

#endif

#endif
