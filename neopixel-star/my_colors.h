#ifndef MY_COLORS_H
#define MY_COLORS_H

#include "hardware.h"

// (0, 0, 0)
#define BLACK 0

// (235, 235, 235)
#define WHITE strip.Color(235, 235, 235)

//These triplets are the R, G, and B values that go to NeoPixels.
uint8_t myColors[] = {
  //(0, 0) brite yellow
  160, 171, 0,
  37, 45, 0,
  //(1, 6)  blue
  0, 0, 160,
  0, 0, 60,
  //(2, 12)  mango, dark orange
  145, 25, 0,
  75, 12, 0,
  //(3, 18)  emerald green
  0, 160, 0,
  0, 60, 0,
  //(4, 24)  dark pink
  135, 0, 26,
  45, 0, 10,
  //(5, 30)  salmon
  112, 24, 20,
  43, 12, 8,
  //(6, 36)  bright pink
  134, 0, 126,
  59, 0, 41,
  //(7, 42)  red
  160, 0, 0,
  60, 0, 0,
  //(8, 48)  orange yellow
  137, 104, 0,
  50, 44, 0,
  //(9, 54)  brite green-yellow
  110, 152, 0,
  59, 84, 0,
  //(10, 60)  dark purple
  62, 0, 114,
  31, 0, 56,
  //(11, 66)  fuschia
  180, 0, 157,
  53, 0, 42,
  //(12, 72)  light lime
  49, 154, 0,
  30, 75, 0,
  //(13, 78)  cyan
  0, 90, 117,
  0, 33, 45,
  //(14, 84)  dim blue
  6, 11, 100,
  3, 8, 50,
  //(15, 90)  teal
  0, 167, 43,
  0, 70, 20,
  //(16, 96)  orange
  160, 60, 0,
  66, 28, 0,
  //(17, 102)  light green
  43, 175, 10,
  22, 64, 8,
  //(18, 108)  lavender
  146, 104, 182,
  89, 71, 106,
#ifdef CHRISTMAS_COLORS
  // https://www.schemecolor.com/christmas-hex-color-codes.php
  // Cal Poly Pomona Green
  22, 91, 51,
  // Dark Spring Green
  20, 107, 58,	
  // Orange-Yellow
  248, 178, 41,
  // Cinnabar
  234, 70, 48,
  // Firebrick
  187, 37, 40,
#endif
#ifdef DADS_COLORS
  //(19, 114)  Dad's pink
  82, 12, 31,
  //(20, 120) Dad's Pale Yellow
  170, 68, 13,
  //(21, 126)  Dad's light-pink
  136, 43, 28,
  //(22, 132)  Dad's all red
  255, 0, 0,
  //(23, 132)  Dad's all blue
  0, 0, 255,
  //(23, 132)  Dad's light-blue
  20, 89, 255,
  //(24, 138)  Dad's rose
  40, 0, 26
#endif
};
uint8_t colorSets = (sizeof(myColors) / sizeof(uint8_t)) / 3;

#endif
