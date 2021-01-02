/*!
 * @file
 * @brief
 */

#include "stm8s.h"
#include "clock.h"

void clock_init(void)
{
  // No divisor, run at 16 MHz
  CLK->CKDIVR = 0;

  // Gate all peripheral clocks by default
  CLK->PCKENR1 = 0;
  CLK->PCKENR2 = 0;

  // Wait for clock to stabilize
  while(!(CLK->ICKR & CLK_ICKR_HSIRDY)) {
  }
}
