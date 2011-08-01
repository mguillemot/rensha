#include <avr/interrupt.h>
#include "time.h"

uint16_t timer1overflows = 0;

uint32_t getCurrentTime()
{
	uint32_t res = timer1overflows;
	res <<= 16;
	res |= TCNT1;
	return res; 
}

/* interrupt vector for timer 1 overflow */
ISR(TIMER1_OVF_vect)
{
	timer1overflows++;
}

void Timer_Init()
{
	
}