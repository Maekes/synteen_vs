#include <graphics.h>

HX8357_t3 tft = HX8357_t3(TFT_CS, TFT_DC, TFT_RST, MOSI, SCK, MISO);
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

float vectorDisplaySize  = 250;
int xs = 25;  //Obere Linke Ecke des Quadrates (Offset)
int ys = 70;  //Obere Linke Ecke des Quadrates (Offset)
float xMem, yMem; //Speichert den vorangegangenen x und y Wert um die richtigen Pixel schwarz zu setzen beim neuzeichnen
int screenMem     = 1;  //Speichert den vorangegangenen Wert der Screennumber um bei änderunden zunächst das Display komplett schwarz zu setzen
int OSC_SelectedMem     = -1;

float maxi = vectorDisplaySize * vectorDisplaySize; //maximale größe der Fläche
int xd, yd;   //Umgrechenete Größen des Position im Display

int x_a = 1;
int y_a = 1;

int radius = 20;

TS_Point p;
TS_Point garbage;
boolean bounce = false;

const int numPoints = 4;
int drawnPoints = 0;
float xa[numPoints] = { }; //Alle gepeicherten Punkte für den autoMover
float ya[numPoints] = { };

int aktPoint = 0;

float bewegungsVektorX = 0;
float bewegungsVektorY = 0;

float alpha = 0.0;
float speedF = 0.01;

boolean starting = true;

void deleteMute(){
  tft.fillRect(0, 280, 480, 35, HX8357_BLACK);
}

void drawMenu(uint16_t backgroundST){
  tft.fillRect(0,0,180,60, HX8357_BLACK);
  tft.fillRect(181,0,100,60, LIGHTRED);
  tft.fillRect(281,0,100,60, LIGHTGREEN);
  tft.fillRect(381,0,100,60, backgroundST);


  tft.setCursor(200, 10);
  tft.setTextColor(HX8357_WHITE);
  tft.setTextSize(6);
  tft.print("VS");

  tft.setCursor(300, 10);
  tft.setTextColor(HX8357_WHITE);
  tft.setTextSize(6);
  tft.print("AF");

  if(screenNumber == 1){
    tft.setTextColor(backgroundST);
  }else{
    tft.setTextColor(HX8357_WHITE);
  }
  tft.setCursor(400, 10);
  tft.setTextSize(6);
  tft.print("ST");

  tft.drawFastVLine(180,0,60, HX8357_BLACK);
  tft.drawFastVLine(181,0,60, HX8357_BLACK);

  tft.drawFastVLine(280,0,60, HX8357_BLACK);
  tft.drawFastVLine(281,0,60, HX8357_BLACK);
  tft.drawFastVLine(380,0,60, HX8357_BLACK);
  tft.drawFastVLine(381,0,60, HX8357_BLACK);
}


void tftInit(){
  tft.begin(HX8357D);
  tft.setRotation(1);
  tft.fillScreen(HX8357_BLACK);
  //Menü
  drawMenu(HX8357_BLACK);

  if(!ts.begin()){
    Serial.println("Touchscreen could´t be initialised");
  }
  Serial.println("Display & Touchscreen Initalisation Finish");
}

void drawText(int x, int y, uint16_t color, uint16_t background_color, int textSize, int value){
    tft.setCursor(x, y);
    tft.setTextColor(color, background_color);
    tft.setTextSize(textSize);
    tft.print(value);
    tft.println(" ");
    encoderLoop();
  }

  void drawText(int x, int y, uint16_t color, uint16_t background_color, int textSize, float value){
      tft.setCursor(x, y);
      tft.setTextColor(color, background_color);
      tft.setTextSize(textSize);
      tft.print(value);
      tft.println(" ");
      encoderLoop();
    }

void drawText(int x, int y, uint16_t color, uint16_t background_color, int textSize, String value){
    tft.setCursor(x, y);
    tft.setTextColor(color, background_color);
    tft.setTextSize(textSize);
    tft.println(value);
    encoderLoop();
  }

