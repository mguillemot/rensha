/*
 * video.c
 *
 * Created: 30/07/2011 02:21:44
 *  Author: Erhune
 */ 

#include <avr/interrupt.h>
#include "video.h"
#include "time.h"
#include "asm.h"
#include "autofire.h"


uint16_t line = 0;

#ifdef ENABLE_VSYNC_DETECTION

uint32_t vsyncCount = 0;
uint32_t lastVsync = 0;
uint32_t lastVsyncDelta = 0;

void Video_Vsync_Triggered()
{
	/*uint32_t now = getCurrentTime();
	lastVsyncDelta = now - lastVsync;
	lastVsync = now;*/
	vsyncCount++;

	line = 0;
	
	Autofire_Update(vsyncCount);
}

uint16_t Video_ComputeFps()
{
	double fps = F_CPU;
	fps /= lastVsyncDelta * 64; // 64 = prescaler
	return (uint16_t) (fps * 1000.0);
}

#endif /* ENABLE_VSYNC_DETECTION */

#ifdef ENABLE_BBP_DETECTION

void Video_Bbp_Triggered()
{
	line++;
	
	/*_delay_us(15);
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
	PORTB = 0;*/
}

#endif /* ENABLE_BBP_DETECTION */

void Video_Init()
{
	#ifdef ENABLE_VSYNC_DETECTION
	
	/* VSYNC detection is on port C, pin 4 (PCINT12_pin) */
	bit_clear(DDRC, _BV(4)); // input
	bit_clear(PORTC, _BV(4));  // no pull-up
	bit_set(PCMSK1, _BV(PCINT12)); // enable contribution to PCINT1_int

	/* enable interrupt */
	bit_set(PCICR, _BV(PCIE1)); // PCINT1_int
	
	#endif /* ENABLE_VSYNC_DETECTION */

	#ifdef ENABLE_BBP_DETECTION
	
	/* Burst / Back Porch detection is on port C, pin 5 (PCINT13_pin) */
	bit_clear(DDRC, _BV(5)); // input
	bit_clear(PORTC, _BV(5));  // no pull-up
	bit_set(PCMSK1, _BV(PCINT13)); // enable contribution to PCINT1_int

	/* enable interrupt */
	bit_set(PCICR, _BV(PCIE1)); // PCINT1_int
	
	#endif /* ENABLE_BBP_DETECTION */

	/* timer 1 */
	/*
	TCCR1B |= _BV(CS11) | _BV(CS10); // enabled with div64 prescaleer
	TIMSK1 |= _BV(TOIE1); // Overflow Interrupt Enable
	*/
}