/*
 * mini_project 1.
 * Digital clock.
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

unsigned char second = 0;
unsigned char minute = 0;
unsigned char hour = 0;

void Timer1_CTC_Init(void)
{
	TCCR1A = (1<<FOC1A) | (1<<FOC1B); // non PWM
	/*
	 * WGM10 , 11, 13 = 0	WGM12 = 1 work in compare mode
	 * CS11 = 0 , CS10,12 = 1 prescaler 1024
	 */
	TCCR1B = (1<<WGM12) | (1<< CS10) | (1<<CS12);
	// initial value the counter by ZERO
	TCNT1=0;
	// by 1 MHZ and prescaler 1024 counter will count by 1ms ,1 second = 1000 ms OCR1A = 1000;
	OCR1A=100;
	// enable enterrupt for timer1A compare
	TIMSK |= (1<<OCIE1A);
	SREG  |= (1<<7);
}
ISR(TIMER1_COMPA_vect)
{
	second ++;
	if (second == 60)
	{
		second = 0;
		minute ++;
		if (minute == 60 )
			{
				second = 0;
				minute = 0;
				hour ++;
				if (hour == 24)
					{
						second = 0;
						minute = 0;
						hour = 0;
					}
			}

	}

}
void INT0_Init(void)
{
	// disable IBET.
	SREG   &= ~(1<<7);
	// enable internal pull up for INT0
	DDRD   &= ~(1<<PD2);
	PORTD  |= (1<<PD2);
	// enable interrupt for INT0
	GICR   |= (1<<INT0);
	// falling edge for INT0
	MCUCR  &= ~(1<<ISC00);
	MCUCR  |= (1<<ISC01);
	// enable i bet
	SREG   |= (1<<7);
}

ISR (INT0_vect)
{
	// reset the timer
	second = 0 ;
	minute = 0;
	hour = 0 ;
}

void INT1_Init(void){
	// disable IBET.
	SREG   &= ~(1<<7);
	// setting PD3 as input bin
	DDRD   &= ~(1<<PD3);
	// enable interrupt for INT0
	GICR   |= (1<<INT1);
	// raising edge for INT1
	MCUCR  |= (1<<ISC10) | (1<<ISC11);
	// enable I bet
	SREG   |= (1<<7);

}

ISR (INT1_vect)
{
	TCCR1A = 0x00;
	TCCR1B = 0x00;

}

void INT2_Init(void){
	// disable IBET.
	SREG   &= ~(1<<7);
	// enable internal pull up for INT2
	DDRB   &= ~(1<<PB2);
	PORTB  |= (1<<PB2);
	// enable interrupt for INT2
	GICR   |= (1<<INT2);
	// falling edge
	MCUCSR &= ~(1<<ISC2);
	// enable i bet
	SREG   |= (1<<7);
}

ISR(INT2_vect)
{
	TCCR1A = (1<<FOC1A) | (1<<FOC1B); // non PWM
	/*
	 * WGM10 , 11, 13 = 0	WGM12 = 1 work in compare mode
	 * CS11 = 0 , CS10,12 = 1 prescaler 1024
	*/
	TCCR1B = (1<<WGM12) | (1<< CS10) | (1<<CS12);

}

int main (void)
{
	DDRC  |= 0x0F;     // first 4 bins in port c is output
	PORTC &= ~(0xF0);  // start with initial 0
	DDRA  |= 0x3F;	   // first 6 bins in PORT A in output
	PORTA &= 0xC0;     // all segments are disabled
	SREG  |= (1<<7);   // enable IBET


	INT0_Init();
	INT1_Init();
	INT2_Init();
	Timer1_CTC_Init();
	while(1)
		{
			PORTA = (1<<PA0);
			PORTC = (PORTC & (0xF0)) | ((second % 10) & (0x0F));
			_delay_ms(3);

			PORTA = (1<<PA1);
			PORTC = (PORTC & (0xF0)) | ((second / 10) & (0x0F));
			_delay_ms(3);

			PORTA = (1<<PA2);
			PORTC = (PORTC & (0xF0)) | ((minute % 10) & (0x0F));
			_delay_ms(3);

			PORTA = (1<<PA3);
			PORTC = (PORTC & (0xF0)) | ((minute / 10) & (0x0F));
			_delay_ms(3);

			PORTA = (1<<PA4);
			PORTC = (PORTC & (0xF0)) | ((hour % 10) & (0x0F));
			_delay_ms(3);

			PORTA = (1<<PA5);
			PORTC = (PORTC & (0xF0)) | ((hour / 10) & (0x0F));
			_delay_ms(3);
			PORTA &= 0xC0;
		}
}


