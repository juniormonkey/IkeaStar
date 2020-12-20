#ifndef GLOBALS_H
#define GLOBALS_H

#include "hardware.h"
#include "light_sequence.h"

uint32_t colors[NUM_LEDS];

LightSequence sequence;

// Some global variables that are cached between loops, and reset on a button press.
uint16_t nextStep, cycles, cycleLength;
unsigned long pauseEnd;

// These are #define instead of function definitions, so that the if condition is
// inlined wherever they're used, and the inner function is only evaluated when
// the condition is true.
#define setCycles(C) if (cycles <= 0) { cycles = (C); }
#define setCycleLength(L) if (cycleLength <= 0) { cycleLength = (L); }

// Use this instead of delay(), as a non-blocking alternative.
void pause(uint16_t length) { 
  unsigned long now = millis();
  pauseEnd = (now > pauseEnd ? now : pauseEnd) + length;
}

void reset() {
  nextStep = 0;
  cycles = 0;
  cycleLength = 0;
  pauseEnd = 0;
}

#endif
