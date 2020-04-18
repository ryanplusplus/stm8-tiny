/*!
 * @file
 * @brief
 */

#include <stddef.h>
#include "stm8s_clk.h"
#include "uart1.h"
#include "tiny_single_subscriber_event.h"

#ifdef UART1

static i_tiny_uart_t self;
static tiny_single_subscriber_event_t send_complete;
static tiny_single_subscriber_event_t receive;

void uart1_send_complete_isr(void) __interrupt(ITC_IRQ_UART1_TX) {
  // Disable TXE (transmit data register empty) interrupt
  UART1->CR2 &= ~UART1_CR2_TIEN;
  tiny_single_subscriber_event_publish(&send_complete, NULL);
}

void uart1_receive_isr(void) __interrupt(ITC_IRQ_UART1_RX) {
  volatile uint8_t dummy = UART1->SR;
  tiny_uart_on_receive_args_t args = { UART1->DR };
  tiny_single_subscriber_event_publish(&receive, &args);
}

void send(i_tiny_uart_t* _self, uint8_t byte) {
  (void)_self;

  UART1->DR = byte;

  // Enable TXE (transmit data register empty) interrupt
  UART1->CR2 |= UART1_CR2_TIEN;
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

i_tiny_uart_t* uart1_init(void) {
  // Un-gate clock for UART1
  CLK->PCKENR1 |= (1 << CLK_PERIPHERAL_UART1);

  // Configure 230.4k
  // 16,000,000 / UART_DIV = 230,400 => UART_DIV ~= 69 = 0x45
  UART1->BRR2 = 0x5;
  UART1->BRR1 = 0x4;

  // Enable TX, RX
  UART1->CR2 |= UART1_CR2_TEN | UART1_CR2_REN;

  // Enable RXNE (receive data register not empty) interrupt
  UART1->CR2 |= UART1_CR2_RIEN;

  self.api = &api;

  tiny_single_subscriber_event_init(&send_complete);
  tiny_single_subscriber_event_init(&receive);

  return &self;
}

#endif
