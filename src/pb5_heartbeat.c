/*!
 * @file
 * @brief
 */

#include "stm8s.h"
#include <stddef.h>
#include <stdbool.h>
#include "pb5_heartbeat.h"

enum {
  half_period_in_msec = 500,
  pin_5 = (1 << 5)
};

static tiny_timer_t timer;

static void blink(void* context)
{
  (void)context;
  GPIOB->ODR ^= pin_5;
}

void pb5_heartbeat_init(tiny_timer_group_t* timer_group)
{
  // Configure push pull output
  GPIOB->CR1 |= pin_5;
  GPIOB->DDR |= pin_5;

  tiny_timer_start_periodic(timer_group, &timer, half_period_in_msec, NULL, blink);
}
