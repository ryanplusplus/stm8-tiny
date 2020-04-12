/*!
 * @file
 * @brief
 */

#ifndef rst_h
#define rst_h

#include <stdint.h>

enum {
  rst_reset_reason_power_on_reset,
  rst_reset_reason_emc,
  rst_reset_reason_swim,
  rst_reset_reason_illegal_opcode,
  rst_reset_reason_independent_watchdog,
  rst_reset_reason_windowed_watchdog
};
typedef uint8_t rst_reset_reason_t;

rst_reset_reason_t rst_reset_reason(void);

#endif
