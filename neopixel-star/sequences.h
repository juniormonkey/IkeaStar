#ifndef sequences_h
#define sequences_h

#include "configuration.h"
#include "globals.h"

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
  for (uint8_t i = 0; i < numLeds; i++) {
    strip.setPixelColor(i, 0);
  }
  strip.show();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//   ** Original Adafruit example code
void rainbow(uint16_t color, uint16_t wait) {
  for (uint16_t pixel = 0; pixel < numLeds; ++pixel) {
    strip.setPixelColor(pixel, Wheel((pixel + color) & 255));
  }
  strip.show();
  delay(wait);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//   ** Original Adafruit example code
void rainbowCycle(uint16_t color, uint8_t speed) {
  // This sets each pixel to a different color of the rainbow, start to end,
  // then cycles all the pixels through their respective rainbows, twice.
  // add some delay to slow down the cycle of the colors.
  for (uint16_t pixel = 0; pixel < numLeds; ++pixel) {
    strip.setPixelColor(pixel, Wheel(((pixel * 256 / numLeds) + color) & 255));
  }
  strip.show();
  delay(speed);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void randomPixels(uint16_t speed) {
  uint8_t i = Entropy.random(0, numLeds);
  uint8_t myBase = randomColor();
  // SERIAL_PRINTLN(myBase);
  uint8_t red = myColors[myBase];
  uint8_t green = myColors[myBase + 1];
  uint8_t blue = myColors[myBase + 2];
  strip.setPixelColor(i, red, green, blue);
  strip.show();
  delay(speed);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void wipeColor(uint16_t nextStep, uint8_t color, uint16_t wait) {
  uint8_t red = myColors[color];
  uint8_t green = myColors[color + 1];
  uint8_t blue = myColors[color + 2];
  strip.setPixelColor(nextStep, red, green, blue);
  strip.show();
  delay(interPixelDelay);
  if (nextStep >= cycleLength - 1) {
    delay(wait);
  }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void cyclePairs(uint16_t nextStep, uint16_t wait) {
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
  strip.setPixelColor(2* nextStep, cr, cg, cb); //set pixel to the first color
  strip.setPixelColor(2* nextStep + 1, ar, ab, ag); //set next pixel to 2nd color
  strip.show();

  if (nextStep == cycleLength - 1) {
    delay(wait);  //  Add a pause to enjoy the effect
  }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void blackLeadWipeColor(uint16_t nextStep, uint8_t color) {
  /* This sequence 'wipes' all the pixels, one at a time, with a color,
      but it leads the wipe with a dark, unlit pixel (like an eraser
      that changes the stripe color). */
  if (nextStep > 0) {
    uint8_t red = myColors[color];
    uint8_t green = myColors[color + 1];
    uint8_t blue = myColors[color + 2];
    strip.setPixelColor(nextStep - 1, red, green, blue);
  }
  if (nextStep < numLeds) {
    strip.setPixelColor(nextStep, 0);
  }
  strip.show();
  delay(interPixelDelay);
  if (nextStep >= cycleLength - 1) {
    uint16_t pause = (500 * random(2, 8));
    delay(pause);
  }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void whiteLeadWipeColor(uint16_t nextStep, uint8_t color) {
  /* This sequence 'wipes' all the pixels, one at a time, with a color,
      but it leads the wipe with a bright, white color (like a meteor
     passing, with a colored tail). */
  if (nextStep > 0) {
    uint8_t red = myColors[color];
    uint8_t green = myColors[color + 1];
    uint8_t blue = myColors[color + 2];
    strip.setPixelColor(nextStep - 1, red, green, blue);
  }
  if (nextStep < numLeds) {
    strip.setPixelColor(nextStep, 235, 235, 235);
  }
  strip.show();
  delay(interPixelDelay);
  if (nextStep >= cycleLength - 1) {
    uint16_t pause = (500 * random(2, 8));
    delay(pause);
  }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void wipeRandom(uint16_t nextStep, uint16_t wait) {
  uint8_t color = randomColor();
  uint8_t red = myColors[color];
  uint8_t green = myColors[color + 1];
  uint8_t blue = myColors[color + 2];
  strip.setPixelColor(nextStep, red, green, blue);
  strip.show();
  delay(interPixelDelay);
  if (nextStep >= cycleLength - 1) {
    delay(wait);
  }
}

#endif
