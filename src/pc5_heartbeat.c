/*!
 * @file
 * @brief
 */

#include "stm8s.h"
#include <stddef.h>
#include <stdbool.h>
#include "pc5_heartbeat.h"

enum {
  half_period_in_msec = 500,
  pin_5 = (1 << 5)
};

static tiny_timer_t timer;

static void blink(void* context)
{
  (void)context;
  GPIOC->ODR ^= pin_5;
}

void pc5_heartbeat_init(tiny_timer_group_t* timer_group)
{
  // Configure push pull output
  GPIOC->CR1 |= pin_5;
  GPIOC->DDR |= pin_5;

  tiny_timer_start_periodic(timer_group, &timer, half_period_in_msec, NULL, blink);
}
