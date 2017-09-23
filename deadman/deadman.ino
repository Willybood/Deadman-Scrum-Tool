/*
 * When the button is pushed a timer counts down.
 * When it reaches a set amount, it starts buzzing until the button is depressed.
 * Piezo buzzer code is taken from https://www.arduino.cc/en/Tutorial/ToneMelody?from=Tutorial.Tone
 */

#include "Timer.h"
#include "pitches.h"

const int BUZZER_PIN = 7; // Output pin for the buzzer
const int BUTTON_PIN = 8; // Input pin for the button to be held down
const int BUTTON_V_PIN = 9; // Input pin for the button, provides the voltage needed
const int RED_LED_PIN = 4; // Output pins for the LEDs
const int GREEN_LED_PIN = 2; // Output pins for the LEDs

const int SECONDS_TO_WAIT = 90UL; // The amount of time to wait before setting off the alarm
const int TIME_BETWEEN_BUZZES = 3000; // The time between tones played

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};
const int NUM_OF_NOTES = 8;

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

// This buffer is used to replace the fact that I don't have a pull-down resistor
const int inputBufferLength = 100;
int inputBuffer[inputBufferLength];

// Events used by the timer library
int buzzerEvent; // The event that controls the buzzing
int timerEvent; // The event that counts down the timer

Timer timer; // Third party object type that controls the timing

bool countingDown = false;

// Function run once at startup
void setup() {
  Serial.println(("Entering %s", __FUNCTION__));
  Serial.begin(9600);
  
  pinMode(BUTTON_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BUTTON_V_PIN, OUTPUT);

  // Set the Red LED on, and the others off by default
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(GREEN_LED_PIN, LOW);

  digitalWrite(BUTTON_V_PIN, HIGH);
  
  Serial.println("Program started");
}

// Plays the tone
void playTone() {
  Serial.println(("Entering %s", __FUNCTION__));
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZER_PIN, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    const int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(BUZZER_PIN);
  }
}

// Starts the buzzing
void startBuzzing() {
  Serial.println(("Entering %s", __FUNCTION__));
  buzzerEvent = timer.every(TIME_BETWEEN_BUZZES, playTone);
}

// Stops the buzzing
void stopBuzzing() {
  Serial.println(("Entering %s", __FUNCTION__));
  timer.stop(buzzerEvent);
}

// Starts the countdown
void startCountdown() {
  Serial.println(("Entering %s", __FUNCTION__));
  const unsigned long timeToWait = SECONDS_TO_WAIT * 1000UL;
  timerEvent = timer.after(timeToWait, startBuzzing);
}

// Stops the countdown
void stopCountdown() {
  Serial.println(("Entering %s", __FUNCTION__));
  timer.stop(timerEvent);
}

// Updates the buffer and returns the current state
int updateBuffer(const int buttonInput) {
  // Move the buffer along by 1
  for(int i = (inputBufferLength - 2); i >= 0; --i) {
    inputBuffer[i + 1] = inputBuffer[i];
  }
  // Add the new input to the buffer
  inputBuffer[0] = buttonInput;
  // return the result from the buffer
  for(int i = 0; i < inputBufferLength; ++i) {
    if(inputBuffer[i] == LOW) {
      return LOW;
    }
  }
  return HIGH;
}

// Function run repeatedly
void loop() {
  timer.update();
  
  // read the state of the pushbutton value:
  int buttonState = updateBuffer(digitalRead(BUTTON_PIN));

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    if(false == countingDown) {
      startCountdown();
      
      // Switch to the Green LED
      digitalWrite(GREEN_LED_PIN, HIGH);
      digitalWrite(RED_LED_PIN, LOW);
    }
    countingDown = true;
  } else {
    if(true == countingDown) {
      stopBuzzing();
      stopCountdown();
      
      // Switch back to the Red LED
      digitalWrite(RED_LED_PIN, HIGH);
      digitalWrite(GREEN_LED_PIN, LOW);
    }
    countingDown = false;
  }
}

