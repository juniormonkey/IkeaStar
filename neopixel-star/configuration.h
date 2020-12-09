// Include the Adafruit_NeoPixel library  https://github.com/adafruit/Adafruit_NeoPixel
// (and check out the Adafruit Arduino Metro Mini! https://www.adafruit.com/product/2590 )
#include <Adafruit_NeoPixel.h>

// The code below will save you from having to change the pin numbers when you
//  change which boards to which you will be uploading.
//
// Set pins, depending on if it's an ATtiny85 or a Pro Mini (larger Arduino...)
#ifdef __AVR_ATtiny85__
#define PIN_NEOPIXEL 0
#define PIN_RANDOMSEED 1
#define PIN_SWITCH   2   // on ATtiny85's, Int 0 = PB2 (chip pin 7)
#else
#define PIN_RANDOMSEED A1
#define PIN_SWITCH   2   // on UNO's, Int 0 = pin D2, and Int 1 = pin D3
#define PIN_NEOPIXEL 8
#endif

//#define DEBUG

#ifdef DEBUG
#define SERIAL_PRINT(X) Serial.print(X);
#define SERIAL_PRINTLN(X) Serial.println(X);
#else
#define SERIAL_PRINT(X)
#define SERIAL_PRINTLN(X)
#endif

enum SequenceAdvanceMode {
  AUTOMATIC,
  ON_BUTTON_PUSH
};

enum SequenceSelectionMode {
  SEQUENTIAL,
  RANDOM
};

enum LightSequence {
  BLACK_NOW,
  RAINBOW_CYCLE,
  RAINBOW,
  WIPE_COLOR,
  BLACK_LEAD_WIPE_COLOR,
  WHITE_LEAD_WIPE_COLOR,
  WIPE_RANDOM,
  RANDOM_PIXELS,
  CYCLE_PAIRS,
  NUM_SEQUENCES  // A dummy sequence, for use in arithmetic.
};

// How many neopixels in the string?
// My IKEA Star project uses 14 neopixels...
// do you use more or less in your project?
const uint8_t numLeds = 14;

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
// NEO_RGB Pixels are wired for RGB bitstream
// NEO_GRB Pixels are wired for GRB bitstream
// NEO_KHZ400 400 KHz bitstream (e.g. FLORA pixels)
// NEO_KHZ800 800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLeds, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

// delay between different sequences
const uint16_t interSequenceDelay = 1500;

// inter-pixel delay, so you can see the wipe & fade patterns move quickly
const uint8_t interPixelDelay = 60;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

// DEFINE WHAT MODE TO OPERATE HERE...
const SequenceAdvanceMode advanceMode = ON_BUTTON_PUSH;
const SequenceSelectionMode selectionMode = SEQUENTIAL;
LightSequence sequence = BLACK_NOW;