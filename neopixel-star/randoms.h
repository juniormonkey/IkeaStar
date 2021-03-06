#ifndef randoms_h
#define randoms_h

// https://github.com/pmjdebruijn/Arduino-Entropy-Library
#include <Entropy.h>

#include "light_sequence.h"

// what was the last sequence chosen at random? (to avoid repeats)
LightSequence previousSequence = BLACK_LEAD_WIPE_COLOR;

LightSequence randomSequence() {
  SERIAL_PRINTLN("Selecting random sequence: ")
  uint8_t i = 0;
  LightSequence newSequence = previousSequence;
  do {
    newSequence = static_cast<LightSequence>(Entropy.random(1, NUM_SEQUENCES));
    SERIAL_PRINT("random sequence =");
    SERIAL_PRINTLN(newSequence);
    ++i;
  } while (i < 5 && newSequence == previousSequence);
  previousSequence = newSequence;
  return newSequence;
}

// what was the last color chosen at random? (to avoid repeats)
uint8_t previousColor = 5;

uint8_t randomColor() {
  uint8_t i = 0, result = previousColor;
  do {
    result = static_cast<uint8_t>(Entropy.random(0, colorSets) * 3);
    SERIAL_PRINT("randomColor =");
    SERIAL_PRINTLN(result);
    ++i;
  } while (i < 4 && result == previousColor);
  previousColor = result;
  return result;
}

#endif
