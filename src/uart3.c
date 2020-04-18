/*!
 * @file
 * @brief
 */

#include <stddef.h>
#include "stm8s_clk.h"
#include "uart3.h"
#include "tiny_single_subscriber_event.h"

#ifdef UART3

static i_tiny_uart_t self;
static tiny_single_subscriber_event_t send_complete;
static tiny_single_subscriber_event_t receive;

void uart3_send_complete_isr(void) __interrupt(ITC_IRQ_UART3_TX) {
  // Disable TXE (transmit data register empty) interrupt
  UART3->CR2 &= ~UART3_CR2_TIEN;
  tiny_single_subscriber_event_publish(&send_complete, NULL);
}

void uart3_receive_isr(void) __interrupt(ITC_IRQ_UART3_RX) {
  volatile uint8_t dummy = UART3->SR;
  tiny_uart_on_receive_args_t args = { UART3->DR };
  tiny_single_subscriber_event_publish(&receive, &args);
}

void send(i_tiny_uart_t* _self, uint8_t byte) {
  (void)_self;

  UART3->DR = byte;

  // Enable TXE (transmit data register empty) interrupt
  UART3->CR2 |= UART3_CR2_TIEN;
}

i_tiny_event_t* on_send_complete(i_tiny_uart_t* _self) {
  (void)_self;
  return &send_complete.interface;
}

i_tiny_event_t* on_receive(i_tiny_uart_t* _self) {
  (void)_self;
  return &receive.interface;
}

static const i_tiny_uart_api_t api = { send, on_send_complete, on_receive };

i_tiny_uart_t* uart3_init(void) {
  // Un-gate clock for UART3
  CLK->PCKENR1 |= (1 << CLK_PERIPHERAL_UART3);

  // Configure 230.4k
  // 16,000,000 / UART_DIV = 230,400 => UART_DIV ~= 69 = 0x45
  UART3->BRR2 = 0x5;
  UART3->BRR1 = 0x4;

  // Enable TX, RX
  UART3->CR2 |= UART3_CR2_TEN | UART3_CR2_REN;

  // Enable RXNE (receive data register not empty) interrupt
  UART3->CR2 |= UART3_CR2_RIEN;

  self.api = &api;

  tiny_single_subscriber_event_init(&send_complete);
  tiny_single_subscriber_event_init(&receive);

  return &self;
}

#endif
