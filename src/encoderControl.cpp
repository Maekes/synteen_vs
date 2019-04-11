#include <encoderControl.h>

Encoder myEnc[8] = {
  Encoder(encodersA1, encodersB1), Encoder(encodersA1, encodersB1), Encoder(encodersA1, encodersB1), Encoder(encodersA1, encodersB1),
  Encoder(encodersA1, encodersB1), Encoder(encodersA1, encodersB1), Encoder(encodersA1, encodersB1), Encoder(encodersA1, encodersB1)
};

Encoder myEnc2[8] = {
  Encoder(encodersA2, encodersB2), Encoder(encodersA2, encodersB2),Encoder(encodersA2, encodersB2),Encoder(encodersA2, encodersB2),
  Encoder(encodersA2, encodersB2),Encoder(encodersA2, encodersB2),Encoder(encodersA2, encodersB2),Encoder(encodersA2, encodersB2),
};

long position[8]  = {
  0, 0, 0, 0, 0, 0, 0, 0
};

long position2[8]  = {
  0, 0, 0, 0, 0, 0, 0, 0
};

void selectAddress(uint8_t a) {
  digitalWrite(inhibitENC, HIGH);
  digitalWrite(eA, a & 0x1);
  digitalWrite(eB, (a >> 1) & 0x1);
  digitalWrite(eC, (a >> 2) & 0x1);
  digitalWrite(inhibitENC, LOW);
}

void setLEDAdress(uint8_t a) {
  digitalWrite(inhibitLED, HIGH);
  digitalWrite(eD, a & 0x1);
  digitalWrite(eE, (a >> 1) & 0x1);
  digitalWrite(eF, (a >> 2) & 0x1);
  digitalWrite(inhibitLED, LOW);
}


Bounce MuteA = Bounce(encodersA2, 10);
Bounce MuteB = Bounce(encodersA2, 10);
Bounce MuteC = Bounce(encodersA2, 10);
Bounce MuteD = Bounce(encodersA2, 10);
Bounce OSCA = Bounce(encodersB2, 10);
Bounce OSCB = Bounce(encodersB2, 10);
Bounce OSCC = Bounce(encodersB2, 10);
Bounce OSCD = Bounce(encodersB2, 10);
Bounce LFO = Bounce(encodersB2, 10);


int delaym = 200;

// LED Steuerung
void LED(){
  if(OSC_Mute[0]){
    setLEDAdress(2);
    delayMicroseconds(delaym);
  }
  if(OSC_Mute[1]){
    setLEDAdress(4);
    delayMicroseconds(delaym);
  }
  if(OSC_Mute[2]){
    setLEDAdress(1);
    delayMicroseconds(delaym);
  }
  if(OSC_Mute[3]){
    setLEDAdress(6);
    delayMicroseconds(delaym);
  }


  switch (OSC_Selected) {
    case 0: setLEDAdress(0); delayMicroseconds(delaym); break;
    case 1: setLEDAdress(7); delayMicroseconds(delaym); break;
    case 2: setLEDAdress(3); delayMicroseconds(delaym); break;
    case 3: setLEDAdress(5); delayMicroseconds(delaym); break;
  }

}

void setScreenNumber(){
  if(LFO_Selected){
    screenNumber = 5;
  }else{
    screenNumber = 2;
  }
}

