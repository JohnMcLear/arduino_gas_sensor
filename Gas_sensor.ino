#include "pitches.h"
#include "Bounce.h"

int gasSensor = 0; // select input pin for gasSensor
int val = 0; // variable to store the value coming from the sensor
int greenLed = 3; // teh LED port
int warningLed = 7; // the LED port
int redLed = 4; // teh LED port
int speaker = 8; // Speaker
int extractorFan = 13; // Extractor fan Solid State port - Used for switching on/off an extractor fan
int lastButtonState = LOW;   // the previous reading from the input pin
int buttonState;             // the current reading from the input pin
int playTone = false; // Set to true to play a tone


// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4,4,4,4,4 
};

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 500;    // the debounce time; increase if the output flickers

void setup() {
  Serial.begin(9600);
}

void loop() {
  int val = analogRead(gasSensor); // read the value from the pot
  Serial.println( val );
  delay(500);

  if (val != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    if(val > 35 && val < 99){ // Turn on a warning LED
      digitalWrite(greenLed, HIGH);    // turn the LED off by making the voltage LOW
      digitalWrite(redLed, LOW);
      digitalWrite(warningLed, HIGH);
      digitalWrite(extractorFan, HIGH); // Turn on the extractor fan
    }
    else if(val > 100){
      digitalWrite(greenLed, LOW);    // turn the LED off by making the voltage LOW
      digitalWrite(redLed, HIGH);
      digitalWrite(warningLed, HIGH);
      digitalWrite(extractorFan, HIGH); // Turn on the extractor fan

      if(playTone == true){  // If the playTone Setting is set)
        // iterate over the notes of the melody:
        for (int thisNote = 0; thisNote < 8; thisNote++) {
  
          // to calculate the note duration, take one second 
          // divided by the note type.
          //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
          int noteDuration = 1000/noteDurations[thisNote];
          tone(speaker, melody[thisNote],noteDuration);
  
          // to distinguish the notes, set a minimum time between them.
          // the note's duration + 30% seems to work well:
          int pauseBetweenNotes = noteDuration * 1.30;
          delay(pauseBetweenNotes);
          // stop the tone playing:
          noTone(speaker);
        }
      }
    }
    else{ // everyting okay.
      digitalWrite(greenLed, HIGH); 
      digitalWrite(redLed, LOW); 
      digitalWrite(warningLed, LOW);
      digitalWrite(extractorFan, LOW); // Turn off the extractor fan
    }
    buttonState = val;
  }else{
     Serial.println("Not changing due to debounce");
  }
  lastButtonState = val;
}









