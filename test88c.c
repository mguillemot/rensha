#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t buttonOutput = 0x00;

void USART_Init()
{
	/* Init USART clock */
	#define BAUD 9600
	#include <util/setbaud.h>
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	#if USE_2X
	UCSR0A |= (1 << U2X0);
	#else
	UCSR0A &= ~(1 << U2X0);
	#endif

	/* Enable receiver and transmitter and receive interrupt */
	UCSR0B = _BV(RXEN0)|_BV(TXEN0)|_BV(RXCIE0);

	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (0<<USBS0)|(3<<UCSZ00);
}

void USART_Transmit(unsigned char data)
{
	/* Wait for empty transmit buffer */
	while (!(UCSR0A & _BV(UDRE0)));
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char USART_Receive()
{
	/* Wait for data to be received */
	while (!(UCSR0A & _BV(RXC0)));
	/* Get and return received data from buffer */
	return UDR0;
}

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

void toggleButton()
{
	buttonOutput = (buttonOutput == 0x00) ? 0x01 : 0x00;
	PORTB = buttonOutput;
}

ISR(USART_RX_vect)
{
	unsigned char c = USART_Receive();
	if (c == '\r')
	{
		toggleButton();
		debugln("");
		debugln("OK.");
	}
	else
	{
		USART_Transmit(c);
	}
}

ISR(PCINT2_vect)
{
	uint8_t in = (PIND >> 2) & 0x1;
	if (in)
	{
		debugln("button 1 released");
	}
	else
	{
		debugln("button 1 pushed");
	}
}

int main(void)
{
	/* USART is on port D, pins 1-0 */
	USART_Init();

	/* button inputs are on port D, pins 7-2 */
	DDRD = 0x00; // 0=input 1=output
	PORTD = 0xfc; // pull-up resistors
	PCMSK2 = _BV(PCINT18); // enable PCINT18 (port B pin 2) contribution to PCINT2 interrupt
	PCICR = _BV(PCIE2); // enable PCINT2 interrupt

	/* button input is on port B */ 
	DDRB = 0xff; // 0=input 1=output

	debugln("Booting Erhune's super-giga-autofire...");
	debugln("...booted!");

	sei();
	while(1)
	{
		// waiting for interrupts...
	}
}

