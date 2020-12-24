#ifndef HARDWARE_H
#define HARDWARE_H

// The code below will save you from having to change the pin numbers when you
// change which boards to which you will be uploading.
//
// Set pins, depending on if it's an ATtiny85 or a Pro Mini (larger Arduino...)
#ifdef __AVR_ATtiny85__
// Zonker's board connects pin 0 to the NeoPixel data wire.
// NB this pin is labelled as pin 5 on the chip:
// https://learn.sparkfun.com/tutorials/tiny-avr-programmer-hookup-guide/attiny85-use-hints
#define PIN_NEOPIXEL 0
// Pin 2, or INT0, is the only pin on an ATtiny85 with an interrupt.
// NB this pin is labelled as pin 7 on the chip:
// https://learn.sparkfun.com/tutorials/tiny-avr-programmer-hookup-guide/attiny85-use-hints
#define PIN_SWITCH   2 //INT0
// Let's use pin A2 for the microphone; A1 is the same pin as digital 2, which
// we're already using for the switch.
// NB this pin is labelled as pin 4 on the chip:
// https://learn.sparkfun.com/tutorials/tiny-avr-programmer-hookup-guide/attiny85-use-hints
#define PIN_MICROPHONE A2
#else
// Let's use pin 2 for the switch, labelled as D2 on the board.
// We can use digitalPinToInterrupt() to get the interrupt.
#define PIN_SWITCH   2
// Let's use pin D8 for the data wire to the NeoPixels.
#define PIN_NEOPIXEL 8
// Let's use pin A0 for the microphone.
#define PIN_MICROPHONE A0
#endif

// NB tinyNeoPixel does not support fill(), gamma32(), and ColorHSV().
#ifdef __AVR_ATtiny85__
#include <tinyNeoPixel.h>
#define NeoPixel tinyNeoPixel
#else
#include <Adafruit_NeoPixel.h>
#define NeoPixel Adafruit_NeoPixel
#endif

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
NeoPixel strip = NeoPixel(NUM_LEDS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

#endif