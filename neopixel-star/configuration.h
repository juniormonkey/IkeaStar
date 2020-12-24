#ifndef CONFIGURATION_H
#define CONFIGURATION_H

//#define DEBUG

#ifdef DEBUG
#define SERIAL_PRINT(X) Serial.print(X);
#define SERIAL_PRINTLN(X) Serial.println(X);
#define SERIAL_PRINTLNIF(X, Y) if(X) { Serial.println(Y); } 
#else
#define SERIAL_PRINT(X)
#define SERIAL_PRINTLN(X)
#define SERIAL_PRINTLNIF(X, Y)
#endif

// delay between different sequences
#define INTER_SEQUENCE_DELAY 1500

// inter-pixel delay, so you can see the wipe & fade patterns move quickly
#define INTER_PIXEL_DELAY 60

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

// DEFINE WHAT MODE TO OPERATE HERE...

// Uncomment this to advance sequences automatically; otherwise, wait for button push
#define ADVANCE_AUTOMATICALLY

// Uncomment this to choose sequences at random; otherwise, choose them in order
#define RANDOM_SEQUENCE

// Uncomment these to enable features. Not all features will fit on an ATTiny85.
//#define MICROPHONE
#define RAINBOW_PATTERNS
#define WIPE_PATTERNS
#define RANDOM_PATTERNS
#define CHRISTMAS_COLORS
#define DADS_COLORS

#endif