void drawText(int x, int y, uint16_t color, int textSize, int value){
      tft.setCursor(x, y);
      tft.setTextColor(color);
      tft.setTextSize(textSize);
      tft.print(value);
      tft.println(" ");
      encoderLoop();
    }

void drawText(int x, int y, uint16_t color, int textSize, String value){
      tft.setCursor(x, y);
      tft.setTextColor(color);
      tft.setTextSize(textSize);
      tft.println(value);
      encoderLoop();
    }

  int counter = 0;

void drawVectorVisualisation(){

  drawText(5,5, LIGHTRED, 3, "Vector");
  drawText(5,35, LIGHTRED, 3, "Synthesis");

  drawText(0,ys, HX8357_WHITE, HX8357_BLACK, 4, "A");
  //drawText(0,ys, HX8357_WHITE, 1, GainA * 100 ); //Max 13 Characters

  drawText(xs+vectorDisplaySize+5,ys, HX8357_WHITE,HX8357_BLACK, 4, "B");
  //drawText(xs+vectorDisplaySize,ys, HX8357_WHITE, 1, GainB * 100);

  drawText(0,ys+vectorDisplaySize-32, HX8357_WHITE,HX8357_BLACK, 4, "C");
  //drawText(0,ys+vectorDisplaySize, HX8357_WHITE, 1, GainC * 100 );

  drawText(xs+vectorDisplaySize+5,ys+vectorDisplaySize-32, HX8357_WHITE,HX8357_BLACK, 4, "D");
  //drawText(xs+vectorDisplaySize,ys+vectorDisplaySize, HX8357_WHITE, 1, GainD * 100 );

  //normiertern Größen x & y auf Displaygrößen xd und yd umrechnen mit offset
  xd = (x * vectorDisplaySize) + xs;
  yd = (y * vectorDisplaySize) + ys;

  for (int n=0; n<drawnPoints; ++n){
   tft.fillCircle((xa[n]*vectorDisplaySize)+xs, (ya[n]*vectorDisplaySize)+ys, 10, LIGHTGREEN);

  }

   tft.drawRect(xs, ys, vectorDisplaySize, vectorDisplaySize, HX8357_WHITE);
   tft.drawRect(xs+1, ys+1, vectorDisplaySize-2, vectorDisplaySize-2, HX8357_WHITE);


   //Alte x und y Werte
   if(xMem != xd || yMem != yd ){
     tft.drawFastHLine(xs, yMem, vectorDisplaySize, HX8357_BLACK);
     tft.drawFastVLine(xMem, ys, vectorDisplaySize, HX8357_BLACK);
     tft.drawCircle(xMem,yMem,10,HX8357_BLACK);


   }

   //Neue x und y Werte
   tft.drawFastHLine(xs, yd, vectorDisplaySize, HX8357_WHITE);
   tft.drawFastVLine(xd, ys, vectorDisplaySize, HX8357_WHITE);
   tft.drawCircle(xd,yd,10,HX8357_WHITE);


   xMem = xd;
   yMem = yd;

   //Menü
   drawText(300,120, HX8357_WHITE,LIGHTRED, 5, "STORE");
   drawText(300,180, HX8357_WHITE,LIGHTRED, 5, "RELOAD");
   drawText(300,240, HX8357_WHITE,LIGHTRED, 5, "RESET");

}

