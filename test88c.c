#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

static int uart_putchar(char c, FILE *stream);

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

uint8_t buttonStatus = 0x00;
uint16_t previousTimer = 0;

void USART_Init()
{
	/* Init USART clock */
	#define BAUD 115200
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

static int uart_putchar(char c, FILE *stream)
{
	if (c == '\n')
		uart_putchar('\r', stream);
	USART_Transmit((unsigned char) c);
    return 0;
}

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

ISR(USART_RX_vect)
{
	unsigned char c = USART_Receive();
	if (c == '\r')
	{
		printf("\nOK.\n");
	}
	else
	{
		USART_Transmit(c);
	}
}

ISR(PCINT2_vect)
{
	updateOutput();
}

int main(void)
{
	/* USART is on port D, pins 1-0 */
	USART_Init();
	stdout = &mystdout;

	/* button inputs are on port D, pins 7-2 */
	DDRD = 0x00; // 0=input 1=output
	PORTD = 0xfc; // pull-up resistors
	PCMSK2 = _BV(PCINT18); // enable PCINT18 (port B pin 2) contribution to PCINT2 interrupt
	//PCICR = _BV(PCIE2); // enable PCINT2 interrupt

	/* button input is on port B */ 
	DDRB = 0xff; // 0=input 1=output

	/* set-up timer 1 to "enabled without prescaler" */
	TCCR1B |= _BV(CS11) | _BV(CS10); // div64

	printf("Booting Erhune's super-giga-autofire...\n");

	//sei();
	while(1)
	{
		// waiting for interrupts...
		_delay_ms(10);
		uint16_t timer = TCNT1;
		uint8_t in = (PIND >> 2) & 0x1;
		printf("%u,%d\n", timer, in);
	}
}

