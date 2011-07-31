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

uint16_t line = 0;

#ifdef ENABLE_VSYNC_DETECTION
uint16_t vsyncCount = 0;
uint32_t lastVsync = 0;
uint32_t lastVsyncDelta = 0;

void reportVsyncToHistory()
{
	uint32_t now = getCurrentTime();
	lastVsyncDelta = now - lastVsync;
	lastVsync = now;
	vsyncCount++;
}

/* interrupt vector for VSYNC detection */
ISR(INT0_vect)
{
	reportVsyncToHistory();

	line = 0;
}

uint16_t computeFps()
{
	double fps = F_CPU;
	fps /= lastVsyncDelta * 64; // 64 = prescaler
	return (uint16_t) (fps * 1000.0);
}
#endif

#ifdef ENABLE_BBP_DETECTION
uint32_t bbpCount = 0;
uint32_t lastBbp = 0;
uint32_t lastBbpDelta = 0;

void reportBbpToHistory()
{
	uint32_t now = getCurrentTime();
	lastBbpDelta = now - lastBbp;
	lastBbp = now;
	bbpCount++;
}

/* interrupt vector for Burst / Back Porch detection */
ISR(INT1_vect)
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

	reportBbpToHistory();
}
#endif

void Video_Init()
{
	#ifdef ENABLE_VSYNC_DETECTION
	/* VSYNC detection is on port C, pin 6 (INT0) */
	//EICRA |= _BV(ISC01); // trigger INT0 on falling edge of signal
	//EIMSK |= _BV(INT0); // enable INT0
	#endif

	#ifdef ENABLE_BBP_DETECTION
	/* Bust / Back Porch detection is on port D, pin 3 (INT1) */
	//EICRA |= _BV(ISC11); // trigger INT1 on falling edge of signal
	//EIMSK |= _BV(INT1); // enable INT1

	/* timer 1 */
	/*
	TCCR1B |= _BV(CS11) | _BV(CS10); // enabled with div64 prescaleer
	TIMSK1 |= _BV(TOIE1); // Overflow Interrupt Enable
	*/
	#endif
}