void drawVectorVisualisationSimple(){

  drawText(0,ys, HX8357_WHITE, HX8357_BLACK, 4, "A");
  //drawText(0,ys, HX8357_WHITE, 1, GainA * 100 ); //Max 13 Characters
  drawText(xs+vectorDisplaySize+5,ys, HX8357_WHITE,HX8357_BLACK, 4, "B");
  //drawText(xs+vectorDisplaySize,ys, HX8357_WHITE, 1, GainB * 100);
  drawText(0,ys+vectorDisplaySize-32, HX8357_WHITE,HX8357_BLACK, 4, "C");
  //drawText(0,ys+vectorDisplaySize, HX8357_WHITE, 1, GainC * 100 );
  drawText(xs+vectorDisplaySize+5,ys+vectorDisplaySize-32, HX8357_WHITE,HX8357_BLACK, 4, "D");
  //drawText(xs+vectorDisplaySize,ys+vectorDisplaySize, HX8357_WHITE, 1, GainD * 100 );

  //normiertern Größen x & y auf Displaygrößen xd und yd umrechnen mit offset
  xd = (x * vectorDisplaySize) + xs;
  yd = (y * vectorDisplaySize) + ys;

  for (int n=0; n<drawnPoints; ++n){
   tft.fillCircle((xa[n]*vectorDisplaySize)+xs, (ya[n]*vectorDisplaySize)+ys, 10, LIGHTGREEN);

  }

   tft.drawRect(xs, ys, vectorDisplaySize, vectorDisplaySize, HX8357_WHITE);
   tft.drawRect(xs+1, ys+1, vectorDisplaySize-2, vectorDisplaySize-2, HX8357_WHITE);

   //Alte x und y Werte
   if(xMem != xd || yMem != yd ){
     tft.drawFastHLine(xs, yMem, vectorDisplaySize, HX8357_BLACK);
     tft.drawFastVLine(xMem, ys, vectorDisplaySize, HX8357_BLACK);
     tft.drawCircle(xMem,yMem,10,HX8357_BLACK);

     //Neue x und y Werte
     tft.drawFastHLine(xs, yd, vectorDisplaySize, HX8357_WHITE);
     tft.drawFastVLine(xd, ys, vectorDisplaySize, HX8357_WHITE);
     tft.drawCircle(xd,yd,10,HX8357_WHITE);

   }

   xMem = xd;
   yMem = yd;

}

void drawOSCOverview(){
  switch (OSC_Selected) {
    case 0: drawText(10,10, LIGHTBLUE, HX8357_BLACK, 5, "OSC-A"); break;
    case 1: drawText(10,10, LIGHTBLUE, HX8357_BLACK,5, "OSC-B"); break;
    case 2: drawText(10,10, LIGHTBLUE, HX8357_BLACK,5, "OSC-C"); break;
    case 3: drawText(10,10, LIGHTBLUE, HX8357_BLACK,5, "OSC-D"); break;
  }


  drawText(50,100, HX8357_WHITE,HX8357_BLACK, 4, (int) floor(OSC_Shape[OSC_Selected]));
  drawText(200,100, HX8357_WHITE,HX8357_BLACK, 4, (int) floor(OSC_Trans[OSC_Selected]));
  drawText(350,100, HX8357_WHITE,HX8357_BLACK, 4, (int) floor(OSC_Tune[OSC_Selected]));

  drawText(50,150, LIGHTBLUE,HX8357_BLACK, 3, "SHAPE");
  drawText(200,150, LIGHTBLUE,HX8357_BLACK, 3, "TRANS");
  drawText(350,150, LIGHTBLUE,HX8357_BLACK, 3, "TUNE");

  drawText(20,200, HX8357_WHITE,HX8357_BLACK, 4, getSteps(OSC_Envelope_Attack[OSC_Selected]));
  drawText(140,200, HX8357_WHITE,HX8357_BLACK, 4, getSteps(OSC_Envelope_Decay[OSC_Selected]));
  drawText(250,200, HX8357_WHITE,HX8357_BLACK, 4, OSC_Envelope_Sustain[OSC_Selected]);
  drawText(380,200, HX8357_WHITE,HX8357_BLACK, 4, getSteps(OSC_Envelope_Release[OSC_Selected]));

  drawText(20,250, LIGHTYELLOW,HX8357_BLACK, 2, "ATTACK");
  drawText(140,250, LIGHTYELLOW,HX8357_BLACK, 2, "DECAY");
  drawText(250,250, LIGHTYELLOW,HX8357_BLACK, 2, "SUSTAIN");
  drawText(380,250, LIGHTYELLOW,HX8357_BLACK, 2, "RELEASE");

  if(OSC_Mute[OSC_Selected]){
    drawText(0,280, HX8357_WHITE,LIGHTRED, 4, "        MUTED       ");
  }else{
    tft.fillRect(0, 280, 480, 35, HX8357_BLACK);
  }

}

