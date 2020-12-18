// The code below will save you from having to change the pin numbers when you
// change which boards to which you will be uploading.
//
// Set pins, depending on if it's an ATtiny85 or a Pro Mini (larger Arduino...)
#ifdef __AVR_ATtiny85__
// Zonker's board connects pin 0 to the NeoPixel data wire.
// NB this pin is labelled as pin 5 on the chip:
// https://learn.sparkfun.com/tutorials/tiny-avr-programmer-hookup-guide/attiny85-use-hints
#define PIN_NEOPIXEL 0
// Pin 2 is the only pin on an ATtiny85 with an interrupt.
// NB this pin is labelled as pin 7 on the chip:
// https://learn.sparkfun.com/tutorials/tiny-avr-programmer-hookup-guide/attiny85-use-hints
#define PIN_SWITCH   2
// TODO: analog pin?
#define PIN_MICROPHONE A1
// The ATtiny85 library doesn't define digitalPinToInterrupt(), so we do that here:
#define digitalPinToInterrupt(P) ( (P) == 2 ? 0 : -1)
#else
// Let's use pin 2 for the switch, labelled as D2 on the board.
// We can use digitalPinToInterrupt() to get the interrupt.
#define PIN_SWITCH   2
// Let's use pin D8 for the data wire to the NeoPixels.
#define PIN_NEOPIXEL 8
// Let's use pin A0 for the microphone.
#define PIN_MICROPHONE A0
#endif

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

#ifndef configuration_h
#define configuration_h

// Include the Adafruit_NeoPixel library  https://github.com/adafruit/Adafruit_NeoPixel
// (and check out the Adafruit Arduino Metro Mini! https://www.adafruit.com/product/2590 )
#include <Adafruit_NeoPixel.h>

enum LightSequence {
//  BLACK_NOW,
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
#define NUM_LEDS 14

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
// NEO_RGB Pixels are wired for RGB bitstream
// NEO_GRB Pixels are wired for GRB bitstream
// NEO_KHZ400 400 KHz bitstream (e.g. FLORA pixels)
// NEO_KHZ800 800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

// delay between different sequences
#define INTER_SEQUENCE_DELAY 1500

// inter-pixel delay, so you can see the wipe & fade patterns move quickly
#define INTER_PIXEL_DELAY 60

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

// DEFINE WHAT MODE TO OPERATE HERE...

// Uncomment this to advance sequences automatically; otherwise, wait for button push
// #define ADVANCE_AUTOMATICALLY

// Uncomment this to choose sequences at random; otherwise, choose them in order
// #define RANDOM_SEQUENCE

#endif
