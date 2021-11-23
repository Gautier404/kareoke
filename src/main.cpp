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

int speaker = 15;

// A, F#, C, D, G
int notes[5] = {220, 185, 262, 147, 800};

int testTone(int note, int speaker);
void showResults(int red, int orange, int green);
void administerTest(int speaker);

void setup() {
  AudioMemory(30);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  while (!Serial) ; // wait for Arduino Serial Monitor
  Serial.println("FFT test");
  notefreq.begin(.05);
  administerTest(speaker);
}

void loop() {
    
}

int testTone(int note, int speaker){
    //play the desired tone for a few seconds
    Serial.println("Playing Desired Tone");
    Serial.println("____________________");
    tone(speaker, note);       // Begin the tone
    for(int i = 0; i<20; i++){
        Serial.print("*");
        delay(100);
    }
    noTone(speaker);
    Serial.println("");
    
    Serial.println("Sing back the note you heard");
    
    Serial.println("___");
    for(int i = 0; i<3; i++){
        Serial.print("*");
        delay(500);
    }
    Serial.println("");

    Serial.println("____________________");
    //ask the user to sing back the tone and listen
    //if the there is a tone available and it falls within +-100 Hz of desired tone add it to a tone total
    float recordedNote;
    float dataPoints = 0;
    float total = 0;
    float diff;
    int error = note/4;

    unsigned long time = millis();
    while(millis()-time <2000){
        if (notefreq.available()){
            recordedNote = notefreq.read();
            Serial.println(recordedNote);

            diff = abs(recordedNote - note);

            if (diff < error){
                total += recordedNote;
                dataPoints++;
            }
        }
    }
    
    //record number of data points

    //after 5 seconds divide the tone total by the number of data points and return it
    return total/dataPoints;
}

void administerTest(int speaker){
    int results[5];
    for (int i = 0; i<5; i++){
        results[i] = testTone(notes[i], speaker);
    }
    for (int i = 0; i<5; i++){
        Serial.print(notes[i]);
        Serial.print(", ");
        Serial.println(results[i]);
    }
}