void drawLFOOverview(){

  switch (OSC_Selected) {
    case 0: drawText(10,10, HX8357_WHITE, HX8357_BLACK,5, "LFO-A"); break;
    case 1: drawText(10,10, HX8357_WHITE, HX8357_BLACK,5, "LFO-B"); break;
    case 2: drawText(10,10, HX8357_WHITE, HX8357_BLACK,5, "LFO-C"); break;
    case 3: drawText(10,10, HX8357_WHITE, HX8357_BLACK,5, "LFO-D"); break;
  }


  drawText(50,100, HX8357_WHITE,HX8357_BLACK, 4,(int) floor(LFO_Shape[OSC_Selected]));

if(LFO_Speed[OSC_Selected]<0){
  drawText(200,100, HX8357_WHITE,HX8357_BLACK, 4, (float) pow(2, floor(LFO_Speed[OSC_Selected])));
}else{
  drawText(200,100, HX8357_WHITE,HX8357_BLACK, 4, (int) pow(2, floor(LFO_Speed[OSC_Selected])));
}
  drawText(350,100, HX8357_WHITE,HX8357_BLACK, 4, LFO_Amplitude_Depth[OSC_Selected]);

  drawText(50,150, LIGHTBLUE,HX8357_BLACK, 3, "SHAPE");
  drawText(200,150, LIGHTBLUE,HX8357_BLACK, 3, "SPEED");
  drawText(315,150, LIGHTBLUE,HX8357_BLACK, 3, "AMPLITUDE");

  drawText(20,200, HX8357_WHITE,HX8357_BLACK, 4, getSteps(LFO_Envelope_Attack[OSC_Selected]));
  drawText(140,200, HX8357_WHITE,HX8357_BLACK, 4, getSteps(LFO_Envelope_Decay[OSC_Selected]));
  drawText(250,200, HX8357_WHITE,HX8357_BLACK, 4, LFO_Envelope_Sustain[OSC_Selected]);
  drawText(380,200, HX8357_WHITE,HX8357_BLACK, 4, getSteps(LFO_Envelope_Release[OSC_Selected]));

  drawText(20,250, LIGHTYELLOW,HX8357_BLACK, 2, "ATTACK");
  drawText(140,250, LIGHTYELLOW,HX8357_BLACK, 2, "DECAY");
  drawText(250,250, LIGHTYELLOW,HX8357_BLACK, 2, "SUSTAIN");
  drawText(380,250, LIGHTYELLOW,HX8357_BLACK, 2, "RELEASE");

  if(OSC_Mute[OSC_Selected]){
    drawText(0,280, HX8357_WHITE,LIGHTRED, 4, "        MUTED       ");
  }else{
    tft.fillRect(0, 280, 480, 35, HX8357_BLACK);
  }

}


void drawAFOverview(){

  drawText(5,5, LIGHTGREEN, 3, "Analog");
  drawText(5,35, LIGHTGREEN, 3, "Filter");

  drawText(20,150, HX8357_WHITE,HX8357_BLACK, 5, AF_Cutoff);
  drawText(170,150, HX8357_WHITE,HX8357_BLACK, 5, AF_Resonance);
  drawText(350,150, HX8357_WHITE,HX8357_BLACK, 5, AF_Gain);

  drawText(20,200, LIGHTGREEN,HX8357_BLACK, 3, "CUTOFF");
  drawText(170,200, LIGHTGREEN,HX8357_BLACK, 3, "RESONANCE");
  drawText(350,200, LIGHTGREEN,HX8357_BLACK, 3, "GAIN");


}

void followPath(){

   if(bewegungsVektorX < 0){
       x -= cos(alpha)*speedF;
       y -= sin(alpha)*speedF;
    }else{
       x += cos(alpha)*speedF;
       y += sin(alpha)*speedF;
    }

    if(fabs(x - xa[aktPoint%drawnPoints]) >= fabs(bewegungsVektorX) && fabs(y - ya[aktPoint%drawnPoints]) >= fabs(bewegungsVektorY)){
      aktPoint++;
      x = xa[aktPoint%drawnPoints];
      y = ya[aktPoint%drawnPoints];
    }

   bewegungsVektorX = xa[(aktPoint+1)%drawnPoints] - xa[aktPoint%drawnPoints];
   bewegungsVektorY = ya[(aktPoint+1)%drawnPoints] - ya[aktPoint%drawnPoints];

   alpha = atan(bewegungsVektorY/bewegungsVektorX);

}

