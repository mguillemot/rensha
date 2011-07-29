/*
 * autofire.c
 *
 * Created: 22/05/2011 23:39:59
 *  Author: Erhune
 */ 

#include "autofire.h"

static struct Profile profile;

void Autofire_Init()
{
	printf("a profile is %d bytes long\n", sizeof(struct Profile));
	profile.name[0] = 'B';
	profile.name[1] = 'a';
	profile.name[2] = 't';
	profile.name[3] = 't';
	profile.name[4] = 'l';
	profile.name[5] = 'e';
	profile.name[6] = ' ';
	profile.name[7] = 'G';
	profile.name[8] = 'a';
	profile.name[9] = 'r';
	profile.name[10] = 'e';
	profile.name[11] = 'g';
	profile.name[12] = 'g';
	profile.name[13] = 'a';
	profile.name[14] = 0;
	profile.name[15] = 0;
	for (uint8_t i = 0; i < BUTTON_COUNT; ++i)
	{
		profile.button[i].type = AFTYPE_NORMAL;
	}
	printf("Profile %s loaded.\n", profile.name);
}

void Autofire_LoadProfile(struct profile *p)
{
		
}

void Autofire_Update()
{
	
	
}