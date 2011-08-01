/*
 * video.h
 *
 * Created: 30/07/2011 02:20:58
 *  Author: Erhune
 */ 


#ifndef VIDEO_H_
#define VIDEO_H_

#include <avr/io.h>
#include "config.h"

void Video_Init();

#ifdef ENABLE_VSYNC_DETECTION

void Video_Vsync_Triggered();

uint16_t Video_ComputeFps();

#endif /* ENABLE_VSYNC_DETECTION */

#ifdef ENABLE_BBP_DETECTION

void Video_Bbp_Triggered();

#endif /* ENABLE_BBP_DETECTION */

#endif /* VIDEO_H_ */