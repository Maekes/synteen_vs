#include <sdstore.h>

File myFile;
int16_t Waveforms_on_SD[4][257];
const int chipSelect = BUILTIN_SDCARD;

void sdstoreInit(){
 Serial.println("Initializing SD card...");
 if (!SD.begin(chipSelect)) {
   Serial.println("initialization failed!");
   return;
 }
 Serial.println("initialization done.");  // if file is present delete it before writing new data
}

void loadSDWavetable(int table, int corner){
  String wavetable;
  wavetable = String("WAVE") + table + ".TXT";

  Serial.println(wavetable);

  myFile = SD.open(wavetable.c_str());
  int arrayIndex = 0;
   if (myFile) {
    // read from the file
    char buffer[100]; // May need to be a bit bigger if you have long names
       byte index = 0;
       while (myFile.available())
       {
        char c = myFile.read();
           if(c == ','){
               if(arrayIndex < 257){
                 Waveforms_on_SD[corner][arrayIndex] = atoi(buffer);
               }
               arrayIndex++;
               index = 0;
               buffer[index] = '\0'; // Keep buffer NULL terminated
           }
           else{
               buffer[index++] = c;
               buffer[index] = '\0'; // Keep buffer NULL terminated
           }
       }
       myFile.close();                                                 // close the file:
     } else {
       Serial.println("error opening file");                // if the file didn't open, print an error:
     }

}

void storeVector(int bank){
  switch (bank) {
    case 1: SD.remove("vector1.txt"); myFile = SD.open("vector1.txt", FILE_WRITE);  break;
    case 2: SD.remove("vector2.txt"); myFile = SD.open("vector2.txt", FILE_WRITE);  break;
    case 3: SD.remove("vector3.txt"); myFile = SD.open("vector3.txt", FILE_WRITE);  break;
    case 4: SD.remove("vector4.txt"); myFile = SD.open("vector4.txt", FILE_WRITE);  break;
    case 5: SD.remove("vector5.txt"); myFile = SD.open("vector5.txt", FILE_WRITE);  break;
    case 6: SD.remove("vector6.txt"); myFile = SD.open("vector6.txt", FILE_WRITE);  break;
    case 7: SD.remove("vector7.txt"); myFile = SD.open("vector7.txt", FILE_WRITE);  break;
    case 8: SD.remove("vector8.txt"); myFile = SD.open("vector8.txt", FILE_WRITE);  break;
  }
   if (myFile){
       myFile.print("DrawnPoints = "); myFile.println(drawnPoints);
       myFile.print("PointX1 = "); myFile.println(xa[0]);
       myFile.print("PointY1 = "); myFile.println(ya[0]);
       myFile.print("PointX2 = "); myFile.println(xa[1]);
       myFile.print("PointY2 = "); myFile.println(ya[1]);
       myFile.print("PointX3 = "); myFile.println(xa[2]);
       myFile.print("PointY3 = "); myFile.println(ya[2]);
       myFile.print("PointX4 = "); myFile.println(xa[3]);
       myFile.print("PointY4 = "); myFile.println(ya[3]);

       myFile.close();                      // close the file:
    } else {
      Serial.println("error opening");                      // if the file didn't open, print an error:
    }
}

void storeAnalogfilter(int bank){
  switch (bank) {
   case 1: SD.remove("analog1.txt"); myFile = SD.open("analog1.txt", FILE_WRITE);  break;
   case 2: SD.remove("analog2.txt"); myFile = SD.open("analog2.txt", FILE_WRITE);  break;
   case 3: SD.remove("analog3.txt"); myFile = SD.open("analog3.txt", FILE_WRITE);  break;
   case 4: SD.remove("analog4.txt"); myFile = SD.open("analog4.txt", FILE_WRITE);  break;
   case 5: SD.remove("analog5.txt"); myFile = SD.open("analog5.txt", FILE_WRITE);  break;
   case 6: SD.remove("analog6.txt"); myFile = SD.open("analog6.txt", FILE_WRITE);  break;
   case 7: SD.remove("analog7.txt"); myFile = SD.open("analog7.txt", FILE_WRITE);  break;
   case 8: SD.remove("analog8.txt"); myFile = SD.open("analog8.txt", FILE_WRITE);  break;
 }
   if (myFile){
      myFile.print("AF_Gain = "); myFile.println(AF_Gain);
      myFile.print("AF_Cutoff = "); myFile.println(AF_Cutoff);
      myFile.print("AF_Resonance = "); myFile.println(AF_Resonance);

      myFile.close();                      // close the file:
    } else {
      Serial.println("error opening");                      // if the file didn't open, print an error:
    }
}

