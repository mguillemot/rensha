/*
 * autofire.h
 *
 * Created: 22/05/2011 23:39:42
 *  Author: Erhune
 */ 


#ifndef AUTOFIRE_H_
#define AUTOFIRE_H_

#include <avr/io.h>

#define BUTTON_COUNT 22
#define PROFILE_NAME_LENGTH 16

enum
{
	BUT_P1_UP,
	BUT_P1_DOWN,
	BUT_P1_LEFT,
	BUT_P1_RIGHT,
	BUT_P1_1,
	BUT_P1_2,
	BUT_P1_3,
	BUT_P1_4,
	BUT_P1_START,
	BUT_P2_UP,
	BUT_P2_DOWN,
	BUT_P2_LEFT,
	BUT_P2_RIGHT,
	BUT_P2_1,
	BUT_P2_2,
	BUT_P2_3,
	BUT_P2_4,
	BUT_P2_START,
	BUT_TEST,
	BUT_SERVICE,
	BUT_COIN1,
	BUT_COIN2
};

enum
{
	AFTYPE_NORMAL,
	AFTYPE_IMMEDIATE_AUTOFIRE,
	AFTYPE_SYNCRONIZED_AUTOFIRE
};

struct ButtonConfig 
{
	uint8_t type; // enum
	uint32_t outputs; // 1 bit per button
	uint8_t frequency; // every N frame
};

typedef struct Profile
{
	char name[PROFILE_NAME_LENGTH];
	struct ButtonConfig button[BUTTON_COUNT];
};

void Autofire_Init();

void Autofire_Update();

#endif /* AUTOFIRE_H_ */