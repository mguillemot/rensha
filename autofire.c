/*
 * autofire.c
 *
 * Created: 22/05/2011 23:39:59
 *  Author: Erhune
 */ 

#include "autofire.h"
#include "asm.h"


void Autofire_P1B1_Changed(uint8_t v)
{
	if (v)
	{
		bit_set(PORTD, _BV(2));
	}
	else
	{
		bit_clear(PORTD, _BV(2));
	}
}

void Autofire_P1B2_Changed(uint8_t v)
{
	if (v)
	{
		bit_set(PORTD, _BV(3));
	}
	else
	{
		bit_clear(PORTD, _BV(3));
	}
}

void Autofire_P1B3_Changed(uint8_t v)
{
	if (v)
	{
		bit_set(PORTD, _BV(4));
	}
	else
	{
		bit_clear(PORTD, _BV(4));
	}
}

void Autofire_P1START_Changed(uint8_t v)
{
	if (v)
	{
		bit_set(PORTD, _BV(5));
	}
	else
	{
		bit_clear(PORTD, _BV(5));
	}
}

void Autofire_Init()
{
	/* P1UP on port C, pin 0 */
	bit_set(DDRC, _BV(0));  // output
	bit_set(PORTC, _BV(0)); // high value

	/* P1DOWN on port C, pin 1 */
	bit_set(DDRC, _BV(1));  // output
	bit_set(PORTC, _BV(1)); // high value
	
	/* P1LEFT on port D, pin 6 */
	bit_set(DDRD, _BV(6));  // output
	bit_set(PORTD, _BV(6)); // high value

	/* P1RIGHT on port D, pin 7 */
	bit_set(DDRD, _BV(7));  // output
	bit_set(PORTD, _BV(7)); // high value
	
	/* P1B1 input on port B, pin 0 (PCINT0_pin); output on port D, pin 2 */
	bit_clear(DDRB, _BV(0)); // input
	bit_set(PORTB, _BV(0));  // pull-up
	bit_set(PCMSK0, _BV(PCINT0)); // enable contribution to PCINT0_int
	bit_set(DDRD, _BV(2));  // output
	bit_set(PORTD, _BV(2)); // high value
	
	/* P1B2 input on port B, pin 1 (PCINT1_pin); output on port D, pin 3 */
	bit_clear(DDRB, _BV(1)); // input
	bit_set(PORTB, _BV(1));  // pull-up
	bit_set(PCMSK0, _BV(PCINT1)); // enable contribution to PCINT0_int
	bit_set(DDRD, _BV(3));  // output
	bit_set(PORTD, _BV(3)); // high value

	/* P1B3 input on port B, pin 2 (PCINT2_pin); output on port D, pin 4 */
	bit_clear(DDRB, _BV(2)); // input
	bit_set(PORTB, _BV(2));  // pull-up
	bit_set(PCMSK0, _BV(PCINT2)); // enable contribution to PCINT0_int
	bit_set(DDRD, _BV(4));  // output
	bit_set(PORTD, _BV(4)); // high value
	
	/* P1START input on port C, pin 3 (PCINT11_pin); output on port D, pin 5 */
	bit_clear(DDRC, _BV(3)); // input
	bit_set(PORTC, _BV(3));  // pull-up
	bit_set(PCMSK1, _BV(PCINT11)); // enable contribution to PCINT1_int
	bit_set(DDRD, _BV(5));  // output
	bit_set(PORTD, _BV(5)); // high value
	
	/* enable interrupts */
	bit_set(PCICR, _BV(PCIE0)); // PCINT0_int
	bit_set(PCICR, _BV(PCIE1)); // PCINT1_int
	//bit_set(PCICR, _BV(PCIE2)); // PCINT2_int
}
