/*
 * autofire.h
 *
 * Created: 22/05/2011 23:39:42
 *  Author: Erhune
 */ 


#ifndef AUTOFIRE_H_
#define AUTOFIRE_H_

#include <avr/io.h>

void Autofire_Init();

void Autofire_P1B1_Changed(uint8_t v);
void Autofire_P1B2_Changed(uint8_t v);
void Autofire_P1B3_Changed(uint8_t v);
void Autofire_P1START_Changed(uint8_t v);

#endif /* AUTOFIRE_H_ */