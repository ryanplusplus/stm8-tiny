/*!
 * @file
 * @brief
 */

#include "stm8s.h"
#include "rst.h"

rst_reset_reason_t rst_reset_reason(void)
{
  rst_reset_reason_t reason = 0;
  uint8_t status = RST->SR;

  while(status) {
    status >>= 1;
    reason++;
  }

  return reason;
}
