/*!
 * @file
 * @brief
 */

#include "stm8s.h"
#include <stddef.h>
#include <stdbool.h>
#include "pc2_heartbeat.h"

enum {
  half_period_in_msec = 500,
  pin_2 = (1 << 2)
};

static tiny_timer_t timer;

static void blink(tiny_timer_group_t* group, void* context)
{
  (void)context;
  GPIOC->ODR ^= pin_2;
  tiny_timer_start(group, &timer, half_period_in_msec, blink, NULL);
}

void pc2_heartbeat_init(tiny_timer_group_t* timer_group)
{
  // Configure push pull output
  GPIOC->CR1 |= pin_2;
  GPIOC->DDR |= pin_2;

  blink(timer_group, NULL);
}