void drawStore_Reload_ResetOSC(){
    drawText(170,80, HX8357_WHITE,LIGHTBLUE, 6, "STORE");
    drawText(150,160, HX8357_WHITE,LIGHTBLUE, 6, "RELOAD");
    drawText(170,240, HX8357_WHITE,LIGHTBLUE, 6, "RESET");

    tft.fillRect(0,0,180,60, HX8357_BLACK);
    drawText(5,5, LIGHTBLUE, 5, "OSC");
}

void drawStoreOSC(){
    drawText(20,80, HX8357_WHITE,LIGHTBLUE, 6, "BANK 1");
    drawText(20,140, HX8357_WHITE,LIGHTBLUE, 6, "BANK 2");
    drawText(20,200, HX8357_WHITE,LIGHTBLUE, 6, "BANK 3");
    drawText(20,260, HX8357_WHITE,LIGHTBLUE, 6, "BANK 4");

    drawText(260,80, HX8357_WHITE,LIGHTBLUE, 6, "BANK 5");
    drawText(260,140, HX8357_WHITE,LIGHTBLUE, 6, "BANK 6");
    drawText(260,200, HX8357_WHITE,LIGHTBLUE, 6, "BANK 7");
    drawText(260,260, HX8357_WHITE,LIGHTBLUE, 6, "BANK 8");
    tft.fillRect(0,0,180,60, HX8357_BLACK);
    drawText(5,5, LIGHTBLUE, 3, "STORE");
    drawText(5,35, LIGHTBLUE, 3, "OSC");
}

void drawReloadOSC(){
  drawText(20,80, HX8357_WHITE,LIGHTBLUE, 6, "BANK 1");
  drawText(20,140, HX8357_WHITE,LIGHTBLUE, 6, "BANK 2");
  drawText(20,200, HX8357_WHITE,LIGHTBLUE, 6, "BANK 3");
  drawText(20,260, HX8357_WHITE,LIGHTBLUE, 6, "BANK 4");

  drawText(260,80, HX8357_WHITE,LIGHTBLUE, 6, "BANK 5");
  drawText(260,140, HX8357_WHITE,LIGHTBLUE, 6, "BANK 6");
  drawText(260,200, HX8357_WHITE,LIGHTBLUE, 6, "BANK 7");
  drawText(260,260, HX8357_WHITE,LIGHTBLUE, 6, "BANK 8");
  tft.fillRect(0,0,180,60, HX8357_BLACK);
  drawText(5,5, LIGHTBLUE, 3, "RELOAD");
  drawText(5,35, LIGHTBLUE, 3, "OSC");
}

void drawResetOSC(){
  drawText(20,80, HX8357_WHITE,LIGHTBLUE, 6, "BANK 1");
  drawText(20,140, HX8357_WHITE,LIGHTBLUE, 6, "BANK 2");
  drawText(20,200, HX8357_WHITE,LIGHTBLUE, 6, "BANK 3");
  drawText(20,260, HX8357_WHITE,LIGHTBLUE, 6, "BANK 4");

  drawText(260,80, HX8357_WHITE,LIGHTBLUE, 6, "BANK 5");
  drawText(260,140, HX8357_WHITE,LIGHTBLUE, 6, "BANK 6");
  drawText(260,200, HX8357_WHITE,LIGHTBLUE, 6, "BANK 7");
  drawText(260,260, HX8357_WHITE,LIGHTBLUE, 6, "BANK 8");
  tft.fillRect(0,0,180,60, HX8357_BLACK);
  drawText(5,5, LIGHTBLUE, 3, "RESET");
  drawText(5,35, LIGHTBLUE, 3, "OSC");
}

