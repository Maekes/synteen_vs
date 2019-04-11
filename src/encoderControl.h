#ifndef _encoder_h_
#define _encoder_h_

#define ENCODER_DO_NOT_USE_INTERRUPTS

#include <Encoder.h>
#include <values.h>
#include <Bounce.h>
#include <vector.h>
#include <graphics.h>
#include <analogFilter.h>

#define inhibitENC 38
#define eA 37
#define eB 36
#define eC 35

// LED
#define inhibitLED 17
#define eD 30
#define eE 29
#define eF 16


#define encodersA1 24
#define encodersB1 27
#define encodersA2 26
#define encodersB2 25

#define LFO_LED 39


void encoderLoop();
void encoderInit();

#endif
