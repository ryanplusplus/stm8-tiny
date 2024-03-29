/*!
 * @file
 * @brief
 */

#include "stm8s.h"
#include <stddef.h>
#include <stdbool.h>
#include "pa3_heartbeat.h"

enum {
  half_period_in_msec = 500,
  pin_3 = (1 << 3)
};

static tiny_timer_t timer;

static void blink(void* context)
{
  (void)context;
  GPIOA->ODR ^= pin_3;
}

void pa3_heartbeat_init(tiny_timer_group_t* timer_group)
{
  // Configure push pull output
  GPIOA->CR1 |= pin_3;
  GPIOA->DDR |= pin_3;

  tiny_timer_start_periodic(timer_group, &timer, half_period_in_msec, NULL, blink);
}
