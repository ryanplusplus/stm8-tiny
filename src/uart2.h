/*!
 * @file
 * @brief
 */

#ifndef uart2_h
#define uart2_h

#include "stm8s.h"
#include "stm8s_itc.h"
#include "hal/i_tiny_uart.h"

#ifdef UART2

i_tiny_uart_t* uart2_init(void);

void uart2_send_complete_isr(void) __interrupt(ITC_IRQ_UART2_TX);
void uart2_receive_isr(void) __interrupt(ITC_IRQ_UART2_RX);

#endif

#endif
