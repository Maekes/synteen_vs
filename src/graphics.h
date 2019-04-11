#ifndef _graphics_h_
#define _graphics_h_

#include <math.h>
#include <HX8357_t3.h>
#include <Adafruit_STMPE610.h>
#include <Values.h>
#include <EncoderControl.h>
#include <sdstore.h>

#define BUFFPIXEL 80

//colors
#define LIGHTBLUE 0x045F
#define LIGHTGREEN 0x070A
#define LIGHTRED 0xFA08
#define LIGHTYELLOW 0xFFE8

void tftInit();
void autoMover();
void show();
void checkTouch();
void drawText(int x, int y, uint16_t color,uint16_t background_color, int textSize, int value);
void drawText(int x, int y, uint16_t color,uint16_t background_color, int textSize, float value);
void drawText(int x, int y, uint16_t color,uint16_t background_color, int textSize, String value);
void deleteMute();
#endif
