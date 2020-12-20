#ifndef LIGHT_SEQUENCE_H
#define LIGHT_SEQUENCE_H

#include "configuration.h"

enum LightSequence {
  #ifdef RAINBOW_PATTERNS
  RAINBOW_CYCLE,
  RAINBOW,
  #endif
  #ifdef WIPE_PATTERNS
  WIPE_COLOR,
  BLACK_LEAD_WIPE_COLOR,
  WHITE_LEAD_WIPE_COLOR,
  #endif
  #ifdef RANDOM_PATTERNS
  WIPE_RANDOM,
  RANDOM_PIXELS,
  #endif
  CYCLE_PAIRS,
  NUM_SEQUENCES  // A dummy sequence, for use in arithmetic.
};

#endif