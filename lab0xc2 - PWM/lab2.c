#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "usart.h"

volatile char pb2_pressed=0;
volatile char dir = 1;
volatile char dir_changed = 0;

ISR(TIMER1_COMPA_vect)
{
	/* Set pin high */
	PORTD |= (1 << PD5);
}

ISR(TIMER1_COMPB_vect)
{
	/* Set pin low */
	PORTD &= ~(1 << PD5);
}

void timer1_init()
{
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << CS11);

	TCNT1 = 0;
	OCR1A = 150;
	OCR1B = 50;
}


void timer1_init_new()
{
	TCCR1A |= _BV(WGM10) | _BV(COM1A1);
	TCCR1B |= _BV(WGM12) | _BV(CS11);

	TCNT1 = 0;
	OCR1A = 25;
}

ISR(INT2_vect)
{
	pb2_pressed = 1;
}

ISR(INT0_vect)
{
	dir_changed = 1;
	dir = (PIND & (1 << PD3));
}

void int0_init() {
	EICRA |= _BV(ISC01) | _BV(ISC00);
	EIMSK |= _BV(INT0);
}

void int2_init()
{
	EICRA |= _BV(ISC21);
	EIMSK |= _BV(INT2);
}


int main(void)
{
	// initialize the timer
	timer1_init_new();
	// initialize interrupt for int2
	int2_init();
	int0_init();
	sei();
	USART0_init();

	DDRB &= ~_BV(PB2);
	PORTB |= _BV(PB2);

	DDRD |= (1 << PD4);
	DDRD |= (1 << PD5);

	DDRD  &= ~(1 << PD2);
	DDRD &= ~(1 << PD3);

    while (1)
    {
	    if(pb2_pressed)
	    {
			USART0_print("Buton apasat\r\n");
			PORTD ^= (1 << PD4);
			pb2_pressed = 0;
	    }

		OCR1A = (OCR1A + 25) % 255;

		if (dir_changed)
		{
			if (dir)
				 USART0_print("CLOCK\r\n");
			else USART0_print("COUNTER\r\n");
			dir_changed = 0;
		}
	}

	return 0;
}