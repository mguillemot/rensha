#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "usart.h"

uint16_t timer1overflows = 0;

uint32_t getCurrentTime()
{
	uint32_t res = timer1overflows;
	res <<= 16;
	res |= TCNT1;
	return res; 
}

#define VSYNC_HISTORY_SIZE 16
uint32_t vsyncHistory[VSYNC_HISTORY_SIZE];
uint8_t nextVsyncHistory = 0;

void reportVsyncToHistory()
{
	vsyncHistory[nextVsyncHistory++] = getCurrentTime();
	if (nextVsyncHistory == VSYNC_HISTORY_SIZE)
	{
		nextVsyncHistory = 0;
	}
}

uint16_t computeFps()
{
	uint32_t first = vsyncHistory[nextVsyncHistory];
	uint32_t last = (nextVsyncHistory > 0) ? vsyncHistory[nextVsyncHistory - 1] : vsyncHistory[VSYNC_HISTORY_SIZE - 1];
	double averageDeltaT = 0;
	/*for (uint8_t i = 0; i < VSYNC_HISTORY_SIZE; i++)
	{
		printf("%u %08ld\n", i, vsyncHistory[i]);
	}*/
	averageDeltaT = (last - first) / (VSYNC_HISTORY_SIZE - 1);
	averageDeltaT *= 64; // prescaler
	double fps = F_CPU;
	fps /= averageDeltaT;
	return (uint16_t) (fps * 1000.0);
}

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

/* interrupt vector for port D inputs */
ISR(PCINT2_vect)
{
	//updateOutput();

	// VSYNC
	//uint16_t timer = TCNT1;
	//uint8_t in = PIND;
	//uint8_t burst = (in >> 3) & 1; // pin 3
	//uint8_t oddeven = (in >> 4) & 1; // pin 4
	//uint8_t vsync = (in >> 5) & 1; // pin 5
	//uint8_t compo = (in >> 6) & 1; // pin 6
	//printf("%u,%d\n", timer, vsync);
}

/* interrupt vector for VSYNC detection */
ISR(INT0_vect)
{
	reportVsyncToHistory();
}

/* interrupt vector for timer 1 overflow */
ISR(TIMER1_OVF_vect)
{
	timer1overflows++;
	//printf("now is %08lx\n", getCurrentTime());
}

int main(void)
{
	/* USART is on port D, pins 1-0 */
	USART_Init();

	/* VSYNC detection is on port D, pin 2 (INT0) */
	EICRA |= _BV(ISC01); // trigger INT0 on falling edge of signal
	EIMSK |= _BV(INT0); // enable INT0

	/* button inputs are on port D, pins 7-3 */
	DDRD = 0x00; // 0=input 1=output
	PORTD = 0b11111000; // pull-up resistors on pins 7-3
	//PCMSK2 |= _BV(PCINT19); // enable port D pin 3 contribution to PCINT2 interrupt
	//PCMSK2 |= _BV(PCINT20); // enable port D pin 4 contribution to PCINT2 interrupt
	//PCMSK2 |= _BV(PCINT21); // enable port D pin 5 contribution to PCINT2 interrupt
	//PCMSK2 |= _BV(PCINT22); // enable port D pin 6 contribution to PCINT2 interrupt
	//PCICR = _BV(PCIE2); // enable PCINT2 interrupt

	/* button outputs are on port B */ 
	DDRB = 0xff; // 0=input 1=output

	/* timer 1 */
	TCCR1B |= _BV(CS11) | _BV(CS10); // enabled with div64 prescaleer
	TIMSK1 |= _BV(TOIE1); // Overflow Interrupt Enable

	printf("Booting...\n");

	sei();
	while(1)
	{
		// waiting for interrupts...

		_delay_ms(1000);
		printf("%u fps\n", computeFps());
		
		/*
		uint16_t timer = TCNT1;
		uint8_t in = PIND;
		uint8_t burst = (in >> 3) & 1; // pin 3
		uint8_t oddeven = (in >> 4) & 1; // pin 4
		uint8_t vsync = (in >> 5) & 1; // pin 5
		uint8_t compo = (in >> 6) & 1; // pin 6
		printf("%d,%d,%d,%d\n", burst, oddeven, vsync, compo);*/
	}
}

