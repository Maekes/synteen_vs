#include <vector.h>
#include <wavetable.h>

// GUItool: begin automatically generated code
AudioSynthWaveform       lfo[4];
AudioSynthWaveform       waveform[4];      //xy=603.0000457763672,147.3333339691162
AudioEffectEnvelope      lfoEnv[4];        //xy=777.3333282470703,692.3333282470703
AudioEffectEnvelope      envelope[4];      //xy=777.3333282470703,853.3333282470703
AudioEffectMultiply      multiply[4];        //xy=971.9047241210938,690.904748916626 // jetzt: multiply
AudioMixer4              mixer[4];
AudioMixer4              mixer1;         //xy=1423.3333702087402,511.6190276145935
AudioOutputI2S           i2s1;           //xy=1630.3333702087402,507.6190276145935

AudioConnection          patchCord1(lfo[2], lfoEnv[2]);
AudioConnection          patchCord2(waveform[0], envelope[0]);
AudioConnection          patchCord3(lfo[1], lfoEnv[1]);
AudioConnection          patchCord4(waveform[1], envelope[1]);
AudioConnection          patchCord5(lfo[0], lfoEnv[0]);
AudioConnection          patchCord6(waveform[2], envelope[2]);
AudioConnection          patchCord7(lfo[3], lfoEnv[3]);
AudioConnection          patchCord8(waveform[3], envelope[3]);
AudioConnection          patchCord9(lfoEnv[2], 0, multiply[2], 1);
AudioConnection          patchCord10(lfoEnv[0], 0, multiply[0], 1);
AudioConnection          patchCord11(envelope[3], 0, multiply[3], 0);
AudioConnection          patchCord12(envelope[3], 0, mixer[3], 0);
AudioConnection          patchCord13(envelope[2], 0, multiply[2], 0);
AudioConnection          patchCord14(envelope[2], 0, mixer[2], 0);
AudioConnection          patchCord15(lfoEnv[3], 0, multiply[3], 1);
AudioConnection          patchCord16(envelope[1], 0, multiply[1], 0);
AudioConnection          patchCord17(envelope[1], 0, mixer[1], 0);
AudioConnection          patchCord18(lfoEnv[1], 0, multiply[1], 1);
AudioConnection          patchCord19(envelope[0], 0, multiply[0], 0);
AudioConnection          patchCord20(envelope[0], 0, mixer[0], 0);
AudioConnection          patchCord21(multiply[2], 0, mixer[2], 1);
AudioConnection          patchCord22(multiply[3], 0, mixer[3], 1);
AudioConnection          patchCord23(multiply[1], 0, mixer[1], 1);
AudioConnection          patchCord24(multiply[0], 0, mixer[0], 1);
AudioConnection          patchCord25(mixer[3], 0, mixer1, 3);
AudioConnection          patchCord26(mixer[2], 0, mixer1, 2);
AudioConnection          patchCord27(mixer[0], 0, mixer1, 0);
AudioConnection          patchCord28(mixer[1], 0, mixer1, 1);
AudioConnection          patchCord29(mixer1, 0, i2s1, 0);
AudioConnection          patchCord30(mixer1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=1604.1904907226562,185.47618103027344
// GUItool: end automatically generated code

// Gain der einzelnen OSC-Signale (0=A, 1=B, ...) :
float oscGain[4];

// Array fuer Umrechnung: MIDI-Werte in Frequenzen:
float midi[127];

int   aktPlayNote = 200;
int   vectorControllMode;

float x;
float y;

// Für die Demo Vector Steuerung (Mode 2)
float x_dev = 50.0;
float y_dev = 50.0;
float xm = 0.8;
float ym = 1;


void playMidiSound(int note, int velocity){

  // falls Taste gedrückt (auf Keyboard)
  if(velocity != 0){
  aktPlayNote = note; // aktuell gespielte Note zwischenspeichern
  analogVCA(AF_Gain);

  AudioNoInterrupts();

    for (int i = 0; i < 4; i++) {

      waveform[i].amplitude((float)velocity/127); // Amplitude gekoppelt an velocity

      // Oszillatoren auf die gespielte Tonhoehe/Frequenz und den jeweiligen Shape setzen:
      switch((int)floor(OSC_Shape[i])){
        case 0: waveform[i].frequency(calculateFreq(i)); waveform[i].begin(WAVEFORM_SINE); break;
        case 1: waveform[i].frequency(calculateFreq(i)); waveform[i].begin(WAVEFORM_SQUARE); break;
        case 2: waveform[i].frequency(calculateFreq(i)); waveform[i].begin(WAVEFORM_TRIANGLE); break;
        case 3: waveform[i].frequency(calculateFreq(i)); waveform[i].begin(WAVEFORM_SAWTOOTH); break; // is in Audio Library Sawtooth Reverse
        case 4: waveform[i].frequency(calculateFreq(i)); waveform[i].begin(WAVEFORM_SAWTOOTH_REVERSE); break; // is in Audio Library Sawtooth
        default: waveform[i].frequency(calculateFreq(i)); waveform[i].begin(WAVEFORM_ARBITRARY); break;
      }

      // LFOs auf den jeweiligen Shape setzen (sofern ihre Amplitude ungleich 0):
      if(LFO_Amplitude_Depth[i] != 0){
        //fliter[i].frequency(midi[aktPlayNote]*4);
        switch((int)floor(LFO_Shape[i])){
          case 0: lfo[i].begin(WAVEFORM_SINE); break;
          case 1: lfo[i].begin(WAVEFORM_SQUARE); break;
          case 2: lfo[i].begin(WAVEFORM_TRIANGLE); break;
          case 3: lfo[i].begin(WAVEFORM_SAWTOOTH); break; // is in Audio Library Sawtooth Reverse
          case 4: lfo[i].begin(WAVEFORM_SAWTOOTH_REVERSE); break; // is in Audio Library Sawtooth
        }
      }
    }

  	if(AF_Active_Tracking){
  		analogVCF(AF_Cutoff+note-64);
      // analogVCF(AF_Cutoff+note-64+velocity-64);
    }

  	// Envelopes starten
  	for (int i = 0; i < 4; i++) {
        envelope[i].noteOn();
        lfoEnv[i].noteOn();
  	}
     AudioInterrupts();
  }
  //kein Ton
  else{
    if(note == aktPlayNote){
      for (int i = 0; i < 4; i++) {
        envelope[i].noteOff();
        lfoEnv[i].noteOff();
      }
    }
  }
}

// Vektorsynthese berechnen:
void calculateGains(){

  // Analog Filter Gain auf 0 setzten, falls kein Ton zu hören ist.
  if(!envelope[0].isActive() && !envelope[1].isActive() && !envelope[2].isActive() && !envelope[3].isActive()){
    analogVCA(0);
  }

  switch (vectorControllMode) {
    // 0 = Manuelle Stuerung über JoyStick, 1 = Automatische Steuerung über Wegpunkte
    case 0:{
      //x und y auslesen und Tiefpassfiltern um Rauschen zu reduzieren
      x = 0.2 * analogRead(JoyPinX) + 0.8 * (x*1023);
      y = 0.2 * analogRead(JoyPinY) + 0.8 * (y*1023);

      //Joystick geht nicht ganz auf 0, bzw. 1023, daher früher abschneiden
      if(x < 3) x = 0;
      if(y < 3) y = 0;

      if(x > 1020) x = 1023;
      if(y > 1020) y = 1023;

      //x und y auf 1 normieren
      x = (x / 1023);
      y = (y / 1023);
      break;
    }
    case 1:{
      autoMover();
      delay(20);
      break;
    }
    case 2:{
      //x und y auslesen
      if(x_dev > 100){
        xm = -0.8;
      }
      if(y_dev > 100){
        ym = -1;
      }
      if(x_dev < 0){
        xm = 0.8;
      }
      if(y_dev < 0){
        ym = 1;
      }

      x_dev = x_dev + xm;
      y_dev = y_dev + ym;

      //x und y auf 1 normieren
      x = (x_dev / 100);
      y = (y_dev / 100);
            break;
    }
  }

  //Gains anhand der Fläche berechnen | Max Fläche:  1 | x, y <= 1
  oscGain[0] = ((1-x) * (1-y));
  oscGain[1] = (x * (1-y));
  oscGain[2] = (x * y);
  oscGain[3] = ((1-x) * y);

  //Mixer1 auf den jeweiligen Gain setzen
  for (int i = 0; i < 4; i++) {
    if(!OSC_Mute[i]){
      mixer1.gain(i, oscGain[i]);
    }
  }

}



//--------- OSC: ---------

/* Change Shape (= Waveform) of OSC
params: 	int corner: 0 is A,
						1 is B,
						2 is C,
						3 is D.
*/


void oscChangeTrans(int corner){
  waveform[corner].frequency(calculateFreq(corner));
    drawText(200,100, HX8357_WHITE,HX8357_BLACK, 4, (int) floor(OSC_Trans[OSC_Selected]));
}

void oscChangeTune(int corner){
  waveform[corner].frequency(calculateFreq(corner));
    drawText(350,100, HX8357_WHITE,HX8357_BLACK, 4, (int)floor(OSC_Tune[OSC_Selected]));
}


float calculateFreq(int corner){
  float actFreq;
  actFreq = midi[aktPlayNote+(int)floor(OSC_Trans[corner])]*(float)pow(2, (double)floor(OSC_Tune[corner])/(100*12));
  return actFreq;

}

void oscChangeShape(int corner){

    if((int)floor(OSC_Shape[corner]) > 4){
    loadSDWavetable((int)floor(OSC_Shape[corner])-4, corner);
    for (int i = 0; i < 257; i++) {
      Serial.println(Waveforms_on_SD[0][i]);
    }
    switch (corner) {
      case 0:waveform[corner].arbitraryWaveform(Waveforms_on_SD[0], 172.0); break;
      case 1:waveform[corner].arbitraryWaveform(Waveforms_on_SD[1], 172.0); break;
      case 2:waveform[corner].arbitraryWaveform(Waveforms_on_SD[2], 172.0); break;
      case 3:waveform[corner].arbitraryWaveform(Waveforms_on_SD[3], 172.0); break;
      }
    }
    //waveform[corner].amplitude(1.0);
    drawText(50,100, HX8357_WHITE,HX8357_BLACK, 4,(int)floor(OSC_Shape[OSC_Selected]));
}

// Set OSC to MUTE
void changeMute(int corner){
  if(OSC_Mute[corner]){
	   OSC_Mute[corner] = false;
     mixer1.gain(corner, oscGain[corner]);
     deleteMute();
   }else{
     OSC_Mute[corner] = true;
     mixer1.gain(corner, 0);
     if(screenNumber == 2 || screenNumber == 5){
       drawText(0,280, HX8357_WHITE,LIGHTRED, 4, "        MUTED       ");
     }
   }
}


//--------- OSC-Envelope: ---------

// Change Attack-Time of OSC-Envelope
void envelopeChangeAttack(int corner){
  int temp = getSteps(OSC_Envelope_Attack[corner]);
  envelope[corner].attack(temp);
  drawText(20,200, HX8357_WHITE,HX8357_BLACK, 4, temp);
}

// Change Decay-Time of OSC-Envelope
void envelopeChangeDecay(int corner){
  int temp = getSteps(OSC_Envelope_Decay[corner]);
	envelope[corner].decay(temp);
  drawText(140,200, HX8357_WHITE,HX8357_BLACK, 4, temp);

}

// Change Sustain-Level of OSC-Envelop1e
void envelopeChangeSustain(int corner){
	envelope[corner].sustain(OSC_Envelope_Sustain[corner]);
  drawText(250,200, HX8357_WHITE,HX8357_BLACK, 4, OSC_Envelope_Sustain[OSC_Selected]);

}

// Change Release-Time of OSC-Envelope
void envelopeChangeRelease(int corner){
  int temp = getSteps(OSC_Envelope_Release[corner]);
	envelope[corner].release(temp);
  drawText(380,200, HX8357_WHITE,HX8357_BLACK, 4, temp);
  Serial.print(temp); Serial.print(" "); Serial.println(OSC_Envelope_Release[corner]);
}

// Runded die Encoder-Werte auf die gewünschten Schritte 0,10,20 ..100, 200, ... 1000, 2000...
int getSteps(float value){
  if(value <= 100){
    return (int) floor(value/10)*10;
  }else if(value > 100 && value <= 1000){
    return (int) floor(value/100)*100;
  }else if(value > 1000){
    return (int) floor(value/1000)*1000;
  }
}

//--------- LFO: ---------

/* Change Shape (= Waveform) of LFO
params: 	int corner: 0 is A,
						1 is B,
						2 is C,
						3 is D.
*/
void lfoChangeShape(int corner){

  drawText(50,100, HX8357_WHITE,HX8357_BLACK, 4,(int) LFO_Shape[OSC_Selected]);
}

// Change Speed (= Frequenz) of LFO
void lfoChangeSpeed(int corner){
	lfo[corner].frequency(pow(2,(int)floor(LFO_Speed[corner])));
  if(LFO_Speed[corner]<0){
    drawText(200,100, HX8357_WHITE,HX8357_BLACK, 4, (float) pow(2, floor(LFO_Speed[corner])));
  }else{
    drawText(200,100, HX8357_WHITE,HX8357_BLACK, 4, (int) pow(2, floor(LFO_Speed[corner])));
  }
}


// Change Amplitude (= Depth) of LFO
void lfoChangeAmplitude(int corner){
	lfo[corner].amplitude(LFO_Amplitude_Depth[corner]);
  lfo[corner].offset(1.0-LFO_Amplitude_Depth[corner]);

  drawText(350,100, HX8357_WHITE,HX8357_BLACK, 4, LFO_Amplitude_Depth[OSC_Selected]);

  if(LFO_Amplitude_Depth[corner] < 0.01){
    mixer[corner].gain(0,1);
    mixer[corner].gain(1,0);
  }
  else{
    mixer[corner].gain(0,0);
    mixer[corner].gain(1,1);
    Serial.println("test");
  }

}


//--------- LFO-: ---------

// Change Attack-Time of LFO-Envelope
void lfoEnvChangeAttack(int corner){
  int temp = getSteps(LFO_Envelope_Attack[corner]);
	lfoEnv[corner].attack(temp);
  drawText(20,200, HX8357_WHITE,HX8357_BLACK, 4, temp);

}

// Change Decay-Time of LFO-Envelope
void lfoEnvChangeDecay(int corner){
  int temp = getSteps(LFO_Envelope_Decay[corner]);
	lfoEnv[corner].decay(temp);
  drawText(140,200, HX8357_WHITE,HX8357_BLACK, 4, temp);

}

// Change Sustain-Level of LFO-Envelope
void lfoEnvChangeSustain(int corner){
	lfoEnv[corner].sustain(LFO_Envelope_Sustain[corner]);
  drawText(250,200, HX8357_WHITE,HX8357_BLACK, 4, LFO_Envelope_Sustain[OSC_Selected]);

}


// Change Release-Time of LFO-Envelope
void lfoEnvChangeRelease(int corner){
  int temp = getSteps(LFO_Envelope_Release[corner]);
	lfoEnv[corner].release(temp);
  drawText(380,200, HX8357_WHITE,HX8357_BLACK, 4, temp);

}

void changeLFO(){
	LFO_Selected = !LFO_Selected ;
}

//--------- Analog Filter: ---------

//TODO
// Change Cutoff-Frequency of Filter
void afChangeCutoff(){
  if(AF_Active_Tracking){
    analogVCF(AF_Cutoff+aktPlayNote-64);
  }
  else{
    analogVCF(AF_Cutoff);
  }
 screenNumber = 3;
 drawText(20,150, HX8357_WHITE,HX8357_BLACK, 5, AF_Cutoff);
	// Wert von 0 bis 127
}

// Change Resonance of Filter
void afChangeResonance(){
	analogRES(map(AF_Resonance, 0, 100, 0, 127));
	// Wert von 0 bis 127
   screenNumber = 3;
   drawText(170,150, HX8357_WHITE,HX8357_BLACK, 5, AF_Resonance);
}

// Change Gain of Filter
void afChangeGain(){
	analogVCA(map(AF_Gain, 0, 100, 0, 110));
	// Wert von 0 bis 127
   screenNumber = 3;
   drawText(350,150, HX8357_WHITE,HX8357_BLACK, 5, AF_Gain);
}


// Initialisierung der Vektorsynthese und der Waveforms/WAV-Files auf der SD-Karte:
void vectorInit(){
  AudioMemory(40);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0); //Nur Headphones-Out, betrifft nicht Line Out
  sgtl5000_1.lineOutLevel(27); // LineOutLevel setzen 27 -> 1.44 V p-p

  // Vector mittig setzen
  x = 0.5;
  y = 0.5;

  // VectorControllMode auf manuell setzen
  vectorControllMode = 0;

// Standard-Waveformen initialisieren:
for (int i = 0; i < 4; i++){
	OSC_Shape[i] = 0;
	OSC_Trans[i] = 0;
	OSC_Tune[i] = 0;

	oscChangeShape(i);
}


// OSC-Envelope:
// OSC-Envelope-Variablen auf Default setzen + Hold-Wert auf 0 setzen (wird nicht von einem Poti gesteuert):
for (int i = 0; i < 4; i++){
	OSC_Envelope_Attack[i] = 10;
	OSC_Envelope_Decay[i] = 30;
	OSC_Envelope_Sustain[i] = 1.0;   // 0.0 bis 1.0 (Schrittweite 0.01)
	OSC_Envelope_Release[i] = 300;

	envelope[i].hold(0);

	envelopeChangeAttack(i);
	envelopeChangeDecay(i);
	envelopeChangeRelease(i);
	envelopeChangeSustain(i);
}

// LFO:
// LFO-Variablen auf Default setzen
for (int i = 0; i < 4; i++){
	LFO_Shape[i] = 0;
	LFO_Speed[i] = 0;
	LFO_Amplitude_Depth[i] = 0.0;

	lfoChangeShape(i);
	lfoChangeSpeed(i);
	lfoChangeAmplitude(i);
  //lfo[i].offset(0.5);

  //filter[i].octaveControl(2.5);
}

// LFO-Envelope:
// LFO-Envelope-Variablen auf Default setzen + Hold-Wert auf 0 setzen (wird nicht von einem Poti gesteuert):
for (int i = 0; i < 4; i++){
	LFO_Envelope_Attack[i] = 10;
	LFO_Envelope_Decay[i] = 30;
	LFO_Envelope_Sustain[i] = 1.0;   // 0.0 bis 1.0 (Schrittweite 0.01)
	LFO_Envelope_Release[i] = 300;

	lfoEnv[i].hold(0);

	lfoEnvChangeAttack(i);
	lfoEnvChangeDecay(i);
	lfoEnvChangeRelease(i);
	lfoEnvChangeSustain(i);
}


// Umrechnung MIDI-Werte in Frequenz:
   float a = 440; // a is 440 hz...
   for (int x = 0; x < 127; ++x)
   {
     midi[x] = a  * pow(2, ((float)x - 69) / 12 );
   }

// Analoger Filter: Default-Werte setzen:
   AF_Cutoff =127;
   afChangeCutoff();
   AF_Gain = 50;
   afChangeGain();
   AF_Resonance = 0;
   afChangeResonance();

   AF_Active_Tracking = true;

// LFO als Default auf AUS schalten:
   for (int i = 0; i < 4; i++){
     mixer[i].gain(0,1);
     mixer[i].gain(1,0);
   }
}
