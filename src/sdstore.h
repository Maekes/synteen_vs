#ifndef _sdstore_h_
#define _sdstore_h_

#include <SPI.h>
#include <SD.h>
#include <values.h>


void sdstoreInit();
void storeVector(int bank);
void storeAnalogfilter(int bank);
void storeOsc(int bank);

void reloadVector(int bank);
void reloadAnalogfilter(int bank);
void reloadOsc(int bank);

void loadSDWavetable(int table, int corner);

//void resetOsc(int bank);
//void resetAnalogfilter(int bank);

#endif
