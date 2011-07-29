/*
 * video.h
 *
 * Created: 30/07/2011 02:20:58
 *  Author: Erhune
 */ 


#ifndef VIDEO_H_
#define VIDEO_H_

#include <avr/io.h>

void Video_Init();

#ifdef ENABLE_VSYNC_DETECTION
uint16_t computeFps();
#endif

#endif /* VIDEO_H_ */