#ifndef _vector_h_
#define _vector_h_

#include <Audio.h>
#include <Wire.h>

#include <Values.h>
#include <graphics.h>
#include <analogFilter.h>

#include <Flute_100kbyte_samples.h>
#include <FrenchHorns_samples.h>
#include <Pizzicato_samples.h>
#include <Viola_samples.h>


void vectorInit();
void playSounds();
void calculateGains();
void playMidiSound(int note, int velocity);
float calculateFreq(int number);

void changeMute(int number);
void changeLFO();

void afChangeCutoff();
void afChangeGain();
void afChangeResonance();

void envelopeChangeAttack(int number);
void envelopeChangeDecay(int number);
void envelopeChangeSustain(int number);
void envelopeChangeRelease(int number);

int  getSteps(float value);
void lfoChangeShape(int number);
void lfoChangeSpeed(int number);
void lfoChangeAmplitude(int number);

void lfoEnvChangeAttack(int number);
void lfoEnvChangeDecay(int number);
void lfoEnvChangeRelease(int number);
void lfoEnvChangeSustain(int number);

void oscChangeShape(int number);
void oscChangeTrans(int number);
void oscChangeTune(int number);

#endif
