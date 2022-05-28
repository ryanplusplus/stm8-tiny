/*!
 * @file
 * @brief
 */

#include "stm8s.h"
#include "watchdog.h"

enum {
  kick_period_msec = 25
};

enum {
  unlock = 0x55,
  start = 0xCC,
  refresh = 0xAA
};

static tiny_timer_t timer;

static void kick(void* context)
{
  (void)context;
  IWDG->KR = refresh;
}

void watchdog_init(tiny_timer_group_t* timer_group)
{
  // Start the watchdog and unlock the IWDG registers
  IWDG->KR = start;
  IWDG->KR = unlock;

  // T = 2 * Tlsi * P * R
  // T = timeout
  // Tlsi = 1 / Flsi
  // Flsi = 128 KHz
  // P = 2 ^ (PR[2:0] + 2)
  // R = RLR[7:0] + 1
  //
  // PR[2:0] = 3, RLR[7:0] = 0xFF => ~127 msec
  IWDG->PR = 0x3;
  IWDG->RLR = 0xFF;

  // Lock the IWDG registers and actually start counting
  IWDG->RLR = refresh;

  tiny_timer_start_periodic(timer_group, &timer, kick_period_msec, NULL, kick);
}
