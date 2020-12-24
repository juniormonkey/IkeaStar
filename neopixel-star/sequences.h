#ifndef SEQUENCES_H
#define SEQUENCES_H

#include "color_utils.h"
#include "configuration.h"
#include "globals.h"
#include "hardware.h"

#define MIN_VOLUME 200.0
#define MAX_VOLUME 320.0
#define MAX_HUE 65535.0
#define MAX_COLOR 255.0

// Adjusts the depth of a given color dimension by the given volume.
uint32_t Normalize(uint32_t color, uint16_t volume) {
  #ifdef MICROPHONE
  float hsv[3];
  rgb2hsv(((float) Red(color)) / MAX_COLOR,
          ((float) Green(color)) / MAX_COLOR,
          ((float) Blue(color)) / MAX_COLOR,
          hsv);
  uint8_t volumeAdjustment = (
      volume > MAX_VOLUME 
      ? MAX_COLOR
      : (volume - MIN_VOLUME) / ((MAX_VOLUME - MIN_VOLUME) / MAX_COLOR));
  uint8_t saturation = (hsv[1] * MAX_COLOR / 2) + (hsv[1] * volumeAdjustment / 2);
  uint8_t value = (hsv[2] * MAX_COLOR / 3) + (2 * hsv[2] * volumeAdjustment / 3);
  
  #ifdef __AVR_ATtiny85__
  float rgb[3];
  hsv2rgb(hsv[0], (float)saturation / MAX_COLOR, (float)value / MAX_COLOR, rgb);
  return strip.Color((uint8_t)(rgb[0] * MAX_COLOR),
                     (uint8_t)(rgb[1] * MAX_COLOR),
                     (uint8_t)(rgb[2] * MAX_COLOR));
  #else
  return strip.gamma32(
             strip.ColorHSV((uint16_t)(hsv[0] * MAX_HUE),
                            (uint8_t)(hsv[1] * MAX_COLOR),
                            value));
  #endif

  #else
  return color;
  #endif
}

#ifdef RAINBOW_PATTERNS
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//   ** Original Adafruit example code
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void blackNow() {
  #ifndef __AVR_ATtiny85__
  strip.fill(0);
  #endif
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    colors[i] = 0;
  }
  strip.show();
}

#ifdef RAINBOW_PATTERNS
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//   ** Original Adafruit example code
void rainbow(uint16_t color) {
  for (uint16_t pixel = 0; pixel < NUM_LEDS; ++pixel) {
    colors[pixel] = Wheel((pixel + color) & 255);
  }
  pause(50);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//   ** Original Adafruit example code
void rainbowCycle(uint16_t color) {
  // This sets each pixel to a different color of the rainbow, start to end,
  // then cycles all the pixels through their respective rainbows, twice.
  // add some delay to slow down the cycle of the colors.
  for (uint16_t pixel = 0; pixel < NUM_LEDS; ++pixel) {
    colors[pixel] = Wheel(((pixel * 256 / NUM_LEDS) + color) & 255);
  }
  pause(50);
}
#endif

#ifdef RANDOM_PATTERNS
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void randomPixels() {
  uint8_t i = Entropy.random(0, NUM_LEDS);
  uint8_t myBase = randomColor();
  // SERIAL_PRINTLN(myBase);
  colors[i] = strip.Color(
      myColors[myBase], 
      myColors[myBase + 1], 
      myColors[myBase + 2]);
   pause(500);
}
#endif

#ifdef WIPE_PATTERNS
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void wipeColor(uint16_t nextStep, uint8_t baseColor) {
  colors[nextStep] = strip.Color(
      myColors[baseColor], 
      myColors[baseColor + 1], 
      myColors[baseColor + 2]);
  pause(INTER_PIXEL_DELAY);
  if (nextStep >= cycleLength - 1) {
    pause(35);
  }
}
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void cyclePairs(uint16_t nextStep) {
  static uint8_t c, d;
  if (nextStep == 0) {
    c = randomColor();  // pick the color for the top/left
    d = randomColor();  // pick the color for the lower/right
    while ( c == d )  {
      d = randomColor();  //if colors match, pick again!
    }
  }

  // set pixel to the first color
  uint8_t pixel1 = 2 * nextStep;
  colors[pixel1] = strip.Color(
      myColors[c],
      myColors[c + 1],
      myColors[c + 2]);

  // set next pixel to 2nd color
  uint8_t pixel2 = pixel1 + 1;
  colors[pixel2] = strip.Color(
      myColors[d],
      myColors[d + 1],
      myColors[d + 2]);
  
  if (nextStep == cycleLength - 1) {
    pause(2000 * (Entropy.random(1, 6)) );  // Add a pause to enjoy the effect
  }
}

#ifdef WIPE_PATTERNS
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void wipeColorWithLead(uint16_t nextStep,
                       uint32_t leadColor,
                       uint8_t baseColor) {
  /* This sequence 'wipes' all the pixels, one at a time, with a color,
      but it leads the wipe with a given color. */
  if (nextStep > 0) {
    colors[nextStep - 1] = strip.Color(
        myColors[baseColor],
        myColors[baseColor + 1],
        myColors[baseColor + 2]);
  }
  if (nextStep < NUM_LEDS) {
    colors[nextStep] = leadColor;
  }
  pause(INTER_PIXEL_DELAY);
  if (nextStep >= cycleLength - 1) {
    uint16_t interWipeDelay = (500 * random(2, 8));
    pause(interWipeDelay);
  }
}
#endif

#ifdef RANDOM_PATTERNS
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void wipeRandom(uint16_t nextStep) {
  uint8_t color = randomColor();
  colors[nextStep] = strip.Color(
      myColors[color],
      myColors[color + 1],
      myColors[color + 2]);
  pause(INTER_PIXEL_DELAY);
  if (nextStep >= cycleLength - 1) {
    pause(INTER_PIXEL_DELAY);
  }
}
#endif

#endif
