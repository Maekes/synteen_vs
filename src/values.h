#ifndef _values_h_
#define _values_h_

//********************************
//    PINBELEGUNG
//********************************

//Joysickt
#define JoyPinX  33
#define JoyPinY  34

//Audioboard
#define AF_Cutoff_Pin  5
#define AF_Resonance_Pin 4
#define AF_Gain_Pin 2


//Display
#define STMPE_CS 3
#define TFT_DC 20
#define TFT_CS 21

#define MOSI 7
#define MISO 12
#define SCK  14
#define TFT_RST -1

//********************************
//    WEITERE DEFINES
//********************************

#define TS_MINX 150
#define TS_MINY 130
#define TS_MAXX 3800
#define TS_MAXY 4000

//********************************
//    GLOBALE VARIABLEN
//********************************

extern float    GainA, GainB, GainC, GainD; //Gain der 4 Wavetables
extern int      aktPlayNote;  //Note die gerade zu hören ist
extern float    x, y;  //x,y Position des Vektorsynthesizers normiert auf 1

extern int      vectorControllMode; // Modus in der der Vatkorsynthesizer gesteuert wird. 0 = Manuell ; 1 = autoMover
extern float    vectorDisplaySize;  // Größe einer Seitenlänge des Quadrates des Vektorsynthesizers auf dem Display
extern int      screenNumber; //Welcher Screen wird gerade angezeigt
// 1 = Vector Visualisation
extern const int numPoints;
extern int drawnPoints;
extern float xa[4]; //Alle gepeicherten Punkte für den autoMover
extern float ya[4];

extern float  	OSC_Shape[4];
extern float    OSC_Trans[4];
extern float    OSC_Tune[4];

extern float    OSC_Envelope_Attack[4];
extern float    OSC_Envelope_Decay[4];
extern float    OSC_Envelope_Sustain[4];   // 0.0 bis 1.0 (Schrittweite 0.01)
extern float    OSC_Envelope_Release[4];

extern float    LFO_Shape[4];
extern float    LFO_Speed[4];
extern float    LFO_Amplitude_Depth[4];

extern float  LFO_Envelope_Attack[4];
extern float  LFO_Envelope_Decay[4];
extern float  LFO_Envelope_Sustain[4];
extern float  LFO_Envelope_Release[4];

extern boolean OSC_Mute[4];

extern int     AF_Cutoff;
extern int     AF_Resonance;
extern int     AF_Gain;
extern boolean AF_Active_Tracking;

extern int16_t Waveforms_on_SD[4][257];

extern int      OSC_Selected; //Wert wischen 0 und 3
extern boolean  LFO_Selected;  //Ist LFO an

#endif
