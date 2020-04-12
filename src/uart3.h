/*!
 * @file
 * @brief
 */

#ifndef uart3_h
#define uart3_h

#include "stm8s.h"
#include "stm8s_itc.h"
#include "hal/i_tiny_uart.h"

#ifdef UART3

i_tiny_uart_t* uart3_init(void);

void uart3_send_complete_isr(void) __interrupt(ITC_IRQ_UART3_TX);
void uart3_receive_isr(void) __interrupt(ITC_IRQ_UART3_RX);

#endif

#endif
