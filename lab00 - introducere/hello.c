void task1(void)
{
    /* TODO1: Setare directii porturi folosite. */
	int arr[] = {PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7};
	DDRC = 255;
	int dilei = 250;
	int bulean = 1;

	DDRB &= ~(1 << PB2);
    PORTB |= (1 << PB2);

    while(1) {
		if (bulean == 1) {
			for (int i = 0; i < 8; ++i) {
				PORTC = (1 << arr[i]);
			 	_delay_ms(dilei);
				bulean = ((PINB & (1 << PB2)) == 0) ? 1 : 0;
			}

			for (int i = 6; i >= 0; --i) {
				PORTC = (1 << arr[i]);
			 	_delay_ms(dilei);
				bulean = ((PINB & (1 << PB2)) == 0) ? 1 : 0;
			}

		} else {
			for (int i = 0; i < 8; ++i) {
				PORTC = (1 << arr[i]);
			 	_delay_ms(dilei);
				bulean = ((PINB & (1 << PB2)) == 0) ? 1 : 0;
			}
		}
    }
}


// ===========================================================


void task2(void)
{
    DDRD &= ~(1 << PD6);
    PORTD |= (1 << PD6);

    DDRB &= ~(1 << PB2);
    PORTB |= (1 << PB2);

    DDRD |= (1 << PD4);
    PORTD &= ~(1 << PD4);

	int dilei_mare = 2;

    while (1) {
        if ((PIND & (1 << PD6)) == 0)
		for (int i = 0; i < 10; ++i) {
			PORTD |= (1 << PD4);
			_delay_ms(1);
			PORTD &= ~(1 << PD4);
			_delay_ms(1);
			PORTD |= (1 << PD4);
			_delay_ms(1);
			PORTD &= ~(1 << PD4);
			_delay_ms(1);
			PORTD |= (1 << PD4);
			_delay_ms(1);
			PORTD &= ~(1 << PD4);
			_delay_ms(1);
		}
		else if ((PINB & (1 << PB2)) == 0)
        for (int i = 0; i < 25; ++i) {
            PORTD |= (1 << PD4);
			_delay_ms(dilei_mare);
			PORTD &= ~(1 << PD4);
			_delay_ms(dilei_mare);
			PORTD |= (1 << PD4);
			_delay_ms(dilei_mare);
			PORTD &= ~(1 << PD4);
			_delay_ms(dilei_mare);
			PORTD |= (1 << PD4);
			_delay_ms(dilei_mare);
			PORTD &= ~(1 << PD4);
			_delay_ms(dilei_mare);
		}
		_delay_ms(50);
    }
}


// ===========================================================


void task3(void) {
    DDRD &= ~(1 << PD6);
    PORTD |= (1 << PD6);

    DDRB &= ~(1 << PB2);
    PORTB |= (1 << PB2);

    DDRD |= (1 << PD4);
    PORTD &= ~(1 << PD4);
	DDRC = 255;

	int dilei_mare = 2, nota = 0;
	int arr[] = {PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7};

    while (1) {
		int f = (2500.0 / float(frecventa_nota[nota]));
		PORTC &= ~(1 << arr[nota]);
		if ((PIND & (1 << PD6)) == 0) ++nota;
		nota = _min(nota, 7);
		if ((PINB & (1 << PB2)) == 0) --nota;
		while (nota == -1) {
			if ((PIND & (1 << PD6)) == 0) ++nota;
		}

		for (int i = 0; i < 10; ++i) {
			PORTD ^= (1 << PD4);
			for (int j = f; j > 0; --j)
				_delay_ms(1);
		}

     PORTC = (1 << arr[nota]);
		_delay_ms(100);
    }
}