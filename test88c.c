#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char buttonOutput = 0x02;

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

void debugln(const char *text)
{
	debug(text);
	USART_Transmit('\r');
	USART_Transmit('\n');
}

ISR(USART_RX_vect)
{
	unsigned char c = USART_Receive();
	if (c == '\r')
	{
		debugln("");
		debugln("OK.");
	}
	else
	{
		USART_Transmit(c);
	}
}

int main(void)
{
	/* USART is on port D */
	USART_Init();

	/* port B is button1out (bit 0) & button1in (bit1) */ 
	DDRB = 0x01; 
	PORTB = 0;

	debugln("Booting Erhune's super-giga-autofire...");

	sei();
	while(1)
	{
		_delay_ms(1000);
		
	}
}

