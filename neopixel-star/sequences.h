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
  uint8_t value = (hsv[2] * MAX_COLOR / 2) + (hsv[2] * maxValue / 2);
  return strip.gamma32(
             strip.ColorHSV((uint16_t)(hsv[0] * MAX_HUE),
                            (uint8_t)(hsv[1] * MAX_COLOR),
                            value));
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
  strip.fill(0);
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    colors[i] = 0;
  }
  strip.show();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//   ** Original Adafruit example code
void rainbow(uint16_t color, uint16_t wait, uint16_t volume) {
  for (uint16_t pixel = 0; pixel < NUM_LEDS; ++pixel) {
    colors[pixel] = Wheel((pixel + color) & 255);
    strip.setPixelColor(pixel, Normalize(colors[pixel], volume));
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
    colors[pixel] = Wheel(((pixel * 256 / NUM_LEDS) + color) & 255);
    strip.setPixelColor(pixel, Normalize(colors[pixel], volume));
  }
  strip.show();
  pause(speed);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void randomPixels(uint16_t speed, uint16_t volume) {
  uint8_t i = Entropy.random(0, NUM_LEDS);
  uint8_t myBase = randomColor();
  // SERIAL_PRINTLN(myBase);
  colors[i] = strip.Color(
      myColors[myBase], 
      myColors[myBase + 1], 
      myColors[myBase + 2]);
  strip.setPixelColor(i, Normalize(colors[i], volume));
  strip.show();
  pause(speed);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void wipeColor(uint16_t nextStep, uint8_t color, uint16_t wait, uint16_t volume) {
  colors[nextStep] = strip.Color(
      myColors[color], 
      myColors[color + 1], 
      myColors[color + 2]);
  strip.setPixelColor(nextStep, Normalize(colors[nextStep], volume));
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

  // set pixel to the first color
  uint8_t pixel1 = 2 * nextStep;
  colors[pixel1] = strip.Color(
      myColors[c],
      myColors[c + 1],
      myColors[c + 2]);
  strip.setPixelColor(pixel1, Normalize(colors[pixel1], volume));

  // set next pixel to 2nd color
  uint8_t pixel2 = pixel1 + 1;
  colors[pixel2] = strip.Color(
      myColors[d],
      myColors[d + 1],
      myColors[d + 2]);
  strip.setPixelColor(pixel2, Normalize(colors[pixel2], volume));
  
  strip.show();

  if (nextStep == cycleLength - 1) {
    pause(wait);  //  Add a pause to enjoy the effect
  }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void wipeColorWithLead(uint16_t nextStep,
                       uint8_t leadColorRed,
                       uint8_t leadColorGreen,
                       uint8_t leadColorBlue,
                       uint8_t color,
                       uint16_t volume) {
  /* This sequence 'wipes' all the pixels, one at a time, with a color,
      but it leads the wipe with a given color. */
  // The brightness of the pixel depends on 'volume'.
  if (nextStep > 0) {
    colors[nextStep - 1] = strip.Color(
        myColors[color],
        myColors[color + 1],
        myColors[color + 2]);
    strip.setPixelColor(nextStep - 1, Normalize(colors[nextStep - 1], volume));
  }
  if (nextStep < NUM_LEDS) {
    colors[nextStep] = strip.Color(leadColorRed, leadColorGreen, leadColorBlue);
    strip.setPixelColor(nextStep, Normalize(colors[nextStep], volume));
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
  colors[nextStep] = strip.Color(
      myColors[color],
      myColors[color + 1],
      myColors[color + 2]);
  strip.setPixelColor(nextStep, Normalize(colors[nextStep], volume));
  strip.show();
  pause(INTER_PIXEL_DELAY);
  if (nextStep >= cycleLength - 1) {
    pause(wait);
  }
}

#endif