void storeOsc(int bank){
  switch (bank) {
   case 1: SD.remove("osc1.txt"); myFile = SD.open("osc1.txt", FILE_WRITE);  break;
   case 2: SD.remove("osc2.txt"); myFile = SD.open("osc2.txt", FILE_WRITE);  break;
   case 3: SD.remove("osc3.txt"); myFile = SD.open("osc3.txt", FILE_WRITE);  break;
   case 4: SD.remove("osc4.txt"); myFile = SD.open("osc4.txt", FILE_WRITE);  break;
   case 5: SD.remove("osc5.txt"); myFile = SD.open("osc5.txt", FILE_WRITE);  break;
   case 6: SD.remove("osc6.txt"); myFile = SD.open("osc6.txt", FILE_WRITE);  break;
   case 7: SD.remove("osc7.txt"); myFile = SD.open("osc7.txt", FILE_WRITE);  break;
   case 8: SD.remove("osc8.txt"); myFile = SD.open("osc8.txt", FILE_WRITE);  break;
 }
   if (myFile){
      myFile.print("OSC_ShapeA = ");myFile.println(OSC_Shape[0]);
      myFile.print("OSC_ShapeB = ");myFile.println(OSC_Shape[1]);
      myFile.print("OSC_ShapeC = ");myFile.println(OSC_Shape[2]);
      myFile.print("OSC_ShapeD = ");myFile.println(OSC_Shape[3]);

      myFile.print("OSC_TransA = ");myFile.println(OSC_Trans[0]);
      myFile.print("OSC_TransB = ");myFile.println(OSC_Trans[1]);
      myFile.print("OSC_TransC = ");myFile.println(OSC_Trans[2]);
      myFile.print("OSC_TransD = ");myFile.println(OSC_Trans[3]);

      myFile.print("OSC_TuneA = ");myFile.println(OSC_Tune[0]);
      myFile.print("OSC_TuneB = ");myFile.println(OSC_Tune[1]);
      myFile.print("OSC_TuneC = ");myFile.println(OSC_Tune[2]);
      myFile.print("OSC_TuneD = ");myFile.println(OSC_Tune[3]);

      myFile.print("OSC_Envelope_AttackA = ");myFile.println(OSC_Envelope_Attack[0]);
      myFile.print("OSC_Envelope_AttackB = ");myFile.println(OSC_Envelope_Attack[1]);
      myFile.print("OSC_Envelope_AttackC = ");myFile.println(OSC_Envelope_Attack[2]);
      myFile.print("OSC_Envelope_AttackD = ");myFile.println(OSC_Envelope_Attack[3]);

      myFile.print("OSC_Envelope_DecayA = ");myFile.println(OSC_Envelope_Decay[0]);
      myFile.print("OSC_Envelope_DecayB = ");myFile.println(OSC_Envelope_Decay[1]);
      myFile.print("OSC_Envelope_DecayC = ");myFile.println(OSC_Envelope_Decay[2]);
      myFile.print("OSC_Envelope_DecayD = ");myFile.println(OSC_Envelope_Decay[3]);

      myFile.print("OSC_Envelope_SustainA = ");myFile.println(OSC_Envelope_Sustain[0]);
      myFile.print("OSC_Envelope_SustainB = ");myFile.println(OSC_Envelope_Sustain[1]);
      myFile.print("OSC_Envelope_SustainC = ");myFile.println(OSC_Envelope_Sustain[2]);
      myFile.print("OSC_Envelope_SustainD = ");myFile.println(OSC_Envelope_Sustain[3]);

      myFile.print("OSC_Envelope_ReleaseA = ");myFile.println(OSC_Envelope_Release[0]);
      myFile.print("OSC_Envelope_ReleaseB = ");myFile.println(OSC_Envelope_Release[1]);
      myFile.print("OSC_Envelope_ReleaseC = ");myFile.println(OSC_Envelope_Release[2]);
      myFile.print("OSC_Envelope_ReleaseD = ");myFile.println(OSC_Envelope_Release[3]);

      myFile.print("LFO_ShapeA = ");myFile.println(LFO_Shape[0]);
      myFile.print("LFO_ShapeB = ");myFile.println(LFO_Shape[1]);
      myFile.print("LFO_ShapeC = ");myFile.println(LFO_Shape[2]);
      myFile.print("LFO_ShapeD = ");myFile.println(LFO_Shape[3]);

      myFile.print("LFO_SpeedA = ");myFile.println(LFO_Speed[0]);
      myFile.print("LFO_SpeedB = ");myFile.println(LFO_Speed[1]);
      myFile.print("LFO_SpeedC = ");myFile.println(LFO_Speed[2]);
      myFile.print("LFO_SpeedD = ");myFile.println(LFO_Speed[3]);

      myFile.print("LFO_Amplitude_DepthA = ");myFile.println(LFO_Amplitude_Depth[0]);
      myFile.print("LFO_Amplitude_DepthB = ");myFile.println(LFO_Amplitude_Depth[1]);
      myFile.print("LFO_Amplitude_DepthC = ");myFile.println(LFO_Amplitude_Depth[2]);
      myFile.print("LFO_Amplitude_DepthD = ");myFile.println(LFO_Amplitude_Depth[3]);

      myFile.print("LFO_Envelope_AttackA = ");myFile.println(LFO_Envelope_Attack[0]);
      myFile.print("LFO_Envelope_AttackB = ");myFile.println(LFO_Envelope_Attack[1]);
      myFile.print("LFO_Envelope_AttackC = ");myFile.println(LFO_Envelope_Attack[2]);
      myFile.print("LFO_Envelope_AttackD = ");myFile.println(LFO_Envelope_Attack[3]);

      myFile.print("LFO_Envelope_DecayA = ");myFile.println(LFO_Envelope_Decay[0]);
      myFile.print("LFO_Envelope_DecayB = ");myFile.println(LFO_Envelope_Decay[1]);
      myFile.print("LFO_Envelope_DecayC = ");myFile.println(LFO_Envelope_Decay[2]);
      myFile.print("LFO_Envelope_DecayD = ");myFile.println(LFO_Envelope_Decay[3]);

      myFile.print("LFO_Envelope_SustainA = ");myFile.println(LFO_Envelope_Sustain[0]);
      myFile.print("LFO_Envelope_SustainB = ");myFile.println(LFO_Envelope_Sustain[1]);
      myFile.print("LFO_Envelope_SustainC = ");myFile.println(LFO_Envelope_Sustain[2]);
      myFile.print("LFO_Envelope_SustainD = ");myFile.println(LFO_Envelope_Sustain[3]);

      myFile.print("LFO_Envelope_ReleaseA = ");myFile.println(LFO_Envelope_Decay[0]);
      myFile.print("LFO_Envelope_ReleaseB = ");myFile.println(LFO_Envelope_Decay[1]);
      myFile.print("LFO_Envelope_ReleaseC = ");myFile.println(LFO_Envelope_Decay[2]);
      myFile.print("LFO_Envelope_ReleaseD = ");myFile.println(LFO_Envelope_Decay[3]);

      myFile.close();                      // close the file:
    } else {
      Serial.println("error opening");                      // if the file didn't open, print an error:
    }
}



  void parseAndSave(char *buff){
     char *name = strtok(buff, " =");
     if(name){
         char *junk = strtok(NULL, " ");
         if(junk){
             char *valu = strtok(NULL, " ");
             if(valu){
                 float val = atof(valu);
                 if(strcmp(name, "OSC_ShapeA") == 0){ OSC_Shape[0] = val; }
                 else if(strcmp(name, "OSC_ShapeB") == 0){ OSC_Shape[1] = val; }
                 else if(strcmp(name, "OSC_ShapeC") == 0){ OSC_Shape[2] = val; }
                 else if(strcmp(name, "OSC_ShapeD") == 0){ OSC_Shape[3] = val; }

                 else if(strcmp(name, "OSC_TransA") == 0){ OSC_Trans[0] = val; }
                 else if(strcmp(name, "OSC_TransB") == 0){ OSC_Trans[1] = val; }
                 else if(strcmp(name, "OSC_TransC") == 0){ OSC_Trans[2] = val; }
                 else if(strcmp(name, "OSC_TransD") == 0){ OSC_Trans[3] = val; }

                 else if(strcmp(name, "OSC_TuneA") == 0){ OSC_Tune[0] = val; }
                 else if(strcmp(name, "OSC_TuneB") == 0){ OSC_Tune[1] = val; }
                 else if(strcmp(name, "OSC_TuneC") == 0){ OSC_Tune[2] = val; }
                 else if(strcmp(name, "OSC_TuneD") == 0){ OSC_Tune[3] = val; }

                 else if(strcmp(name, "OSC_Envelope_AttackA") == 0){ OSC_Envelope_Attack[0] = val; }
                 else if(strcmp(name, "OSC_Envelope_AttackB") == 0){ OSC_Envelope_Attack[1] = val; }
                 else if(strcmp(name, "OSC_Envelope_AttackC") == 0){ OSC_Envelope_Attack[2] = val; }
                 else if(strcmp(name, "OSC_Envelope_AttackD") == 0){ OSC_Envelope_Attack[3] = val; }

                 else if(strcmp(name, "OSC_Envelope_DecayA") == 0){ OSC_Envelope_Decay[0] = val; }
                 else if(strcmp(name, "OSC_Envelope_DecayB") == 0){ OSC_Envelope_Decay[1] = val; }
                 else if(strcmp(name, "OSC_Envelope_DecayC") == 0){ OSC_Envelope_Decay[2] = val; }
                 else if(strcmp(name, "OSC_Envelope_DecayD") == 0){ OSC_Envelope_Decay[3] = val; }

                 else if(strcmp(name, "OSC_Envelope_SustainA") == 0){ OSC_Envelope_Sustain[0] = val; }
                 else if(strcmp(name, "OSC_Envelope_SustainB") == 0){ OSC_Envelope_Sustain[1] = val; }
                 else if(strcmp(name, "OSC_Envelope_SustainC") == 0){ OSC_Envelope_Sustain[2] = val; }
                 else if(strcmp(name, "OSC_Envelope_SustainD") == 0){ OSC_Envelope_Sustain[3] = val; }

                 else if(strcmp(name, "OSC_Envelope_ReleaseA") == 0){ OSC_Envelope_Release[0] = val; }
                 else if(strcmp(name, "OSC_Envelope_ReleaseB") == 0){ OSC_Envelope_Release[1] = val; }
                 else if(strcmp(name, "OSC_Envelope_ReleaseC") == 0){ OSC_Envelope_Release[2] = val; }
                 else if(strcmp(name, "OSC_Envelope_ReleaseD") == 0){ OSC_Envelope_Release[3] = val; }

                 else if(strcmp(name, "LFO_ShapeA") == 0){ LFO_Shape[0] = val; }
                 else if(strcmp(name, "LFO_ShapeB") == 0){ LFO_Shape[1] = val; }
                 else if(strcmp(name, "LFO_ShapeC") == 0){ LFO_Shape[2] = val; }
                 else if(strcmp(name, "LFO_ShapeD") == 0){ LFO_Shape[3] = val; }

                 else if(strcmp(name, "LFO_SpeedA") == 0){ LFO_Speed[0] = val; }
                 else if(strcmp(name, "LFO_SpeedB") == 0){ LFO_Speed[1] = val; }
                 else if(strcmp(name, "LFO_SpeedC") == 0){ LFO_Speed[2] = val; }
                 else if(strcmp(name, "LFO_SpeedD") == 0){ LFO_Speed[3] = val; }

                 else if(strcmp(name, "LFO_Amplitude_DepthA") == 0){ LFO_Amplitude_Depth[0] = val; }
                 else if(strcmp(name, "LFO_Amplitude_DepthB") == 0){ LFO_Amplitude_Depth[1] = val; }
                 else if(strcmp(name, "LFO_Amplitude_DepthC") == 0){ LFO_Amplitude_Depth[2] = val; }
                 else if(strcmp(name, "LFO_Amplitude_DepthD") == 0){ LFO_Amplitude_Depth[3] = val; }

                 else if(strcmp(name, "LFO_Envelope_AttackA") == 0){ LFO_Envelope_Attack[0] = val; }
                 else if(strcmp(name, "LFO_Envelope_AttackB") == 0){ LFO_Envelope_Attack[1] = val; }
                 else if(strcmp(name, "LFO_Envelope_AttackC") == 0){ LFO_Envelope_Attack[2] = val; }
                 else if(strcmp(name, "LFO_Envelope_AttackD") == 0){ LFO_Envelope_Attack[3] = val; }

                 else if(strcmp(name, "LFO_Envelope_DecayA") == 0){ LFO_Envelope_Decay[0] = val; }
                 else if(strcmp(name, "LFO_Envelope_DecayB") == 0){ LFO_Envelope_Decay[1] = val; }
                 else if(strcmp(name, "LFO_Envelope_DecayC") == 0){ LFO_Envelope_Decay[2] = val; }
                 else if(strcmp(name, "LFO_Envelope_DecayD") == 0){ LFO_Envelope_Decay[3] = val; }

                 else if(strcmp(name, "LFO_Envelope_SustainA") == 0){ LFO_Envelope_Sustain[0] = val; }
                 else if(strcmp(name, "LFO_Envelope_SustainB") == 0){ LFO_Envelope_Sustain[1] = val; }
                 else if(strcmp(name, "LFO_Envelope_SustainC") == 0){ LFO_Envelope_Sustain[2] = val; }
                 else if(strcmp(name, "LFO_Envelope_SustainD") == 0){ LFO_Envelope_Sustain[3] = val; }

                 else if(strcmp(name, "LFO_Envelope_ReleaseA") == 0){ LFO_Envelope_Decay[0] = val; }
                 else if(strcmp(name, "LFO_Envelope_ReleaseB") == 0){ LFO_Envelope_Decay[1] = val; }
                 else if(strcmp(name, "LFO_Envelope_ReleaseC") == 0){ LFO_Envelope_Decay[2] = val; }
                 else if(strcmp(name, "LFO_Envelope_ReleaseD") == 0){ LFO_Envelope_Decay[3] = val; }

                 else if(strcmp(name, "AF_Gain") == 0){ AF_Gain = val; }
                 else if(strcmp(name, "AF_Cutoff") == 0){ AF_Cutoff = val; }
                 else if(strcmp(name, "AF_Resonance") == 0){ AF_Resonance = val; }

                 else if(strcmp(name, "NumPoints") == 0){ drawnPoints = val; }
                 else if(strcmp(name, "PointX1") == 0){ xa[0] = val; }
                 else if(strcmp(name, "PointY1") == 0){ ya[0] = val; }
                 else if(strcmp(name, "PointX2") == 0){ xa[1] = val; }
                 else if(strcmp(name, "PointY2") == 0){ ya[1] = val; }
                 else if(strcmp(name, "PointX3") == 0){ xa[2] = val; }
                 else if(strcmp(name, "PointY3") == 0){ ya[2] = val; }
                 else if(strcmp(name, "PointX4") == 0){ xa[3] = val; }
                 else if(strcmp(name, "PointY4") == 0){ ya[3] = val; }

             }
          }
       }
  }

  void reloadVector(int bank){
    switch (bank) {
      case 1: myFile = SD.open("vector1.txt");  break;
      case 2: myFile = SD.open("vector2.txt");  break;
      case 3: myFile = SD.open("vector3.txt");  break;
      case 4: myFile = SD.open("vector4.txt");  break;
      case 5: myFile = SD.open("vector5.txt");  break;
      case 6: myFile = SD.open("vector6.txt");  break;
      case 7: myFile = SD.open("vector7.txt");  break;
      case 8: myFile = SD.open("vector8.txt");  break;
    }
     if (myFile) {
      // read from the file
      char buffer[190]; // May need to be a bit bigger if you have long names
         byte index = 0;
         while (myFile.available())
         {
          char c = myFile.read();
             if(c == '\n' || c == '\r'){
                 parseAndSave(buffer);
                 Serial.println(buffer);
                 index = 0;
                 buffer[index] = '\0'; // Keep buffer NULL terminated
             }
             else{
                 buffer[index++] = c;
                 buffer[index] = '\0'; // Keep buffer NULL terminated
             }
         }
        vectorControllMode = 1;
         myFile.close();                                                 // close the file:
       } else {
         Serial.println("error opening test.txt");                // if the file didn't open, print an error:
       }
    }

    void reloadAnalogfilter(int bank){
      switch (bank) {
        case 1: myFile = SD.open("analog1.txt");  break;
        case 2: myFile = SD.open("analog2.txt");  break;
        case 3: myFile = SD.open("analog3.txt");  break;
        case 4: myFile = SD.open("analog4.txt");  break;
        case 5: myFile = SD.open("analog5.txt");  break;
        case 6: myFile = SD.open("analog6.txt");  break;
        case 7: myFile = SD.open("analog7.txt");  break;
        case 8: myFile = SD.open("analog8.txt");  break;
      }
       if (myFile) {
        // read from the file
        char buffer[190]; // May need to be a bit bigger if you have long names
           byte index = 0;
           while (myFile.available())
           {
            char c = myFile.read();
               if(c == '\n' || c == '\r'){
                   parseAndSave(buffer);
                   index = 0;
                   buffer[index] = '\0'; // Keep buffer NULL terminated
               }
               else{
                   buffer[index++] = c;
                   buffer[index] = '\0'; // Keep buffer NULL terminated
               }
           }

           myFile.close();                                                 // close the file:
         } else {
           Serial.println("error opening test.txt");                // if the file didn't open, print an error:
         }
    }


    void reloadOsc(int bank){
      switch (bank) {
        case 1: myFile = SD.open("osc1.txt");  break;
        case 2: myFile = SD.open("osc2.txt");  break;
        case 3: myFile = SD.open("osc3.txt");  break;
        case 4: myFile = SD.open("osc4.txt");  break;
        case 5: myFile = SD.open("osc5.txt");  break;
        case 6: myFile = SD.open("osc6.txt");  break;
        case 7: myFile = SD.open("osc7.txt");  break;
        case 8: myFile = SD.open("osc8.txt");  break;
      }
       if (myFile) {
        // read from the file
        char buffer[190]; // May need to be a bit bigger if you have long names
           byte index = 0;
           while (myFile.available())
           {
            char c = myFile.read();
               if(c == '\n' || c == '\r'){
                   parseAndSave(buffer);

                   index = 0;
                   buffer[index] = '\0'; // Keep buffer NULL terminated
               }
               else{
                   buffer[index++] = c;
                   buffer[index] = '\0'; // Keep buffer NULL terminated
               }
           }

           myFile.close();                                                 // close the file:
         } else {
           Serial.println("error opening file");                // if the file didn't open, print an error:
         }
      }
