/*====================================================
             IR SENSOR UP/DOWN COUNTER
======================================================
Description:
  This program uses an IR sensor to detect objects and
  update the counter. Switches SW1 and SW2 determine
  whether the count increases or decreases and by how
  much. The current count is displayed on two
  seven-segment displays.

  00 = Count up by 1
  01 = Count down by 1
  10 = Count up by 2
  11 = Count down by 2
  
Programmer: 
  De Asis, Felicity M.

Date:
 Octorber 28, 2026

====================================================*/

#include "decoder.h"

// GPIOS
const uint8_t IR = 36;
const uint8_t SW1 = 2;
const uint8_t SW2 = 15;
const uint8_t RESET = 34;

// VARIABLES
int counter = 0;
int input = HIGH;
int previousInput = HIGH;

int SW1_state = 0;
int SW2_state = 0;
int change = 1;

int resetState = HIGH;
int previousResetState = HIGH;

void setup() {

  // IR SENSOR
  pinMode(IR, INPUT);

  // SWITCHES
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);

  // RESET BUTTON
  pinMode(RESET, INPUT_PULLUP);

  // SEVEN-SEGMENT DISPLAYS
  decoderSetup();

  Serial.begin(9600);

  // INITIAL DISPLAY
  decoder(counter);

  // READ INITIAL SENSOR STATE
  previousInput = digitalRead(IR);
}

void loop() {

  // READ RESET BUTTON
  resetState = digitalRead(RESET);

  // RESET WHEN BUTTON IS PRESSED
  if (previousResetState == HIGH && resetState == LOW) {
    counter = 0;
    decoder(counter);
    Serial.println("Counter Reset: 00");
  }

  previousResetState = resetState;

  // READ SWITCHES
  SW1_state = digitalRead(SW1);
  SW2_state = digitalRead(SW2);

  // SELECT COUNTING MODE
  if (SW1_state == 0 && SW2_state == 0) {
    change = 1;
  }
  else if (SW1_state == 0 && SW2_state == 1) {
    change = -1;
  }
  else if (SW1_state == 1 && SW2_state == 0) {
    change = 2;
  }
  else {
    change = -2;
  }

  // READ IR SENSOR
  input = digitalRead(IR);

  // COUNT ONLY ON A NEW DETECTION
  if (previousInput == HIGH && input == LOW) {

    counter = counter + change;

    // LIMIT COUNTER FROM 00 TO 99
    if (counter < 0) {
      counter = 0;
    }
    else if (counter > 99) {
      counter = 99;
    }

    // UPDATE DISPLAYS
    decoder(counter);

    // SERIAL MONITOR
    Serial.printf("Object Count: %02d\n", counter);
  }

  // SAVE CURRENT SENSOR STATE
  previousInput = input;

  delay(20);
}
