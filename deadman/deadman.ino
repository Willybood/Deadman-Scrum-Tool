/*
 * When the button is pushed a timer counts down.
 * When it reaches a set amount, it starts buzzing until the button is depressed.
 */

#include "Timer.h"

const int BUZZER_PIN = 7; // Output pin for the buzzer/motor
const int BUTTON_PIN = 8; // Input pin for the button to be held down
const int RED_LED_PIN = 4; // Output pins for the LEDs
const int GREEN_LED_PIN = 2; // Output pins for the LEDs
const int BUTTON_V_PIN = 9; // Input pin for the button, provides the voltage needed

const int SECONDS_TO_WAIT = 90; // The amount of time to wait before
const int OSCILLATION_TIME = 1000; // The time between motor oscillations

// Events used by the timer library
int buzzerEvent; // The event that controls the buzzing
int timerEvent; // The event that counts down the timer

Timer timer; // Third party object type that controls the timing

bool countingDown = false;

// Function run once at startup
void setup() {
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

// Starts the buzzing
void startBuzzing() {
  Serial.println(("Entering %s", __FUNCTION__));
  buzzerEvent = timer.oscillate(BUZZER_PIN, OSCILLATION_TIME, HIGH);
}

// Stops the buzzing
void stopBuzzing() {
  Serial.println(("Entering %s", __FUNCTION__));
  timer.stop(buzzerEvent);
}

// Starts the countdown
void startCountdown() {
  Serial.println(("Entering %s", __FUNCTION__));
  timerEvent = timer.after(SECONDS_TO_WAIT * 1000, startBuzzing);
}

// Stops the countdown
void stopCountdown() {
  Serial.println(("Entering %s", __FUNCTION__));
  timer.stop(timerEvent);
}

// Function run repeatedly
void loop() {
  timer.update();
  
  // read the state of the pushbutton value:
  int buttonState = digitalRead(BUTTON_PIN);

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