void drawStore_Reload_ResetAF(){
    drawText(170,80, HX8357_WHITE,LIGHTGREEN, 6, "STORE");
    drawText(150,160, HX8357_WHITE,LIGHTGREEN, 6, "RELOAD");
    drawText(170,240, HX8357_WHITE,LIGHTGREEN, 6, "RESET");


    tft.fillRect(0,0,180,60, HX8357_BLACK);
    drawText(5,5, LIGHTGREEN, 5, "AF");
}

void drawStoreAF(){
    drawText(20,80, HX8357_WHITE,LIGHTGREEN, 6, "BANK 1");
    drawText(20,140, HX8357_WHITE,LIGHTGREEN, 6, "BANK 2");
    drawText(20,200, HX8357_WHITE,LIGHTGREEN, 6, "BANK 3");
    drawText(20,260, HX8357_WHITE,LIGHTGREEN, 6, "BANK 4");

    drawText(260,80, HX8357_WHITE,LIGHTGREEN, 6, "BANK 5");
    drawText(260,140, HX8357_WHITE,LIGHTGREEN, 6, "BANK 6");
    drawText(260,200, HX8357_WHITE,LIGHTGREEN, 6, "BANK 7");
    drawText(260,260, HX8357_WHITE,LIGHTGREEN, 6, "BANK 8");

    tft.fillRect(0,0,180,60, HX8357_BLACK);
    drawText(5,5, LIGHTGREEN, 3, "STORE");
    drawText(5,35, LIGHTGREEN, 3, "AF");
}

void drawReloadAF(){
  drawText(20,80, HX8357_WHITE,LIGHTGREEN, 6, "BANK 1");
  drawText(20,140, HX8357_WHITE,LIGHTGREEN, 6, "BANK 2");
  drawText(20,200, HX8357_WHITE,LIGHTGREEN, 6, "BANK 3");
  drawText(20,260, HX8357_WHITE,LIGHTGREEN, 6, "BANK 4");

  drawText(260,80, HX8357_WHITE,LIGHTGREEN, 6, "BANK 5");
  drawText(260,140, HX8357_WHITE,LIGHTGREEN, 6, "BANK 6");
  drawText(260,200, HX8357_WHITE,LIGHTGREEN, 6, "BANK 7");
  drawText(260,260, HX8357_WHITE,LIGHTGREEN, 6, "BANK 8");

  tft.fillRect(0,0,180,60, HX8357_BLACK);
  drawText(5,5, LIGHTGREEN, 3, "RELOAD");
  drawText(5,35, LIGHTGREEN, 3, "AF");
}

void drawResetAF(){
  drawText(20,80, HX8357_WHITE,LIGHTGREEN, 6, "BANK 1");
  drawText(20,140, HX8357_WHITE,LIGHTGREEN, 6, "BANK 2");
  drawText(20,200, HX8357_WHITE,LIGHTGREEN, 6, "BANK 3");
  drawText(20,260, HX8357_WHITE,LIGHTGREEN, 6, "BANK 4");

  drawText(260,80, HX8357_WHITE,LIGHTGREEN, 6, "BANK 5");
  drawText(260,140, HX8357_WHITE,LIGHTGREEN, 6, "BANK 6");
  drawText(260,200, HX8357_WHITE,LIGHTGREEN, 6, "BANK 7");
  drawText(260,260, HX8357_WHITE,LIGHTGREEN, 6, "BANK 8");

  tft.fillRect(0,0,180,60, HX8357_BLACK);
  drawText(5,5, LIGHTGREEN, 3, "RESET");
  drawText(5,35, LIGHTGREEN, 3, "AF");
}



void drawStoreVECTOR(){
    drawText(20,80, HX8357_WHITE,LIGHTRED, 6, "BANK 1");
    drawText(20,140, HX8357_WHITE,LIGHTRED, 6, "BANK 2");
    drawText(20,200, HX8357_WHITE,LIGHTRED, 6, "BANK 3");
    drawText(20,260, HX8357_WHITE,LIGHTRED, 6, "BANK 4");

    drawText(260,80, HX8357_WHITE,LIGHTRED, 6, "BANK 5");
    drawText(260,140, HX8357_WHITE,LIGHTRED, 6, "BANK 6");
    drawText(260,200, HX8357_WHITE,LIGHTRED, 6, "BANK 7");
    drawText(260,260, HX8357_WHITE,LIGHTRED, 6, "BANK 8");

    tft.fillRect(0,0,180,60, HX8357_BLACK);
    drawText(5,5, LIGHTRED, 3, "STORE");
    drawText(5,35, LIGHTRED, 3, "VECTOR");
}

