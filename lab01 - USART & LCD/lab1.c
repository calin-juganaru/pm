#include "lcd.h"
#include "usart.h"
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>


#define MORSE_LUNG  3
#define MORSE_SCURT 1
#define stai_degeaba for (;;)

//typedef enum {false, true} bool;

/* Literele alfabetului in cod Morse */
char* morse_alphabet[] =
{
    (char*)"13",    // a
    (char*)"3111",  // b
    (char*)"3131",  // c
    (char*)"311",   // d
    (char*)"1",     // e
    (char*)"1131",  // f
    (char*)"331",	// g
    (char*)"1111",	// h
    (char*)"11",	// i
    (char*)"1333",	// j
    (char*)"313",	// k
    (char*)"1311",	// l
    (char*)"33",	// m
    (char*)"31",	// n
    (char*)"333",	// o
    (char*)"1331",	// p
    (char*)"3313",	// q
    (char*)"131",	// r
    (char*)"111",	// s
    (char*)"3",	    // t
    (char*)"113",	// u
    (char*)"1113",	// v
    (char*)"133",	// w
    (char*)"3113",	// x
    (char*)"3133",	// y
    (char*)"3311"	// z
};

void exemplu_usart()
{
    for(;;)
    {
        USART0_print("Salut!\n\r");
        _delay_ms(10);
    }
}

void task1()
{
    bool bulean = false;

    DDRD &= ~(1 << PD6);
    PORTD |= (1 << PD6);

    for (;;)
    {
        if (!(PIND & (1 << PD6)) && !bulean)
        {
            USART0_print("PD6 apasat!\n\r");
            bulean = true;
            _delay_ms(100);
        }

        if ((PIND & (1 << PD6)) && bulean)
        {
            USART0_print("PD6 lasat in pace!\n\r");
            bulean = false;
            _delay_ms(100);
        }

        _delay_ms(5);
    }
}


/* apelati functia pentru a obtine un ton lung/scurt,
 * folosind macro-urile MORSE_LUNG si MORSE_SCURT
 */
void speaker_morse(int tip)
{
    int delay = 1;
    int i;
    DDRD |= (1 << PD4);     // PD4 trebuie sa fie output

    for (i = 0; i < tip * 50 / delay; i++)
    {
        PORTD ^= (1 << PD4);  // alternam starea iesirii
        _delay_ms(1);
    }
}

void task2()
{
    char c;
    int i;
    char* sir;

    DDRD |= (1 << PD4);
    PORTD &= ~(1 << PD4);

    // main loop
    for (;;)
    {
        // primeste un caracter
        // daca este caracter litera mica il reda in codul Morse
        // folosing  speaker_morse
        c = USART0_receive();

        if (c < 'a' || c > 'z')
            continue;

        sir = morse_alphabet[c - 'a'];
        for (i = 0; i < strlen(sir); ++i)
        {
            speaker_morse(sir[i] - '0');
            _delay_ms(50);
        }
    }
}


void exemplu_lcd()
{
    DDRC |= (1 << PC7);
    PORTC |= (1 << PC7);
    LCD_print("Hello World!");

    stai_degeaba;
}

void task4a()
{
    DDRC |= (1 << PC7);
    PORTC |= (1 << PC7);

    DDRD &= ~(1 << PD6);
    PORTD |= (1 << PD6);

    DDRB &= ~(1 << PB2);
    PORTB |= (1 << PB2);

    bool buleanD = 0, buleanB = 0;
    uint8_t pozitie = 0;

    char str[]   = "Helau <3";
    char empty[] = "                     ";
    LCD_print(str);

    for (;;)
    {
        if (!(PIND & (1 << PD6)) && !buleanD)
        {
            pozitie = (pozitie + 1) % 16;
            buleanD = true;
            LCD_printAt(0x80, empty);
            LCD_printAt(pozitie, str);
        }
        if ((PIND & (1 << PD6)) && buleanD)
        {
            buleanD = false;
        }

        if (!(PINB & (1 << PB2)) && !buleanB)
        {
            pozitie = (pozitie - 1) % 16;
            buleanB = true;
            LCD_printAt(0x80, empty);
            LCD_printAt(pozitie, str);
        }
        if ((PINB & (1 << PB2)) && buleanB)
        {
            buleanB = false;
        }

        _delay_ms(5);
    }
}

void task4b()
{
    char c;
    uint8_t address = 0x80;

    DDRC |= (1 << PC7);
    PORTC |= (1 << PC7);

    DDRD |= (1 << PD4);
    PORTD &= ~(1 << PD4);

    char empty[] = "                ";

    for (;;)
    {
        c = USART0_receive();

        if (address >= 0x90)
        {
            address -= 16;
            LCD_printAt(0x80, empty);
        }

        LCD_putCharAt(address++, c);
    }
}

void bonus_usart()
{
    stdout->put = USART0_printf;
    stdout->flags |= 0x0002;

    int8_t PB2_flag = 0, PD6_flag = 0;
    for (;;)
    {
        // cand PB2 este apasat trimite un mesaj pe USART
        if(!(PINB & (1 << PB2)) && !PB2_flag)
        {
            PB2_flag = 1;
            printf("PB2 apasat\n");
        }

        // cand PB2 este lasat trimite un mesaj pe USART
        if((PINB & (1 << PB2)) && PB2_flag)
        {
            PB2_flag = 0;
            printf("PB2 lasat\n");
        }

        // cand PD6 este apasat trimite un mesaj pe USART
        if(!(PIND & (1 << PD6)) && !PD6_flag)
        {
            PD6_flag = 1;
            printf("PD6 apasat\n");
        }

        // cand PD6 este lasat trimite un mesaj pe USART
        if((PIND & (1 << PD6)) && PD6_flag)
        {
            PD6_flag = 0;
            printf("PD6 lasat\n");
        }

        // much switch, so debouncing, such delay
        _delay_ms(5);
    }
}

int main()
{
    USART0_init();
    LCD_init();

    switch (6)
    {
        case 0: exemplu_usart();    break;
        case 1: task1();            break;
        case 2: task2();            break;
        case 3: exemplu_lcd();      break;
        case 4: task4a();           break;
        case 5: task4b();           break;
        case 6: bonus_usart();      break;
    }

    return 0;
}