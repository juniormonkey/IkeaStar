#ifndef globals_h
#define globals_h

// Some global variables that are cached between loops, and reset on a button press.
uint8_t baseColor;
uint16_t nextStep, speed, cycles, cycleLength, pauseLength;

// These are #define instead of function definitions, so that the if condition is
// inlined wherever they're used, and the inner function is only evaluated when
// the condition is true.
#define setSpeed(S) if (speed <= 0) { speed = (S); }
#define setCycles(C) if (cycles <= 0) { cycles = (C); }
#define setCycleLength(L) if (cycleLength <= 0) { cycleLength = (L); }
#define setBaseColor(C) if (baseColor <= 0) { baseColor = (C); }
#define setPauseLength(L) if (pauseLength <= 0) { pauseLength = (L); }

void reset() {
  nextStep = 0;
  speed = 0;
  cycles = 0;
  cycleLength = 0;
  baseColor = 0;
  pauseLength = 0;
}

#endif