void drawReloadVECTOR(){
    drawText(20,80, HX8357_WHITE,LIGHTRED, 6, "BANK 1");
    drawText(20,140, HX8357_WHITE,LIGHTRED, 6, "BANK 2");
    drawText(20,200, HX8357_WHITE,LIGHTRED, 6, "BANK 3");
    drawText(20,260, HX8357_WHITE,LIGHTRED, 6, "BANK 4");

    drawText(260,80, HX8357_WHITE,LIGHTRED, 6, "BANK 5");
    drawText(260,140, HX8357_WHITE,LIGHTRED, 6, "BANK 6");
    drawText(260,200, HX8357_WHITE,LIGHTRED, 6, "BANK 7");
    drawText(260,260, HX8357_WHITE,LIGHTRED, 6, "BANK 8");

    tft.fillRect(0,0,180,60, HX8357_BLACK);
    drawText(5,5, LIGHTRED, 3, "RELOAD");
    drawText(5,35, LIGHTRED, 3, "VECTOR");
}


void show(){
  if(screenMem != screenNumber || OSC_Selected != OSC_SelectedMem){
    tft.fillRect(0,60,480,360, HX8357_BLACK);
    //Menü
    switch (screenNumber) {
        case 1: drawMenu(HX8357_BLACK); break;
        case 2: drawMenu(LIGHTBLUE); break;
        case 3: drawMenu(LIGHTGREEN); break;
        case 4: drawMenu(LIGHTRED); break;
        case 5: drawMenu(LIGHTBLUE); break;
    }

    //Main Page
    switch (screenNumber) {
        case 1: drawVectorVisualisation(); break;
        case 2: drawOSCOverview(); break;
        case 3: drawAFOverview(); break;
        case 4: break;  //ehemals Screen für einzelne Value
        case 5: drawLFOOverview(); break;
        case 6: drawStore_Reload_ResetOSC(); break;
        case 7: drawStoreOSC(); break;
        case 8: drawReloadOSC(); break;
        case 9: drawResetOSC(); break;
        case 10: drawStore_Reload_ResetAF(); break;
        case 11: drawStoreAF(); break;
        case 12: drawReloadAF(); break;
        case 13: drawResetAF(); break;
        case 14: drawStoreVECTOR(); break;
        case 15: drawReloadVECTOR(); break;
      //  drawText(0, 0, HX8357_WHITE, 5, "Hello");
    }
    screenMem = screenNumber;
    OSC_SelectedMem = OSC_Selected;

  } else if(screenNumber == 1){
      drawVectorVisualisationSimple();
  }


}

int screenBackup;

