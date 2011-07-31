#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "asm.h"
#include "config.h"
#include "usart.h"
#include "video.h"
#include "autofire.h"


uint8_t portB = 0;
uint8_t portC = 0;

/* interrupt vector for PCINT0_int pin change interrupt */
ISR(PCINT0_vect)
{
	uint8_t newB = PINB;
	uint8_t change = newB ^ portB;
	portB = newB;
	if (change & _BV(0))
	{
		Autofire_P1B1_Changed(newB & 1);
	}
	if (change & _BV(1))
	{
		Autofire_P1B2_Changed((newB >> 1) & 1);
	}
	if (change & _BV(2))
	{
		Autofire_P1B3_Changed((newB >> 2) & 1);
	}
}

/* interrupt vector for PCINT1_int pin change interrupt */
ISR(PCINT1_vect)
{
	uint8_t newC = PINC;
	uint8_t change = newC ^ portC;
	portC = newC;
	if (change & _BV(3))
	{
		Autofire_P1START_Changed((newC >> 3) & 1);
	}
}

int main(void)
{
	//USART_Init();
	//printf("USART initialized.\n");
	//Video_Init();
	//printf("Video initialized.\n");
	Autofire_Init();
	printf("Autofire initialized.\n");

	/* Output led on port C, pin 2 */
	bit_set(DDRC, _BV(2));  // output
	bit_set(PORTC, _BV(2));

	sei();
	uint8_t i = 0;
	while(1)
	{
		i++;
		
		/* Blinks the LED */
		_delay_ms(500);
		bit_clear(PORTC, _BV(2));
		_delay_ms(500);
		bit_set(PORTC, _BV(2));
		
		/*uint16_t fps = computeFps();
		printf("%u.%03u fps, %d, %ld\n", fps/1000, fps%1000, vsyncCount, lastBbpDelta);*/
	}		
}

