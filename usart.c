#include "usart.h"
#include <avr/interrupt.h>
#include <avr/io.h>

static int uart_putchar(char c, FILE *stream);

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

#define BAUD 115200
#include <util/setbaud.h>

void USART_Init()
{
	/* Init USART clock */
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

	/* Register USART as standard output */
	stdout = &mystdout;
}

void USART_Transmit(uint8_t data)
{
	/* Wait for empty transmit buffer */
	while (!(UCSR0A & _BV(UDRE0)));
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

uint8_t USART_Receive()
{
	/* Wait for data to be received */
	while (!(UCSR0A & _BV(RXC0)));
	/* Get and return received data from buffer */
	return UDR0;
}

static int uart_putchar(char c, FILE *stream)
{
	if (c == '\n')
		uart_putchar('\r', stream);
	USART_Transmit(c);
    return 0;
}

/* interrupt vector for USART data reception */
ISR(USART_RX_vect)
{
	uint8_t c = USART_Receive();
	if (c == '\r')
	{
		printf("\nOK.\n");
	}
	else
	{
		USART_Transmit(c);
	}
}