void checkTouch(){

  if (ts.touched() && bounce){

    //  bounce = false;
        p = ts.getPoint();
        p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.height());
        p.y = map(p.y, TS_MINX, TS_MAXX, 0, tft.width());


      p.x = tft.height() - p.x;
      Serial.print("touched   ");
      Serial.print(p.x);
      Serial.print("   ");
      Serial.println(p.y);
      if(p.y > 0){
        bounce = false;
      }
      if(screenNumber == 1) {
          // check if touch is inside of the square
          // x und y sind auf dem display vertauscht

          //Store
          if ( p.y >= 300 && p.x >= 120  && p.y <= 450 && p.x <= 160){
            screenBackup = screenNumber;
            screenNumber = 14;
            return;
          }

          //Reload
          if ( p.y >= 300 && p.x >= 180  && p.y <= 480 && p.x <= 240){
            screenBackup = screenNumber;
            screenNumber = 15;
              return;
          }

          //Reset
          if ( p.y >= 300 && p.x >= 240  && p.y <= 450 && p.x <= 280){
            for (int n=0; n<drawnPoints; ++n){
             tft.fillCircle((xa[n]*vectorDisplaySize)+xs, (ya[n]*vectorDisplaySize)+ys, 10, HX8357_BLACK);
            }
            drawnPoints = 0;
            vectorControllMode = 0;
              return;
          }

          if (drawnPoints < 4 && p.y <= xs+vectorDisplaySize && p.y >=xs && p.x <= ys+vectorDisplaySize && p.x >=ys) {
              vectorControllMode = 1;
              xa[drawnPoints] = (p.y -xs)/vectorDisplaySize ;
              ya[drawnPoints] = (p.x -ys)/vectorDisplaySize ;
              drawnPoints++;

              starting = true;
              return;
          }
        }

      //Nav Menü
      //x und y müssen vertauscht werden
      if ( p.y >= 180 && p.x >= 0  && p.x <= 60 && p.y <= 280){

        screenNumber = 1;
        return;
      }
      if ( p.y >= 280 && p.x >= 0  && p.x <= 60 && p.y <= 380){

        screenNumber = 3;
        return;
      }
      if ( p.y >= 380 && p.x >= 0  && p.x <= 60 && p.y <= 480){

        screenBackup = screenNumber;
        if(screenNumber == 3){
            screenNumber = 10; //SRR Analog Filter Dialog
        }else{
            screenNumber = 6; //SRR OSC Dialog
        }
        return;
      }

      //Store - Reload - Reset
      if(screenNumber == 6 || screenNumber == 10){
        if ( p.y >= 170 && p.x >= 80  && p.y <= 390 && p.x <= 130){

          screenNumber++;
          return;
        }

        if ( p.y >= 150 && p.x >= 160  && p.y <= 390 && p.x <= 210){

          screenNumber+=2;
          return;
        }

        if ( p.y >= 170 && p.x >= 240  && p.y <= 390 && p.x <= 290){

          screenNumber+=3;
          return;
        }
      }
      // Store OSC
      if(screenNumber >= 6 &&  screenNumber <= 15 ){

        int pushedBank = 0;
        //Bank 1
        if ( p.y >= 20 && p.x >= 80  && p.y <= 240 && p.x <= 130){
          pushedBank = 1;
        }

        //Bank 2
        else if ( p.y >= 20 && p.x >= 140  && p.y <= 240 && p.x <= 190){
          pushedBank = 2;
        }

        //Bank 3
        else if ( p.y >= 20 && p.x >= 200  && p.y <= 240 && p.x <= 250){
          pushedBank = 3;
        }

        //Bank 4
        else if ( p.y >= 20 && p.x >= 260  && p.y <= 240 && p.x <= 310){
          pushedBank = 4;
        }

        //Bank 5
        else if ( p.y >= 260 && p.x >= 80  && p.y <= 380 && p.x <= 130){
          pushedBank = 5;
        }

        //Bank 6
        else if ( p.y >= 260 && p.x >= 140  && p.y <= 380 && p.x <= 190){
          pushedBank = 6;
        }

        //Bank 7
        else if ( p.y >= 260 && p.x >= 200  && p.y <= 380 && p.x <= 250){
          pushedBank = 7;
        }

        //Bank 8
        else if ( p.y >= 260 && p.x >= 260  && p.y <= 380 && p.x <= 310){
          pushedBank = 8;
        }

        if(pushedBank != 0){
          switch (screenNumber) {
            case 7: storeOsc(pushedBank); break;
            case 8: reloadOsc(pushedBank); break;
            //case 9: resetOsc(pushedBank); break;
            case 11: storeAnalogfilter(pushedBank); break;
            case 12: reloadAnalogfilter(pushedBank); break;
            case 14: storeVector(pushedBank); break;
            case 15: reloadVector(pushedBank); break;
          }
          screenNumber = screenBackup;
        }
      }
    }else{
        // Retrieve a point
        if(!ts.touched() && !bounce){
          bounce = true;
        }else{
          //Serial.println(ts.getPoint().y);
          garbage = ts.getPoint();
          garbage = ts.getPoint();

        }
    }
}

void autoMover(){
    if(drawnPoints > 1){
      if(starting){
        x = xa[0];
        y = ya[0];
        starting = false;
      }
      followPath();
    }
}
