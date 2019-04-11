#include <analogFilter.h>


void pwmInit(){

  pinMode(AF_Cutoff_Pin, OUTPUT);
  pinMode(AF_Resonance_Pin, OUTPUT);
  pinMode(AF_Gain_Pin, OUTPUT);

  //Auflösung der PWM Spannung definieren, und PWM Basisfrequenz setzen
  analogWriteResolution(7); // 7 Bit, 0 bis 127
  analogWriteFrequency(AF_Cutoff_Pin, 39215); // idealwert PWM 7 bit (468750 Hz)
                                              // oder original shruti frequenz (39215 Hz)
  analogWriteFrequency(AF_Resonance_Pin, 39215);
  analogWriteFrequency(AF_Gain_Pin, 39215);
}


void analogVCA(int value){
    analogWrite(AF_Gain_Pin, value);
}

void analogVCF(int value){
    analogWrite(AF_Cutoff_Pin, value);
}

void analogRES(int value){
    analogWrite(AF_Resonance_Pin, value);
}
