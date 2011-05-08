#include <avr/io.h>

extern void USART_Transmit(uint8_t data);

void debug(const char *text)
{
	const char *ptr = text;
	while (*ptr != '\0')
	{
		USART_Transmit(*ptr++);
	}
}

void debughex(uint8_t c)
{
	switch (c)
	{
		case 0: debug("0"); break;
		case 1: debug("1"); break;
		case 2: debug("2"); break;
		case 3: debug("3"); break;
		case 4: debug("4"); break;
		case 5: debug("5"); break;
		case 6: debug("6"); break;
		case 7: debug("7"); break;
		case 8: debug("8"); break;
		case 9: debug("9"); break;
		case 10: debug("a"); break;
		case 11: debug("b"); break;
		case 12: debug("c"); break;
		case 13: debug("d"); break;
		case 14: debug("e"); break;
		case 15: debug("f"); break;
		default:
			debughex(c >> 8);
			debughex(c & 0x0f);	
			break;
	}
}

void debugln(const char *text)
{
	debug(text);
	USART_Transmit('\r');
	USART_Transmit('\n');
}
