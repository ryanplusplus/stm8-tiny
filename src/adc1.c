/*!
 * @file
 * @brief
 */

#include "stm8s_clk.h"
#include "adc1.h"

#ifdef ADC1

static i_tiny_analog_input_group_t self;

static tiny_analog_input_counts_t read(i_tiny_analog_input_group_t* _self, tiny_analog_input_channel_t channel) {
  (void)_self;

  // Select the specified channel and clear EOC
  ADC1->CSR = channel;

  // Start the conversion
  ADC1->CR1 |= ADC1_CR1_ADON;

  // Wait for the conversion to complete
  while(!(ADC1->CSR & ADC1_CSR_EOC)) {
  }

  // Read DRL first and shift 6 to normalize from 10-bit to 16-bit
  tiny_analog_input_counts_t result = ADC1->DRL << 6;
  // Read DRH second and shift 8 + 6 to normalize
  result += ADC1->DRH << 14;
  return result;
}

static const i_tiny_analog_input_group_api_t api = { read };

i_tiny_analog_input_group_t* adc1_init(void) {
  // Un-gate clock for ADC1
  CLK->PCKENR2 |= (1 << (CLK_PERIPHERAL_ADC & 0xF));

  // Right-align the result
  ADC1->CR2 = ADC1_CR2_ALIGN;

  // Power on the peripheral
  ADC1->CR1 = ADC1_CR1_ADON;

  self.api = &api;

  return &self;
}

#endif
