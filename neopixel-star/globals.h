#ifndef globals_h
#define globals_h

// Some global variables that are cached between loops, and reset on a button press.
uint8_t baseColor;
uint16_t nextStep, speed, cycles, cycleLength, randomDelay;
long pauseEnd;

// These are #define instead of function definitions, so that the if condition is
// inlined wherever they're used, and the inner function is only evaluated when
// the condition is true.
#define setSpeed(S) if (speed <= 0) { speed = (S); }
#define setCycles(C) if (cycles <= 0) { cycles = (C); }
#define setCycleLength(L) if (cycleLength <= 0) { cycleLength = (L); }
#define setBaseColor(C) if (baseColor <= 0) { baseColor = (C); }
#define setRandomDelay(L) if (randomDelay <= 0) { randomDelay = (L); }

// Use this instead of delay(), as a non-blocking alternative.
void pause(int length) { 
  long now = millis();
  pauseEnd = (now > pauseEnd ? now : pauseEnd) + length;
}

void reset() {
  nextStep = 0;
  speed = 0;
  cycles = 0;
  cycleLength = 0;
  baseColor = 0;
  randomDelay = 0;
  pauseEnd = 0;
}

#endif
