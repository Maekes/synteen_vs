#include <midiIn.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial4, MIDI);

void midiInit(){
   MIDI.begin(MIDI_CHANNEL_OMNI);
}

unsigned long t = 0;

void midiRead() {
  int note, velocity, channel, d1, d2;
  if (MIDI.read()) {                    // Is there a MIDI message incoming ?
    byte type = MIDI.getType();
    switch (type) {
      case midi::NoteOn:
        note = MIDI.getData1();
        velocity = MIDI.getData2();
        channel = MIDI.getChannel();
        if (velocity > 0) {
          playMidiSound(note, velocity);
          Serial.println(String("Message, type=") + type + ", velocity = " + velocity + "   note= " + note);
        } else {
          playMidiSound(note, 0);
        }
        break;
      case midi::NoteOff:
        note = MIDI.getData1();
        velocity = MIDI.getData2();
        channel = MIDI.getChannel();
        playMidiSound(note, 0);
        break;
      default:
        d1 = MIDI.getData1();
        d2 = MIDI.getData2();
        Serial.println(String("Message, type=") + type + ", data = " + d1 + " " + d2);
    }
    t = millis();
  }
  if (millis() - t > 10000) {
    t += 10000;
    Serial.println("(inactivity)");
  }
}
