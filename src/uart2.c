/*!
 * @file
 * @brief
 */

#include <stddef.h>
#include "stm8s_clk.h"
#include "uart2.h"
#include "tiny_single_subscriber_event.h"

#ifdef UART2

static i_tiny_uart_t self;
static tiny_single_subscriber_event_t send_complete;
static tiny_single_subscriber_event_t receive;

void uart2_send_complete_isr(void) __interrupt(ITC_IRQ_UART2_TX)
{
  // Disable TXE (transmit data register empty) interrupt
  UART2->CR2 &= ~UART2_CR2_TIEN;
  tiny_single_subscriber_event_publish(&send_complete, NULL);
}

void uart2_receive_isr(void) __interrupt(ITC_IRQ_UART2_RX)
{
  volatile uint8_t dummy = UART2->SR;
  tiny_uart_on_receive_args_t args = { UART2->DR };
  tiny_single_subscriber_event_publish(&receive, &args);
}

static void send(i_tiny_uart_t* _self, uint8_t byte)
{
  (void)_self;

  UART2->DR = byte;

  // Enable TXE (transmit data register empty) interrupt
  UART2->CR2 |= UART2_CR2_TIEN;
}

static i_tiny_event_t* on_send_complete(i_tiny_uart_t* _self)
{
  (void)_self;
  return &send_complete.interface;
}

static i_tiny_event_t* on_receive(i_tiny_uart_t* _self)
{
  (void)_self;
  return &receive.interface;
}

static const i_tiny_uart_api_t api = { send, on_send_complete, on_receive };

i_tiny_uart_t* uart2_init(void)
{
  // Un-gate clock for UART2
  CLK->PCKENR1 |= (1 << CLK_PERIPHERAL_UART2);

  // Configure 230.4k
  // 16,000,000 / UART_DIV = 230,400 => UART_DIV ~= 69 = 0x45
  UART2->BRR2 = 0x5;
  UART2->BRR1 = 0x4;

  // Enable TX, RX
  UART2->CR2 |= UART2_CR2_TEN | UART2_CR2_REN;

  // Enable RXNE (receive data register not empty) interrupt
  UART2->CR2 |= UART2_CR2_RIEN;

  self.api = &api;

  tiny_single_subscriber_event_init(&send_complete);
  tiny_single_subscriber_event_init(&receive);

  return &self;
}

#endif
