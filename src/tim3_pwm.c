/*!
 * @file
 * @brief
 */

#include "stm8s_clk.h"
#include "stm8s_tim3.h"
#include "tim3_pwm.h"

#ifdef TIM3

enum {
  period = 1600
};

static i_tiny_pwm_t channel1, channel2, channel3;

static void set_duty_cycle_channel1(i_tiny_pwm_t* self, tiny_pwm_duty_cycle_t duty_cycle) {
  (void)self;

  uint16_t compare = ((uint32_t)period * duty_cycle) / UINT16_MAX;
  TIM3->CCR1H = compare >> 8;
  TIM3->CCR1L = compare & 0xFF;
}

static void set_duty_cycle_channel2(i_tiny_pwm_t* self, tiny_pwm_duty_cycle_t duty_cycle) {
  (void)self;

  uint16_t compare = ((uint32_t)period * duty_cycle) / UINT16_MAX;
  TIM3->CCR2H = compare >> 8;
  TIM3->CCR2L = compare & 0xFF;
}

static void set_frequency(i_tiny_pwm_t* self, tiny_pwm_frequency_hz_t frequency) {
  (void)self;
  (void)frequency;
  // Frequency is locked to 10 KHz
}

static const i_tiny_pwm_api_t channel1_api = { set_duty_cycle_channel1, set_frequency };
static const i_tiny_pwm_api_t channel2_api = { set_duty_cycle_channel2, set_frequency };

static void initialize_tim3(void) {
  // Un-gate clock for TIM3
  CLK->PCKENR1 |= (1 << CLK_PERIPHERAL_TIMER3);

  // Prescalar == 1, 16 MHz
  TIM3->PSCR = 0;

  // Set frequency via reload register
  // 1600 => 10KHz
  TIM3->ARRH = period >> 8;
  TIM3->ARRL = period & 0xFF;

  // Enable TIM3
  // Use shadow register for ARR in case we unlock frequency
  TIM3->CR1 = TIM3_CR1_CEN | TIM3_CR1_ARPE;
}

i_tiny_pwm_t* tim3_pwm_channel1_init(void) {
  initialize_tim3();

  // Default to 0% duty cycle
  TIM3->CCR1H = 0;
  TIM3->CCR1L = 0;

  // PWM mode 1
  // Use shadow register for CCMR
  TIM3->CCMR1 = TIM3_OCMODE_PWM1 | TIM3_CCMR_OCxPE;

  // Enable output for channel 1
  TIM3->CCER1 |= TIM3_CCER1_CC1E;

  channel1.api = &channel1_api;

  return &channel1;
}

i_tiny_pwm_t* tim3_pwm_channel2_init(void) {
  initialize_tim3();

  // Default to 0% duty cycle
  TIM3->CCR2H = 0;
  TIM3->CCR2L = 0;

  // PWM mode 1
  // Use shadow register for CCMR
  TIM3->CCMR2 = TIM3_OCMODE_PWM1 | TIM3_CCMR_OCxPE;

  // Enable output for channel 2
  TIM3->CCER1 |= TIM3_CCER1_CC2E;

  channel2.api = &channel2_api;

  return &channel2;
}

#endif
