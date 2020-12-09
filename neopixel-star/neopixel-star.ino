// NeoPixel Star, using the Adafruit NeoPixel library
//   (for 7-point IKEA star lamp) by Zonker Harris
//     started at Hacker Dojo Grand Re-opening, NOV 11, 2016
//  loosely based on my SF Giants sign project v0.5

// https://github.com/pmjdebruijn/Arduino-Entropy-Library
#include <Entropy.h>

#include "my_colors.h"
#include "configuration.h"
#include "globals.h"
#include "randoms.h"
#include "sequences.h"

bool pressed = false;

// Interrupt Service Routine (ISR)
void onButtonPress() {
  SERIAL_PRINTLN("onButtonPress()");
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200) {
    pressed = true;
  }
  last_interrupt_time = interrupt_time;
}

void setup() {
  #ifdef DEBUG
  Serial.begin(9600);
  while (! Serial); // Wait untilSerial is ready
  #endif

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  uint32_t seed_value;

  // This routine sets up the watch dog timer with interrupt handler to
  // maintain a pool of real entropy for use in sketches.  This mechanism
  // is relatively slow since it will only produce a little less than
  // two 32-bit random values per second.
  Entropy.initialize();

  // The random method returns an unsigned 32-bit value, which can be
  // cast as a signed value if needed.  The function will wait until
  // sufficient entropy is available to return, which could cause
  //delays of up to approximately 500ms
  seed_value = Entropy.random();

  // By using the Entropy library to seed the normal pseudo-random
  // number generator which ensures that the standard libraries random
  // number generator will provide different starting values each time
  // the sketch is run.  This performs much better than the normal
  // randomSeed(analogRead(0)).
  randomSeed(seed_value);

  pinMode(PIN_SWITCH, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_SWITCH), onButtonPress, FALLING);
  // on UNO's, Int 0 = pin D2, and Int 1 = pin D3
  // on ATtiny85, Int 0 = pin PB2 (chip pin 7)

  pressed = false;
  SERIAL_PRINT("pressed? ");
  SERIAL_PRINTLN(pressed);

  // DEFINE THE INITIAL SEQUENCE HERE:
  sequence = BLACK_NOW;

  reset();
}

