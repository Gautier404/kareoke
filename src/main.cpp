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


// Define Pin numbers
#define g 11
#define o 10
#define r 9
int speaker = 15;


// Notes that will by tested A, F#, C, D, G
int notes[5] = {220, 185, 262, 147, 520};

//My functions
int testTone(int note, int speaker);
void showResults(int note, float testNote, int red, int orange, int green);
void administerTest(int speaker);

void setup() {
  AudioMemory(30); //Audio library requires I allocate it some memory from the teensy
  sgtl5000_1.enable(); 
  sgtl5000_1.volume(0.5);

  while (!Serial) ; // wait for Arduino Serial Monitor to be ready

  Serial.println("FFT test");

  notefreq.begin(.05); //Tell note frequency function only to spit out notes it is 95% sure are right

  //Tell teensy that it can output to the LED's
  pinMode(o, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(r, OUTPUT);
  
  administerTest(speaker);
}

void loop() {
    
}

//This function tests all the notes and prints what the user sang back to the serial moniter
void administerTest(int speaker){
    int results[5];
    for (int i = 0; i<5; i++){
        results[i] = testTone(notes[i], speaker); //record the results of the test to an array
        showResults(notes[i], results[i], r, o, g); //display results in the form of a green red or yellow light
    }

    //print out what note was played and then what note was sang for all the tests.
    for (int i = 0; i<5; i++){
        Serial.print(notes[i]);
        Serial.print(", ");
        Serial.println(results[i]);
    }
}

//TestTone plays a note and then returns the note the user the sang back
int testTone(int note, int speaker){
    //play the desired tone for a few seconds
    Serial.println("Playing Desired Tone");
    Serial.println("____________________");
    tone(speaker, note);       // Begin the tone

    //Simple loading bar as well as a 2 seconds delay for the note to play
    for(int i = 0; i<20; i++){
        Serial.print("*");
        delay(100);
    }
    noTone(speaker);
    Serial.println("");
    
    Serial.println("Sing back the note you heard");
    
    //small delay and loading bar for the user to get ready
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

            //This block of code ensures only notes that are somewhat close are recorded
            //so messy harmonics do mess up the data
            if (diff < error){
                total += recordedNote;
                dataPoints++;
                digitalWrite(g, HIGH); //if the user is doing a good job light up the green light
            } else {
                digitalWrite(g, LOW);
            }
        }
    }
    
    //record number of data points
    digitalWrite(g, LOW);
    //after 5 seconds divide the tone total by the number of data points and return it
    return total/dataPoints;
}

//This function turns the green led if the user did well, yellow if they were close, or red if they were completely off
void showResults(int note, float testNote, int red, int orange, int green) {

    int diff = abs(note-testNote);
    int dellay = 200;

    unsigned long time = millis();

    //light blinks on and off for 2 seconds
    while(millis()-time <2000){
        //if the user missed turn on red
        if(testNote == 0) {
            digitalWrite(red, HIGH);
            delay(dellay);
            digitalWrite(red, LOW);
            delay(dellay);
        } 
        //if the user was close but not within 1/100 of the desired frequency turn on yellow
        else if (diff > note/100){
            digitalWrite(orange, HIGH);
            delay(dellay);
            digitalWrite(orange, LOW);
            delay(dellay);
        }
        //if the user was within 1/100 of the desired frequency turn on green
        else {
            digitalWrite(green, HIGH);
            delay(dellay);
            digitalWrite(green, LOW);
            delay(dellay);
        }
    }

    delay(1000);
}