/*!
 * @file
 * @brief
 */

#include "stm8s.h"
#include <stddef.h>
#include <stdbool.h>
#include "pe5_heartbeat.h"

enum {
  half_period_in_msec = 500,
  pin_5 = (1 << 5)
};

static tiny_timer_t timer;

static void blink(tiny_timer_group_t* group, void* context)
{
  (void)group;
  (void)context;
  GPIOE->ODR ^= pin_5;
}

void pe5_heartbeat_init(tiny_timer_group_t* timer_group)
{
  // Configure push pull output
  GPIOE->CR1 |= pin_5;
  GPIOE->DDR |= pin_5;

  tiny_timer_start_periodic(timer_group, &timer, half_period_in_msec, blink, NULL);
}
