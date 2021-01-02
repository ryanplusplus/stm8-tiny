/*!
 * @file
 * @brief
 */

#include "stm8s.h"
#include "stm8s_clk.h"
#include "stm8s_tim2.h"
#include "tim2_pwm.h"

#ifdef TIM2

enum {
  period = 1600
};

static i_tiny_pwm_t channel1, channel2, channel3;

static void set_duty_cycle_channel1(i_tiny_pwm_t* self, tiny_pwm_duty_cycle_t duty_cycle)
{
  (void)self;

  uint16_t compare = ((uint32_t)period * duty_cycle) / UINT16_MAX;
  TIM2->CCR1H = compare >> 8;
  TIM2->CCR1L = compare & 0xFF;
}

static void set_duty_cycle_channel2(i_tiny_pwm_t* self, tiny_pwm_duty_cycle_t duty_cycle)
{
  (void)self;

  uint16_t compare = ((uint32_t)period * duty_cycle) / UINT16_MAX;
  TIM2->CCR2H = compare >> 8;
  TIM2->CCR2L = compare & 0xFF;
}

static void set_duty_cycle_channel3(i_tiny_pwm_t* self, tiny_pwm_duty_cycle_t duty_cycle)
{
  (void)self;

  uint16_t compare = ((uint32_t)period * duty_cycle) / UINT16_MAX;
  TIM2->CCR3H = compare >> 8;
  TIM2->CCR3L = compare & 0xFF;
}

static void set_frequency(i_tiny_pwm_t* self, tiny_pwm_frequency_hz_t frequency)
{
  (void)self;
  (void)frequency;
  // Frequency is locked to 10 KHz
}

static const i_tiny_pwm_api_t channel1_api = { set_duty_cycle_channel1, set_frequency };
static const i_tiny_pwm_api_t channel2_api = { set_duty_cycle_channel2, set_frequency };
static const i_tiny_pwm_api_t channel3_api = { set_duty_cycle_channel3, set_frequency };

static void initialize_tim2(void)
{
  // Un-gate clock for TIM2
  CLK->PCKENR1 |= (1 << CLK_PERIPHERAL_TIMER2);

  // Prescalar == 1, 16 MHz
  TIM2->PSCR = 0;

  // Set frequency via reload register
  // 1600 => 10KHz
  TIM2->ARRH = period >> 8;
  TIM2->ARRL = period & 0xFF;

  // Enable TIM2
  // Use shadow register for ARR in case we unlock frequency
  TIM2->CR1 = TIM2_CR1_CEN | TIM2_CR1_ARPE;
}

i_tiny_pwm_t* tim2_pwm_channel1_init(void)
{
  initialize_tim2();

  // Default to 0% duty cycle
  TIM2->CCR1H = 0;
  TIM2->CCR1L = 0;

  // PWM mode 1
  // Use shadow register for CCMR
  TIM2->CCMR1 = TIM2_OCMODE_PWM1 | TIM2_CCMR_OCxPE;

  // Enable output for channel 1
  TIM2->CCER1 |= TIM2_CCER1_CC1E;

  channel1.api = &channel1_api;

  return &channel1;
}

i_tiny_pwm_t* tim2_pwm_channel2_init(void)
{
  initialize_tim2();

  // Default to 0% duty cycle
  TIM2->CCR2H = 0;
  TIM2->CCR2L = 0;

  // PWM mode 1
  // Use shadow register for CCMR
  TIM2->CCMR2 = TIM2_OCMODE_PWM1 | TIM2_CCMR_OCxPE;

  // Enable output for channel 2
  TIM2->CCER1 |= TIM2_CCER1_CC2E;

  channel2.api = &channel2_api;

  return &channel2;
}

i_tiny_pwm_t* tim2_pwm_channel3_init(void)
{
  initialize_tim2();

  // Default to 0% duty cycle
  TIM2->CCR3H = 0;
  TIM2->CCR3L = 0;

  // PWM mode 1
  // Use shadow register for CCMR
  TIM2->CCMR3 = TIM2_OCMODE_PWM1 | TIM2_CCMR_OCxPE;

  // Enable output for channel 3
  TIM2->CCER2 |= TIM2_CCER2_CC3E;

  channel3.api = &channel3_api;

  return &channel3;
}

#endif
