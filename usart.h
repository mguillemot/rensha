#ifndef USART_H_
#define USART_H_

#include <stdio.h>

#include "config.h" // will define BAUD that will be used by setbaud.h
#include <util/setbaud.h>

void USART_Init();

#endif /* USART_H_ */