//Funktion, in der Buttons und Encoder ausgelesen werden
void readEnc(uint8_t enc) {

  // Auslesen und Updaten der Buttons
  if (enc >= 4 || enc == 2) {
      switch (enc) {
        case 2: if(OSCC.update()){if(OSCC.fallingEdge()){OSC_Selected = 2;  setScreenNumber();}}
                break;
        case 4: if(MuteD.update()){if(MuteD.fallingEdge()){changeMute(3);}}
                if(LFO.update()){if(LFO.fallingEdge()){changeLFO(); setScreenNumber(); if(LFO_Selected){digitalWrite(LFO_LED, HIGH);}else{digitalWrite(LFO_LED,LOW);}}}
                break;
        case 5: if(MuteA.update()) {if(MuteA.fallingEdge()){changeMute(0);}}
                if(OSCA.update()){if(OSCA.fallingEdge()){OSC_Selected = 0;  setScreenNumber();}}
                break;
        case 6: if(MuteC.update()) {if(MuteC.fallingEdge()){changeMute(2);}}
                if(OSCD.update()){if(OSCD.fallingEdge()){OSC_Selected = 3;  setScreenNumber();}}
                break;
        case 7: if(MuteB.update()) {if(MuteB.fallingEdge()){changeMute(1);}}
                if(OSCB.update()){if(OSCB.fallingEdge()){OSC_Selected = 1;  setScreenNumber();}}
                break;
      }
  }

  long newPos = myEnc[enc].read();
  long newPos2 = myEnc2[enc].read();

// für myEnc

  if (newPos != position[enc]) {
    position[enc] = newPos;
    if(position[enc] < 0) {
       Serial.print("1 DOWN ");
       Serial.println(enc);
       if(LFO_Selected){
         switch (enc) {
           case 0: if(LFO_Envelope_Sustain[OSC_Selected] > 0.01){LFO_Envelope_Sustain[OSC_Selected] -= 0.01; lfoEnvChangeSustain(OSC_Selected);} break;

           case 1: if(LFO_Envelope_Decay[OSC_Selected] > 0){if(LFO_Envelope_Decay[OSC_Selected]<=100){LFO_Envelope_Decay[OSC_Selected] -= 2.5;}
              else if(LFO_Envelope_Decay[OSC_Selected]>100 && LFO_Envelope_Decay[OSC_Selected]<=1000){LFO_Envelope_Decay[OSC_Selected] -= 25;}
              else if(LFO_Envelope_Decay[OSC_Selected]>1000){LFO_Envelope_Decay[OSC_Selected] -= 250;}
              lfoEnvChangeDecay(OSC_Selected);} break;
           case 2: if(LFO_Envelope_Attack[OSC_Selected] > 0){if(LFO_Envelope_Attack[OSC_Selected]<=100){LFO_Envelope_Attack[OSC_Selected] -= 2.5;}
              else if(LFO_Envelope_Attack[OSC_Selected]>100 && LFO_Envelope_Attack[OSC_Selected]<=1000){LFO_Envelope_Attack[OSC_Selected] -= 25;}
              else if(LFO_Envelope_Attack[OSC_Selected]>1000){LFO_Envelope_Attack[OSC_Selected] -= 250;}
              lfoEnvChangeAttack(OSC_Selected);} break;
           case 3: if(LFO_Envelope_Release[OSC_Selected] > 0){if(LFO_Envelope_Release[OSC_Selected]<=100){LFO_Envelope_Release[OSC_Selected] -= 2.5;}
              else if(LFO_Envelope_Release[OSC_Selected]>100 && LFO_Envelope_Release[OSC_Selected]<=1000){LFO_Envelope_Release[OSC_Selected] -= 25;}
              else if(LFO_Envelope_Release[OSC_Selected]>1000){LFO_Envelope_Release[OSC_Selected] -= 250;}
              lfoEnvChangeRelease(OSC_Selected);} break;
           //case 4: not used
           case 5: if(LFO_Shape[OSC_Selected] > 0){LFO_Shape[OSC_Selected] -= 0.25; lfoChangeShape(OSC_Selected);} break;
           case 6: if(LFO_Speed[OSC_Selected] > -8){LFO_Speed[OSC_Selected] -= 0.25; lfoChangeSpeed(OSC_Selected);} break;
           case 7: if(LFO_Amplitude_Depth[OSC_Selected] >= 0.01){LFO_Amplitude_Depth[OSC_Selected] -= 0.01; lfoChangeAmplitude(OSC_Selected);} break;
         }
       }else{
         switch (enc) {
           case 0: if(OSC_Envelope_Sustain[OSC_Selected] > 0.01){OSC_Envelope_Sustain[OSC_Selected] -= 0.01; envelopeChangeSustain(OSC_Selected);} break;

           case 1: if(OSC_Envelope_Decay[OSC_Selected] > 0){if(OSC_Envelope_Decay[OSC_Selected]<=100){OSC_Envelope_Decay[OSC_Selected] -= 2.5;}
              else if(OSC_Envelope_Decay[OSC_Selected]>100 && OSC_Envelope_Decay[OSC_Selected]<=1000){OSC_Envelope_Decay[OSC_Selected] -= 25;}
              else if(OSC_Envelope_Decay[OSC_Selected]>1000){OSC_Envelope_Decay[OSC_Selected] -= 250;}
              envelopeChangeDecay(OSC_Selected);} break;
           case 2: if(OSC_Envelope_Attack[OSC_Selected] > 0){if(OSC_Envelope_Attack[OSC_Selected]<=100){OSC_Envelope_Attack[OSC_Selected] -= 2.5;}
              else if(OSC_Envelope_Attack[OSC_Selected]>100 && OSC_Envelope_Attack[OSC_Selected]<=1000){OSC_Envelope_Attack[OSC_Selected] -= 25;}
              else if(OSC_Envelope_Attack[OSC_Selected]>1000){OSC_Envelope_Attack[OSC_Selected] -= 250;}
              envelopeChangeAttack(OSC_Selected);} break;
           case 3: if(OSC_Envelope_Release[OSC_Selected] > 0){if(OSC_Envelope_Release[OSC_Selected]<=100){OSC_Envelope_Release[OSC_Selected] -= 2.5;}
              else if(OSC_Envelope_Release[OSC_Selected]>100 && OSC_Envelope_Release[OSC_Selected]<=1000){OSC_Envelope_Release[OSC_Selected] -= 25;}
              else if(OSC_Envelope_Release[OSC_Selected]>1000){OSC_Envelope_Release[OSC_Selected] -= 250;}
              envelopeChangeRelease(OSC_Selected);} break;
           //case 4: not used
           case 5: if(OSC_Shape[OSC_Selected] > 0){OSC_Shape[OSC_Selected] -= 0.25; oscChangeShape(OSC_Selected);} break;
           case 6: if(OSC_Trans[OSC_Selected] > -48){OSC_Trans[OSC_Selected] -= 0.25; oscChangeTrans(OSC_Selected);} break;
           case 7: if(OSC_Tune[OSC_Selected] > -100){OSC_Tune[OSC_Selected] -= 0.25; oscChangeTune(OSC_Selected);} break;
         }
       }
    }
    if(position[enc] > 0) {
       Serial.print("1 UP ");
       Serial.println(enc);
       if(LFO_Selected){
         switch (enc) {
           case 0: if(LFO_Envelope_Sustain[OSC_Selected]< 1 ){LFO_Envelope_Sustain[OSC_Selected] += 0.01; lfoEnvChangeSustain(OSC_Selected);} break;

           case 1: if(LFO_Envelope_Decay[OSC_Selected] < 9000){if(LFO_Envelope_Decay[OSC_Selected]<100){LFO_Envelope_Decay[OSC_Selected] += 2.5;}
              else if(LFO_Envelope_Decay[OSC_Selected]>=100 && LFO_Envelope_Decay[OSC_Selected]<1000){LFO_Envelope_Decay[OSC_Selected] += 25;}
              else if(LFO_Envelope_Decay[OSC_Selected]>=1000){LFO_Envelope_Decay[OSC_Selected] += 250;}
              lfoEnvChangeDecay(OSC_Selected);} break;
           case 2: if(LFO_Envelope_Attack[OSC_Selected] < 9000){if(LFO_Envelope_Attack[OSC_Selected]<100){LFO_Envelope_Attack[OSC_Selected] += 2.5;}
              else if(LFO_Envelope_Attack[OSC_Selected]>=100 && LFO_Envelope_Attack[OSC_Selected]<1000){LFO_Envelope_Attack[OSC_Selected] += 25;}
              else if(LFO_Envelope_Attack[OSC_Selected]>=1000){LFO_Envelope_Attack[OSC_Selected] += 250;}
              lfoEnvChangeAttack(OSC_Selected);} break;
           case 3: if(LFO_Envelope_Release[OSC_Selected] < 9000){if(LFO_Envelope_Release[OSC_Selected]<100){LFO_Envelope_Release[OSC_Selected] += 2.5;}
              else if(LFO_Envelope_Release[OSC_Selected]>=100 && LFO_Envelope_Release[OSC_Selected]<1000){LFO_Envelope_Release[OSC_Selected] += 25;}
              else if(LFO_Envelope_Release[OSC_Selected]>=1000){LFO_Envelope_Release[OSC_Selected] += 250;}
              lfoEnvChangeRelease(OSC_Selected);} break;
           //case 4: not used
           case 5: if(LFO_Shape[OSC_Selected] < 127){LFO_Shape[OSC_Selected] += 0.25; lfoChangeShape(OSC_Selected);} break;
           case 6: if(LFO_Speed[OSC_Selected] < 8){LFO_Speed[OSC_Selected] += 0.25; lfoChangeSpeed(OSC_Selected);} break;
           case 7: if(LFO_Amplitude_Depth[OSC_Selected] < 1 ){LFO_Amplitude_Depth[OSC_Selected] += 0.01; lfoChangeAmplitude(OSC_Selected);} break;
         }
       }else{
         switch (enc) {
           case 0: if(OSC_Envelope_Sustain[OSC_Selected] < 1){OSC_Envelope_Sustain[OSC_Selected] += 0.01; envelopeChangeSustain(OSC_Selected);} break;

           case 1: if(OSC_Envelope_Decay[OSC_Selected] < 9000){if(OSC_Envelope_Decay[OSC_Selected]<100){OSC_Envelope_Decay[OSC_Selected] += 2.5;}
              else if(OSC_Envelope_Decay[OSC_Selected]>=100 && OSC_Envelope_Decay[OSC_Selected]<1000){OSC_Envelope_Decay[OSC_Selected] += 25;}
              else if(OSC_Envelope_Decay[OSC_Selected]>=1000){OSC_Envelope_Decay[OSC_Selected] += 250;}
              envelopeChangeDecay(OSC_Selected);} break;
          case 2: if(OSC_Envelope_Attack[OSC_Selected] < 9000){if(OSC_Envelope_Attack[OSC_Selected]<100){OSC_Envelope_Attack[OSC_Selected] += 2.5;}
             else if(OSC_Envelope_Attack[OSC_Selected]>=100 && OSC_Envelope_Attack[OSC_Selected]<1000){OSC_Envelope_Attack[OSC_Selected] += 25;}
             else if(OSC_Envelope_Attack[OSC_Selected]>=1000){OSC_Envelope_Attack[OSC_Selected] += 250;}
             envelopeChangeAttack(OSC_Selected);} break;
          case 3: if(OSC_Envelope_Release[OSC_Selected] < 9000){if(OSC_Envelope_Release[OSC_Selected]<100){OSC_Envelope_Release[OSC_Selected] += 2.5;}
             else if(OSC_Envelope_Release[OSC_Selected]>=100 && OSC_Envelope_Release[OSC_Selected]<1000){OSC_Envelope_Release[OSC_Selected] += 25;}
             else if(OSC_Envelope_Release[OSC_Selected]>=1000){OSC_Envelope_Release[OSC_Selected] += 250;}
             envelopeChangeRelease(OSC_Selected);} break;
            //case 4: not used
           case 5: if(OSC_Shape[OSC_Selected] < 127){OSC_Shape[OSC_Selected] += 0.25; oscChangeShape(OSC_Selected);} break;
           case 6: if(OSC_Trans[OSC_Selected] < 48){OSC_Trans[OSC_Selected] += 0.25; oscChangeTrans(OSC_Selected);} break;
           case 7: if(OSC_Tune[OSC_Selected] < 100){OSC_Tune[OSC_Selected] += 0.25; oscChangeTune(OSC_Selected);} break;
         }
       }
    }
    myEnc[enc].write(0);
    setScreenNumber();
  }

  // für myEnc2

  if (newPos2 != position2[enc]) {
    position2[enc] = newPos2;
    if(position2[enc] < 0) {
       Serial.print("2 DOWN");
       Serial.println(enc);
      /* testWert--;
       screenNumber = 5;
       drawText(10, 100, HX8357_WHITE,HX8357_BLACK, 3, testWert);
*/
       switch (enc) {
         case 0: if(AF_Resonance>0){AF_Resonance--; afChangeResonance(); Serial.print("AF_Resonance Change "); Serial.println(AF_Resonance);}
         break;
         case 1: if(AF_Cutoff>0){AF_Cutoff--; afChangeCutoff();}
         break;
         //case 2:
         case 3: if(AF_Gain>0){AF_Gain--; afChangeGain(); Serial.print("AF_Gain Change "); Serial.println(AF_Gain);}
         break;
         //case 4:
         //case 5:
         //case 6:
        // case

       }

    }
    if(position2[enc] > 0) {
       Serial.print("2 UP ");
       Serial.println(enc);
      /* screenNumber = 5;
       testWert++;
       drawText(10, 100, HX8357_WHITE,HX8357_BLACK, 3, testWert);
*/
       switch (enc) {
         case 0: if(AF_Resonance<100){AF_Resonance++; afChangeResonance(); Serial.print("AF_Resonance Change "); Serial.println(AF_Resonance);}
         break;
         case 1: if(AF_Cutoff<127){AF_Cutoff++; afChangeCutoff();}
         break;
         //case 2:
         case 3: if(AF_Gain<100){AF_Gain++; afChangeGain();Serial.print("AF_Gain Change "); Serial.println(AF_Gain);}
         break;
         //case 4:
         //case 5:
         //case 6:
        // case 7:

       }

    }
    myEnc2[enc].write(0);

  }
}


// Funktion zum Aufruf aller Encoder und Buttons
void readInputs(uint8_t i) {
    selectAddress(i);
    delayMicroseconds(30);
    readEnc(i);
}

// Funktion zur Initialisierung der Encoder und Button Pins
void encoderInit() {
  Serial.println("Start Encoder Init");
  // put your setup code here, to run once:
  pinMode(inhibitENC, OUTPUT);
  pinMode(eA, OUTPUT);
  pinMode(eB, OUTPUT);
  pinMode(eC, OUTPUT);

  pinMode(inhibitLED, OUTPUT);
  pinMode(eD, OUTPUT);
  pinMode(eE, OUTPUT);
  pinMode(eF, OUTPUT);
  pinMode(encodersA1, INPUT);
  pinMode(encodersB1, INPUT);
  pinMode(encodersA2, INPUT);
  pinMode(encodersB2, INPUT);


  pinMode(LFO_LED, OUTPUT);
  digitalWrite(LFO_LED, LOW);

  Serial.println("Setup finished");
}


void encoderLoop() {
  for (int i = 0; i < 8; i++) {
    readInputs(i);
  }
  LED();
}
