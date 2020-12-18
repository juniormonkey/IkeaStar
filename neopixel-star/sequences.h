#ifndef sequences_h
#define sequences_h

#include "configuration.h"
#include "globals.h"
#include "color_utils.h"

#define MAX_VOLUME 320.0
#define MAX_HUE 65535.0
#define MAX_COLOR 255.0

// Adjusts the depth of a given color dimension by the given volume.
uint32_t Normalize(uint8_t red, uint8_t green, uint8_t blue, uint16_t volume) {
  float hsv[3];
  rgb2hsv(((float) red) / MAX_COLOR,
          ((float) green) / MAX_COLOR,
          ((float) blue) / MAX_COLOR,
          hsv);
  uint8_t maxValue = (
      volume > MAX_VOLUME 
      ? MAX_COLOR
      : volume / (MAX_VOLUME / MAX_COLOR));
  return strip.gamma32(
             strip.ColorHSV((uint16_t)(hsv[0] * MAX_HUE),
                            (uint8_t)(hsv[1] * MAX_COLOR),
                            (uint8_t)(hsv[2] * maxValue)));
}

uint32_t Normalize(uint32_t color, uint16_t volume) {
  return Normalize(Red(color), Green(color), Blue(color), volume);
}

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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void blackNow() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, 0);
  }
  strip.show();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//   ** Original Adafruit example code
void rainbow(uint16_t color, uint16_t wait, uint16_t volume) {
  for (uint16_t pixel = 0; pixel < NUM_LEDS; ++pixel) {
    strip.setPixelColor(pixel, Normalize(Wheel((pixel + color) & 255), volume));
  }
  strip.show();
  pause(wait);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//   ** Original Adafruit example code
void rainbowCycle(uint16_t color, uint8_t speed, uint16_t volume) {
  // This sets each pixel to a different color of the rainbow, start to end,
  // then cycles all the pixels through their respective rainbows, twice.
  // add some delay to slow down the cycle of the colors.
  for (uint16_t pixel = 0; pixel < NUM_LEDS; ++pixel) {
    strip.setPixelColor(pixel, Normalize(Wheel(((pixel * 256 / NUM_LEDS) + color) & 255), volume));
  }
  strip.show();
  pause(speed);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void randomPixels(uint16_t speed, uint16_t volume) {
  uint8_t i = Entropy.random(0, NUM_LEDS);
  uint8_t myBase = randomColor();
  // SERIAL_PRINTLN(myBase);
  uint8_t red = myColors[myBase];
  uint8_t green = myColors[myBase + 1];
  uint8_t blue = myColors[myBase + 2];
  strip.setPixelColor(i, Normalize(red, green, blue, volume));
  strip.show();
  pause(speed);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void wipeColor(uint16_t nextStep, uint8_t color, uint16_t wait, uint16_t volume) {
  uint8_t red = myColors[color];
  uint8_t green = myColors[color + 1];
  uint8_t blue = myColors[color + 2];
  strip.setPixelColor(nextStep, Normalize(red, green, blue, volume));
  strip.show();
  pause(INTER_PIXEL_DELAY);
  if (nextStep >= cycleLength - 1) {
    pause(wait);
  }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void cyclePairs(uint16_t nextStep, uint16_t wait, uint16_t volume) {
  static uint8_t c, d;
  if (nextStep == 0) {
    c = randomColor();  // pick the color for the top/left
    d = randomColor();  // pick the color for the lower/right
    while ( c == d )  {
      d = randomColor();  //if colors match, pick again!
    }
  }

  uint8_t cr = myColors[c];
  uint8_t cg = myColors[c + 1];
  uint8_t cb = myColors[c + 2];
  uint8_t ar = myColors[d];
  uint8_t ag = myColors[d + 1];
  uint8_t ab = myColors[d + 2];
  strip.setPixelColor(2 * nextStep, Normalize(cr, cg, cb, volume)); //set pixel to the first color
  strip.setPixelColor(2 * nextStep + 1, Normalize(ar, ag, ab, volume)); //set next pixel to 2nd color

  strip.show();

  if (nextStep == cycleLength - 1) {
    pause(wait);  //  Add a pause to enjoy the effect
  }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void blackLeadWipeColor(uint16_t nextStep, uint8_t color, uint16_t volume) {
  /* This sequence 'wipes' all the pixels, one at a time, with a color,
      but it leads the wipe with a dark, unlit pixel (like an eraser
      that changes the stripe color). */
  if (nextStep > 0) {
    uint8_t red = myColors[color];
    uint8_t green = myColors[color + 1];
    uint8_t blue = myColors[color + 2];
    strip.setPixelColor(nextStep - 1, Normalize(red, green, blue, volume));
  }
  if (nextStep < NUM_LEDS) {
    strip.setPixelColor(nextStep, 0);
  }
  strip.show();
  pause(INTER_PIXEL_DELAY);
  if (nextStep >= cycleLength - 1) {
    uint16_t interWipeDelay = (500 * random(2, 8));
    pause(interWipeDelay);
  }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void whiteLeadWipeColor(uint16_t nextStep, uint8_t color, uint16_t volume) {
  /* This sequence 'wipes' all the pixels, one at a time, with a color,
      but it leads the wipe with a bright, white color (like a meteor
     passing, with a colored tail). */
     // The brightness of the pixel depends on 'volume'.
  if (nextStep > 0) {
    uint8_t red = myColors[color];
    uint8_t green = myColors[color + 1];
    uint8_t blue = myColors[color + 2];
    strip.setPixelColor(nextStep - 1, Normalize(red, green, blue, volume));
  }
  if (nextStep < NUM_LEDS) {
    uint8_t white = 235 * volume / 1024;
    strip.setPixelColor(nextStep, white, white, white);
  }
  strip.show();
  pause(INTER_PIXEL_DELAY);
  if (nextStep >= cycleLength - 1) {
    uint16_t interWipeDelay = (500 * random(2, 8));
    pause(interWipeDelay);
  }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void wipeRandom(uint16_t nextStep, uint16_t wait, uint16_t volume) {
  uint8_t color = randomColor();
  uint8_t red = myColors[color];
  uint8_t green = myColors[color + 1];
  uint8_t blue = myColors[color + 2];
  strip.setPixelColor(nextStep, Normalize(red, green, blue, volume));
  strip.show();
  pause(INTER_PIXEL_DELAY);
  if (nextStep >= cycleLength - 1) {
    pause(wait);
  }
}

#endif
