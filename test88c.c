#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "usart.h"

uint8_t buttonStatus = 0x00;
uint16_t previousTimer = 0;

void updateOutput()
{	
	uint8_t in = (PIND >> 2) & 0x1;
	uint16_t timer = TCNT1;
	int16_t delta = timer - previousTimer;
	//if (delta > 220)
		printf("%u,%d,%d\n", timer, delta, in);
	previousTimer = timer;
	/*if (in == buttonStatus)
	{
		printf("%u | no change in button 1 status\n", timer); // should not happen
	}
	else
	{
		if (in)
		{
			printf("%u | button 1 released\n", timer);
		}
		else
		{
			printf("%u | button 1 pushed\n", timer);
		}
		buttonStatus = in;
	}
	PORTB = !buttonStatus; */
}

ISR(PCINT2_vect)
{
	updateOutput();
}

int main(void)
{
	/* USART is on port D, pins 1-0 */
	USART_Init();

	/* button inputs are on port D, pins 7-2 */
	DDRD = 0x00; // 0=input 1=output
	PORTD = 0xfc; // pull-up resistors
	PCMSK2 = _BV(PCINT18); // enable PCINT18 (port B pin 2) contribution to PCINT2 interrupt
	//PCICR = _BV(PCIE2); // enable PCINT2 interrupt

	/* button input is on port B */ 
	DDRB = 0xff; // 0=input 1=output

	/* set-up timer 1 to "enabled without prescaler" */
	TCCR1B |= _BV(CS11) | _BV(CS10); // div64

	printf("Booting Erhune's super-giga-autofire...\n");

	//sei();
	while(1)
	{
		// waiting for interrupts...

		_delay_ms(10);
		uint16_t timer = TCNT1;
		uint8_t in = (PIND >> 2) & 0x1;
		printf("%u,%d\n", timer, in);
	}
}

