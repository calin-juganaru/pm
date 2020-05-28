#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "usart.h"

#define BUFSIZE	256
#define ADC_AREF_VOLTAGE	5.0
#define ADC_MAX_LEVEL	1024.0
#define LM35_RESOLUTION	0.01

volatile uint16_t ADC_value;

ISR(ADC_vect)
{	
	/* TODO 2: Read ADC value */
	/* TODO 2: Start another conversion */
	ADCSRA |= (1 << ADSC);
	/* TODO 3: Clear output compare match flag A */
	TIFR0 |= (1 << OCF0A);
}

void ADC_init()
{
	/* TODO: Configure channel */
	ADMUX = (6 << MUX0) | (1 << REFS0);
	
	/* set prescaler at 128 */
	ADCSRA = (7 << ADPS0);

	/* TODO 3: Enable auto trigger */
	ADCSRA |= (1 << ADATE);

	/* TODO 3: Timer0 compare match A */
	ADCSRB = (3 << ADTS0);

	/* TODO 2: Enable conversion complete interrupt */
	/* enable ADC */
	ADCSRA |= (1 << ADEN) | (1 << ADIE);

	/* start conversion */
	ADCSRA |= (1 << ADSC);

	TCNT0 = 0;
	OCR0A = 117;
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS00) | (1 << CS02);
}

int main(void)
{
	ADC_init();
	USART0_init();

	/* TODO2: Enable global interrupts */
	sei();

	char* buffer = (char *)calloc(20, 1);

	if (1) // Task 3:
		while (1)
		{
			double tensiune = ADC * ADC_AREF_VOLTAGE / ADC_MAX_LEVEL;
			sprintf(buffer, "Tensiune = %.2f\r\n", tensiune);
			USART0_print(buffer);		
			_delay_ms(1000);
		}
	else while (1)
	{
		// Task 1 & 2:
		ADCSRA |= (1 << ADSC);
		while (!(ADCSRA & (1 << ADIF)));

		double tensiune = ADC * ADC_AREF_VOLTAGE / ADC_MAX_LEVEL;
		double temperatura = tensiune / LM35_RESOLUTION;

		sprintf(buffer, "Tensiune = %.2f\r\n", tensiune);
		USART0_print(buffer);
		sprintf(buffer, "Temperatura = %.2f\r\n", temperatura);
		USART0_print(buffer);	
		
		_delay_ms(1500);
	}

	free(buffer);
	return 0;
}
