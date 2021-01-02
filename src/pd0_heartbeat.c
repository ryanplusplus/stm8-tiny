/*!
 * @file
 * @brief
 */

#include "stm8s.h"
#include <stddef.h>
#include <stdbool.h>
#include "pd0_heartbeat.h"

enum {
  half_period_in_msec = 500,
  pin_0 = (1 << 0)
};

static tiny_timer_t timer;

static void blink(tiny_timer_group_t* group, void* context)
{
  (void)context;
  GPIOD->ODR ^= pin_0;
  tiny_timer_start(group, &timer, half_period_in_msec, blink, NULL);
}

void pd0_heartbeat_init(tiny_timer_group_t* timer_group)
{
  // Configure push pull output
  GPIOD->CR1 |= pin_0;
  GPIOD->DDR |= pin_0;

  tiny_timer_start(timer_group, &timer, half_period_in_msec, blink, NULL);
}
