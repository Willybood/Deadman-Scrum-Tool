/*
 * When the button is pushed a timer counts down.
 * When it reaches a set amount, it starts buzzing until the button is depressed.
 * Piezo buzzer code is taken from https://www.arduino.cc/en/Tutorial/ToneMelody?from=Tutorial.Tone
 */

#include "Timer.h"
#include "ttone.h"

// Defines for the serial port, disables it for all non-prototype hardware
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__)
#define serialPrint(x) Serial.println(x)
#define serialBegin(x) Serial.begin(x)
#else
#define serialPrint(x)
#define serialBegin(x)
#endif

const int BUZZER_PIN = 1; // Output pin for the buzzer, required to be pin 1 for the ttone library
const int BUTTON_PIN = 0; // Input pin for the button to be held down
const int BUTTON_V_PIN = 2; // Input pin for the button, provides the voltage needed
const int RED_LED_PIN = 3; // Output pins for the LEDs
const int GREEN_LED_PIN = 4; // Output pins for the LEDs

const int SECONDS_TO_WAIT = 90UL; // The amount of time to wait before setting off the alarm
const int TIME_BETWEEN_BUZZES = 3000; // The time between tones played

const int NUM_OF_OSC_SEGMENTS = 9; // The number of segments of oscillation of the LED to make as the user gets closer to their limit
int currentOscSegment = 0; // The current oscillating segment

// notes in the melody:
int melody[] = {
  C4, G3, G3, A3, G3, 0, B3, C4
};
const int NUM_OF_NOTES = 8;

unsigned long millisWhenPushed = 0; // The value of millis (milliseconds since boot) when the button is pushed

const long LONG_OSC_PERIOD = 1000L; // longest time in ms to flash the LED. Other variants are fractions of this number

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

// Events used by the timer library
// By default are set to 'MAX_NUMBER_OF_EVENTS' so they cannot be set or unset accidentally
int buzzerEvent = MAX_NUMBER_OF_EVENTS; // The event that controls the buzzing
int timerEvent = MAX_NUMBER_OF_EVENTS; // The event that counts down the timer
int ledOscillations = MAX_NUMBER_OF_EVENTS; // The event that controls the LED oscillations

Timer timer; // Third party object type that controls the timing

bool countingDown = false;

// Function run once at startup
void setup() {
  serialPrint(("Entering %s", __FUNCTION__));
  serialBegin(9600);
  
  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUTTON_V_PIN, OUTPUT);
  digitalWrite(BUTTON_V_PIN, LOW);

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);
  
  serialPrint("Program started");
}

// Plays the tone
void playTone() {
  serialPrint(("Entering %s", __FUNCTION__));
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < NUM_OF_NOTES; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    trinketTone(melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    const int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
  }
}

// Starts the buzzing
void startBuzzing() {
  serialPrint(("Entering %s", __FUNCTION__));
  timer.stop(ledOscillations);
  digitalWrite(RED_LED_PIN, HIGH);
  buzzerEvent = timer.every(TIME_BETWEEN_BUZZES, playTone);
}

// Stops the buzzing
void stopBuzzing() {
  serialPrint(("Entering %s", __FUNCTION__));
  timer.stop(buzzerEvent);
}

// Starts the countdown
void startCountdown() {
  serialPrint(("Entering %s", __FUNCTION__));
  const unsigned long timeToWait = SECONDS_TO_WAIT * 1000UL;
  timerEvent = timer.after(timeToWait, startBuzzing);
}

// Stops the countdown
void stopCountdown() {
  serialPrint(("Entering %s", __FUNCTION__));
  timer.stop(timerEvent);
}

// Sets the pulsing of the LEDs.
// Gets faster as the user reaches their talking limit.
void updatePulsingLEDs() {
  for(int i = 1; i < NUM_OF_OSC_SEGMENTS; ++i) {
    const unsigned long millisSpent = millis() - millisWhenPushed; // The number of milliseconds ela[sed since the button was pushed.
    const unsigned long proportionOfWaitTime = ((unsigned long)SECONDS_TO_WAIT * 1000UL) / (unsigned long)NUM_OF_OSC_SEGMENTS;
    if((millisSpent < (proportionOfWaitTime * i)) && (millisSpent >= (proportionOfWaitTime * (i - 1)))) {
      if(currentOscSegment != i) {
        timer.stop(ledOscillations);
        ledOscillations = timer.oscillate(RED_LED_PIN, (LONG_OSC_PERIOD / (long)NUM_OF_OSC_SEGMENTS) * (NUM_OF_OSC_SEGMENTS - i), digitalRead(RED_LED_PIN));
      }
      currentOscSegment = i;
    }
  }
}

// Function run repeatedly
void loop() {
  timer.update();
  
  // read the state of the pushbutton value:
  int buttonState = digitalRead(BUTTON_PIN);

  // check if the pushbutton is pressed.
  if (buttonState == LOW) {
    if(false == countingDown) {
      startCountdown();
      millisWhenPushed = millis();
      currentOscSegment = 0;
      digitalWrite(RED_LED_PIN, HIGH); // Turn the LED pin on when first pushed.
    }
    digitalWrite(GREEN_LED_PIN, LOW);
    updatePulsingLEDs();
    countingDown = true;
  } else {
    if(true == countingDown) {
      stopBuzzing();
      stopCountdown();
    }
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, HIGH);
    countingDown = false;
  }
}

