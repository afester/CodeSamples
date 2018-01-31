#include <avr/io.h>
#include <avr/interrupt.h>

#include "adc.h"


void adcInit() {
   ADMUX = /*_BV(REFS1) |*/ _BV(REFS0);	// 2.56V reference, select ADC0

   ADCSRA = _BV(ADEN) | 			   // Enable ADC
            _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);  // 156 kHz ADC clock
   ADCSRB = 0;
   DIDR0 = _BV(ADC0D);			// disable PA0 digital input pin
}


uint16_t adcRead() {
   PRR0 &= ~_BV(PRADC);
   ADCSRA |= _BV(ADSC);		// start single conversion
   while(ADCSRA & _BV(ADSC));   // wait until conversion is complete
   return ADC;
}