void loop() {
  if ((advanceMode == ON_BUTTON_PUSH && pressed) || advanceMode == AUTOMATIC) {
    switch (selectionMode) {
      case SEQUENTIAL:
        sequence = static_cast<LightSequence>((sequence + 1) % NUM_SEQUENCES);
        break;
      case RANDOM:
        sequence = randomSequence();
        break;
    }
    pressed = false;
    reset();
  }

  uint16_t innerStep = cycleLength > 0 ? (nextStep % cycleLength) : nextStep;

  //  *** These are the animations that can be chosen...
  switch (sequence) {
    case BLACK_NOW:
      SERIAL_PRINTLN("BLACK_NOW");
      // (If you select this manually, it looks like the pixels are all off...)
      setCycles(1);
      setCycleLength(1);
      blackNow();
      delay(500);
      break;
    case RAINBOW_CYCLE:
      SERIAL_PRINTLN("RAINBOW_CYCLE");
      // Set pixels to a rainbow, then cycles all pixels thru the rainbow, *twice*
      setSpeed(Entropy.random(10, 75)); // How slow should color shift? Higher = slower
      setCycles(advanceMode == AUTOMATIC ? Entropy.random(2, 5) : 1);
      setCycleLength(256 * 2);
      rainbowCycle(innerStep, speed);
      break;
    case RAINBOW:
      SERIAL_PRINTLN("RAINBOW");
      setSpeed(Entropy.random(10, 75));  // How slow should the colors shift? Higher number = slower
      setCycles(advanceMode == AUTOMATIC ? Entropy.random(2, 4) : 1);
      setCycleLength(256);
      // Set all pixels to color, then cycles the pixels through the rainbow once
      rainbow(innerStep, speed);
      break;
    case WIPE_COLOR:
      SERIAL_PRINTLN("WIPE_COLOR");
      setSpeed(35);
      setCycles(advanceMode == AUTOMATIC ? Entropy.random(1, 6) : 1);
      setCycleLength(numLeds);
      setBaseColor(randomColor());  // pick the base color for all pixels.
      if (innerStep == 0) {
        baseColor = randomColor();  // pick a new random color every cycle.
      }
      wipeColor(innerStep, baseColor, speed); // set all the pixels to that color
      break;
    case BLACK_LEAD_WIPE_COLOR:
      SERIAL_PRINTLN("BLACK_LEAD_WIPE_COLOR");
      /* This sequence 'wipes' all the pixels, one at a time, with
         a color, but it leads the wipe with a dark, unlit pixel
         (like an eraser that changes the stripe color) */
      setSpeed(15);
      setCycles(advanceMode == AUTOMATIC ? random(2, 7) : 1);
      setCycleLength(numLeds + 1);
      setBaseColor(randomColor());  // pick the base color for all pixels.
      if (innerStep == 0) {
        baseColor = randomColor();  // pick a new random color every cycle.
      }
      // Black pixel at the lead, followed by colors, cycles, delay
      blackLeadWipeColor(innerStep, baseColor);
      break;
    case WHITE_LEAD_WIPE_COLOR:
      SERIAL_PRINTLN("WHITE_LEAD_WIPE_COLOR");
      /* This sequence 'wipes' all the pixels, one at a time, with a
        color, but it leads the wipe with a bright, white color
        (like a meteor passing, with a colored tail). */
      setSpeed(15);
      setCycles(advanceMode == AUTOMATIC ? Entropy.random(2, 7) : 1);
      setCycleLength(numLeds + 1);
      setBaseColor(randomColor());  // pick the base color for all pixels.
      if (innerStep == 0) {
        baseColor = randomColor();  // pick a new random color every cycle.
      }
      // White pixel at the lead, followed by colors, cycles, delay
      whiteLeadWipeColor(innerStep, baseColor);
      break;
    case WIPE_RANDOM:
      SERIAL_PRINTLN("WIPE_RANDOM");
      /* This sequence 'wipes' all the pixels, one at a time, with a
        random color, but it leads the wipe with a dark, unlit pixel
        (like an eraser that changes the stripe color) */
      setSpeed(15);
      setCycles(advanceMode == AUTOMATIC ? Entropy.random(1, 6) : 1);
      setCycleLength(numLeds);
      wipeRandom(innerStep, interPixelDelay);  // set each pixel to a random color.
      break;
    case RANDOM_PIXELS:
      SERIAL_PRINTLN("RANDOM_PIXELS");
      // pick pixels at random, set each pixel to a random color
      setSpeed(500);  // Delay between changes.
      setCycles(1);
      // How many pixel changes? Pick a large range (low, high)
      setCycleLength(Entropy.random(20, 65));
      randomPixels(speed);
      break;
    case CYCLE_PAIRS:
      SERIAL_PRINTLN("CYCLE_PAIRS");
      /* The way the IKEA Star is wired, one pixel is on the "top/left"
         side of each arm, and the next pixel is on the "bottom/right
         side of that arm. This sequence tries to paint the "top" with
         one color, and the "bottom" with another color.  */
      setSpeed(15);
      setCycles(advanceMode == AUTOMATIC ? Entropy.random(1, 6) : 1);
      setCycleLength(numLeds / 2);
      setPauseLength(2000 * (Entropy.random(1, 6)) ); //  Add a pause to enjoy the effect
      cyclePairs(innerStep, pauseLength);
      break;
    default:
      // NUM_SEQUENCES is not a real value.
      reset();
  }

  nextStep++;
  if (nextStep >= cycles * cycleLength) {
    if (advanceMode == AUTOMATIC) {
      delay(interSequenceDelay);  //  Add a pause to enjoy the effect
      pressed = true;
    }
  }
}