#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "usart.h"
#include "time.h"
#include "nop.h"
#include "config.h"

uint16_t vsyncCount = 0;
uint32_t lastVsync = 0;
uint32_t lastVsyncDelta = 0;

uint32_t bbpCount = 0;
uint32_t lastBbp = 0;
uint32_t lastBbpDelta = 0;

void reportVsyncToHistory()
{
	uint32_t now = getCurrentTime();
	lastVsyncDelta = now - lastVsync;
	lastVsync = now;
	vsyncCount++;
}

void reportBbpToHistory()
{
	uint32_t now = getCurrentTime();
	lastBbpDelta = now - lastBbp;
	lastBbp = now;
	bbpCount++;
}

uint16_t computeFps()
{
	double fps = F_CPU;
	fps /= lastVsyncDelta * 64; // 64 = prescaler
	return (uint16_t) (fps * 1000.0);
}

//uint8_t buttonStatus = 0x00;

void updateOutput()
{	
	//buttonStatus = (buttonStatus == 0) ? 1 : 0;
	//PORTB = buttonStatus;
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

uint16_t line = 0;

/* interrupt vector for VSYNC detection */
ISR(INT0_vect)
{
	reportVsyncToHistory();

	line = 0;
}

/* interrupt vector for Bust / Back Porch detection */
ISR(INT1_vect)
{
	line++;
	_delay_us(15);
	if (line > 50 && line <= 100)
	{
		PORTB = 1;
		nop();
		nop();
		PORTB = 0;
		nop();
		nop();
		PORTB = 1;
		nop();
		nop();
	}
	PORTB = 0;

	reportBbpToHistory();
}

int main(void)
{
	/* USART is on port D, pins 1-0 */
	USART_Init();

	/* VSYNC detection is on port D, pin 2 (INT0) */
	EICRA |= _BV(ISC01); // trigger INT0 on falling edge of signal
	EIMSK |= _BV(INT0); // enable INT0

	/* Bust / Back Porch detection is on port D, pin 3 (INT1) */
	EICRA |= _BV(ISC11); // trigger INT1 on falling edge of signal
	EIMSK |= _BV(INT1); // enable INT1

	/* button inputs are on port D, pins 7-4 */
	DDRD = 0x00; // 0=input 1=output
	PORTD = 0b11110000; // pull-up resistors on pins 7-4
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
	//sei();
	uint8_t i = 0;
	while(1)
	{
		// afficheur 16seg
		PORTB = i++;
		_delay_ms(5);
		
		// autofire:
		/*_delay_ms(1000);
		uint16_t fps = computeFps();
		printf("%u.%03u fps, %d, %ld\n", fps/1000, fps%1000, vsyncCount, lastBbpDelta);*/

		// what?		
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

