
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

// digit pattern for a 7-segment display
const uint8_t digit_pattern[16] =
{
		0b00111111, // 0
		0b00000110, // 1
		0b01011011, // 2
		0b01001111, // 3
		0b01100110, // 4
		0b01101101, // 5
		0b01111101, // 6
		0b00000111, // 7
		0b01111111, // 8
		0b01101111, // 9
		0b01110111, // A
		0b01111100, // b
		0b00111001, // C
		0b01011110, // d
		0b01111001, // E
		0b01110001  // F
};

volatile uint16_t counter = 0;
uint8_t digits[2];
void get_digits(uint8_t num, uint8_t ndigits);
void output_7segment(uint8_t val);
void switch_digit(uint8_t dig);
volatile bool state = false;

/* Timer1 Output Compare Interrupt A */
ISR(TIMER1_COMPA_vect)
{
	if (state)
	{
		counter = (counter + 1) % 0xFF;
		PORTD ^= (1 << PD2);
	}
}

/* pin change interrupt on PORTD */
ISR(PCINT3_vect)
{
	if (!(PIND & (1 << PD3)))
	{
		state = !state;
		PORTD ^= (1 << PD4);
	}
}

/* initialize GPIO inputs and outputs */
void IO_init()
{
	DDRD |= (1 << PD4);
	PORTD &= ~(1 << PD4);

	DDRD &= ~(1 << PD3);
	PORTD |= (1 << PD3);

	PCMSK3 |= (1 << PCINT27);
	PCICR |= (1 << PCIE3);

	DDRD |= (1 << PD2);
	PORTD &= ~(1 << PD2);

	DDRA |= (1 << PA0);
	DDRA |= (1 << PA1);
	DDRA |= (1 << PA2);
	DDRA |= (1 << PA3);
	DDRA |= (1 << PA4);
	DDRA |= (1 << PA5);
	DDRA |= (1 << PA6);

	DDRC |= (1 << PC0);
	DDRC |= (1 << PC1);
}

/* initializeaza timer-ul 1 */
void timer1_init()
{
	// initialize the timer counter
	TCNT1 = 0;
	// configure the threshold
	OCR1A = 46875;
	// enable compare interrupt A
	TIMSK1 = (1 << OCIE1A);
	// configure mode of operation e.g. CTC with OCR1A
	TCCR1B = (1 << WGM12);
	// set the prescaler e.g 256
	TCCR1B |= (1 << CS12);
}

int main(void)
{
	// init peripherals
	IO_init();
	_delay_ms(1000);
	timer1_init();
	sei();

	// power on test
	PORTD |= (1 << PD4);
	_delay_ms(500);
	PORTD &= ~(1 << PD4);
	_delay_ms(500);

	// main loop
	while (1)
	{
		PORTC = (1 << PC1);
		PORTA = digit_pattern[counter % 16];
		_delay_ms(10);
		PORTC = (1 << PC0);
		PORTA = digit_pattern[counter / 16];
		_delay_ms(10);	
	}

	return 0;
}
