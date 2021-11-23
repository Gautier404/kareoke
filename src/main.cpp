#include <Arduino.h>


#include <Audio.h>

// GUItool: begin automatically generated code
AudioInputAnalog         adc1;           //xy=197,73
//AudioAnalyzeFFT1024      fft1024_1;      //xy=361,47
AudioOutputI2S           i2s1;           //xy=378,99
AudioAnalyzeNoteFrequency notefreq;
AudioConnection          patchCord1(adc1, 0, i2s1, 0);
AudioConnection          patchCord2(adc1, 0, i2s1, 1);
AudioConnection          patchCord3(adc1, notefreq);
AudioControlSGTL5000     sgtl5000_1;     //xy=265,161

// GUItool: end automatically generated code

void setup() {
  AudioMemory(30);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  while (!Serial) ; // wait for Arduino Serial Monitor
  Serial.println("FFT test");
  notefreq.begin(.05);
}

void loop() {
    if (notefreq.available()) {
        float note = notefreq.read();
        float prob = notefreq.probability();
        Serial.printf("Note: %3.2f | Probability: %.2f\n", note, prob);
        Serial.println();
    }
    
}