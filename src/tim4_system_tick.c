/*!
 * @file
 * @brief
 */

#include "tim4_system_tick.h"
#include "stm8s_clk.h"

#ifdef TIM4

static i_tiny_time_source_t self;
static volatile tiny_time_source_ticks_t ticks;

void tim4_system_tick_isr(void) __interrupt(ITC_IRQ_TIM4_OVF)
{
  // Clear interrupt flag
  TIM4->SR1 &= ~TIM4_SR1_UIF;
  ticks++;
}

static tiny_time_source_ticks_t _ticks(i_tiny_time_source_t* self) __critical
{
  (void)self;
  return ticks;
}

static const i_tiny_time_source_api_t api = { _ticks };

i_tiny_time_source_t* tim4_system_tick_init(void)
{
  // Un-gate clock for TIM4
  CLK->PCKENR1 |= (1 << CLK_PERIPHERAL_TIMER4);

  // 16,000,000 / 2 ^ $PSCR / ($ARR + 1) = 1000
  // => $PSCR = 6
  // => $ARR = 249
  TIM4->PSCR = 6;
  TIM4->ARR = 249;

  // Enable interrupt
  TIM4->IER |= TIM4_IER_UIE;

  // Enable counter
  TIM4->CR1 |= TIM4_CR1_CEN;

  self.api = &api;

  return &self;
}

#endif
