#include <SPI.h>
#include <graphics.h>
#include <midiIn.h>
#include <encoderControl.h>
#include <vector.h>
#include <analogFilter.h>
#include <Values.h>
#include <sdstore.h>

#include <TimerOne.h>

int screenNumber ;

float  OSC_Shape[4];
float  OSC_Trans[4];
float  OSC_Tune[4];

float    OSC_Envelope_Attack[4];
float    OSC_Envelope_Decay[4];
float    OSC_Envelope_Sustain[4];   // 0.0 bis 1.0 (Schrittweite 0.01)
float    OSC_Envelope_Release[4];

float  LFO_Shape[4];
float  LFO_Speed[4];
float  LFO_Amplitude_Depth[4];

float    LFO_Envelope_Attack[4];
float    LFO_Envelope_Decay[4];
float    LFO_Envelope_Sustain[4];
float    LFO_Envelope_Release[4];

boolean  OSC_Mute[4];

int     AF_Cutoff;
int     AF_Resonance;
int     AF_Gain;
boolean AF_Active_Tracking;

boolean LFO_Selected;
int     OSC_Selected;

void setup() {

  Serial.begin(115200);
  Serial.println("Start with Initialisation");

  sdstoreInit(); //Initialize SD-Card
  pwmInit(); //Initialize Analog Filter
  tftInit(); //Initialize Touchscreen
  vectorInit(); //Initialize Vector Synthesis and Wavetables
  midiInit(); //Initialize MIDI-Input
  encoderInit(); //Initialize Encoders

  Serial.println("Full Initialisation Finish");

  //Sound for testing
  //playSounds();

  screenNumber  = 1;


}


void loop() {

  //unsigned long start = micros();
  encoderLoop();
  midiRead();
  calculateGains();
  show();
  checkTouch();
//  start = micros() - start;

  //Serial.println(start);

}
