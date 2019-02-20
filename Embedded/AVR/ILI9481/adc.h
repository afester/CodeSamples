#ifndef __ADC_H__
#define __ADC_H__

/**
 * Initializes the A/D converter.
 */
void adcInit();

/**
 * @return The current value on ADC
 */
uint16_t adcRead();

uint16_t adcReadChannel(uint8_t channel);

#endif
