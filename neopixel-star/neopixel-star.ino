// NeoPixel Star, using the Adafruit NeoPixel library
//   (for 7-point IKEA star lamp) by Zonker Harris
//     started at Hacker Dojo Grand Re-opening, NOV 11, 2016
//  loosely based on my SF Giants sign project v0.5

// https://github.com/pmjdebruijn/Arduino-Entropy-Library
#include <Entropy.h>

#include "configuration.h"
#include "globals.h"
#include "hardware.h"
#include "light_sequence.h"
#include "my_colors.h"
#include "randoms.h"
#include "sequences.h"

bool pressed = false;

// Interrupt Service Routine (ISR)
void onButtonPress() {
  SERIAL_PRINTLN("onButtonPress()");
  static unsigned long last_interrupt_time = 0;
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (millis() - last_interrupt_time > 200) {
    pressed = true;
  }
  last_interrupt_time = millis();
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
  // delays of up to approximately 500ms
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

  #ifndef __AVR_ATtiny85__
  analogReference(DEFAULT);
  #endif

  // DEFINE THE INITIAL SEQUENCE HERE:
  sequence = randomSequence();

  reset();
}

void loop() {
  // Slow the flicker a bit.
  static unsigned long nextTick;
  if (millis() < nextTick) {
    if (!pressed) { return; }
  }
  nextTick = millis() + 100;

  #ifdef MICROPHONE
  uint16_t volume = analogRead(PIN_MICROPHONE);
  for (uint8_t led = 0; led < NUM_LEDS; ++led) {
    strip.setPixelColor(led, Normalize(colors[led], volume));
    strip.show();
  }
  #endif

  // Instead of using the blocking delay(), set a delay using the pause()
  // method in globals.h; this way the loop will continue to react to button
  // presses during the delay.
  if (millis() < pauseEnd) {
    if (!pressed) { return; }
  }

  if (pressed) {
    #ifdef RANDOM_SEQUENCE
    sequence = randomSequence();
    #else
    sequence = static_cast<LightSequence>((sequence + 1) % NUM_SEQUENCES);
    #endif
    pressed = false;
    reset();

    // Black out all pixels between sequences.
    blackNow();
  }

  uint16_t innerStep = nextStep % cycleLength;

  #ifdef ADVANCE_AUTOMATICALLY
  setCycles(Entropy.random(2, 7));
  #else
  setCycles(1);
  #endif

  #ifdef WIPE_PATTERNS
  static uint8_t baseColor;
  #endif

  //  *** These are the animations that can be chosen...
  switch (sequence) {
    #ifdef RAINBOW_PATTERNS
    case RAINBOW_CYCLE:
      SERIAL_PRINTLNIF(innerStep == 0, "RAINBOW_CYCLE");
      // Set pixels to a rainbow, then cycles all pixels thru the rainbow, *twice*
      setCycleLength(256 * 2);
      rainbowCycle(innerStep);
      break;
    case RAINBOW:
      SERIAL_PRINTLNIF(innerStep == 0, "RAINBOW");
      // Set all pixels to color, then cycles the pixels through the rainbow once
      setCycleLength(256);
      rainbow(innerStep);
      break;
    #endif
    #ifdef WIPE_PATTERNS
    case WIPE_COLOR:
      SERIAL_PRINTLNIF(innerStep == 0, "WIPE_COLOR");
      setCycleLength(NUM_LEDS);
      if (innerStep == 0) {
        baseColor = randomColor();  // pick a new random color every cycle.
      }
      wipeColor(innerStep, baseColor); // set all the pixels to that color
      break;
    case BLACK_LEAD_WIPE_COLOR:
      SERIAL_PRINTLNIF(innerStep == 0, "BLACK_LEAD_WIPE_COLOR");
      /* This sequence 'wipes' all the pixels, one at a time, with
         a color, but it leads the wipe with a dark, unlit pixel
         (like an eraser that changes the stripe color) */
      setCycleLength(NUM_LEDS + 1);
      if (innerStep == 0) {
        baseColor = randomColor();  // pick a new random color every cycle.
      }
      // Black pixel at the lead, followed by colors, cycles, delay
      wipeColorWithLead(innerStep, BLACK, baseColor);
      break;
    case WHITE_LEAD_WIPE_COLOR:
      SERIAL_PRINTLNIF(innerStep == 0, "WHITE_LEAD_WIPE_COLOR");
      /* This sequence 'wipes' all the pixels, one at a time, with a
        color, but it leads the wipe with a bright, white color
        (like a meteor passing, with a colored tail). */
      setCycleLength(NUM_LEDS + 1);
      if (innerStep == 0) {
        baseColor = randomColor();  // pick a new random color every cycle.
      }
      // White pixel at the lead, followed by colors, cycles, delay
      wipeColorWithLead(innerStep, WHITE, baseColor);
      break;
    #endif
    #ifdef RANDOM_PATTERNS
    case WIPE_RANDOM:
      SERIAL_PRINTLNIF(innerStep == 0, "WIPE_RANDOM");
      /* This sequence 'wipes' all the pixels, one at a time, with a
        random color, but it leads the wipe with a dark, unlit pixel
        (like an eraser that changes the stripe color) */
      setCycleLength(NUM_LEDS);
      wipeRandom(innerStep);  // set each pixel to a random color.
      break;
    case RANDOM_PIXELS:
      SERIAL_PRINTLNIF(innerStep == 0, "RANDOM_PIXELS");
      // Pick pixels at random, set each pixel to a random color
      setCycles(1);
      // How many pixel changes? Pick a large range (low, high)
      setCycleLength(NUM_LEDS + Entropy.random(20, 65));
      if (innerStep < NUM_LEDS) {
        // First do a random wipe, to make sure all pixels are on
        wipeRandom(innerStep);
      } else {
        // Then, set pixels at random.
        randomPixels();
      }
      break;
    #endif
    case CYCLE_PAIRS:
      SERIAL_PRINTLNIF(innerStep == 0, "CYCLE_PAIRS");
      /* The way the IKEA Star is wired, one pixel is on the "top/left"
         side of each arm, and the next pixel is on the "bottom/right
         side of that arm. This sequence tries to paint the "top" with
         one color, and the "bottom" with another color.  */
      setCycleLength(NUM_LEDS / 2);
      cyclePairs(innerStep);
      break;
    default:
      // NUM_SEQUENCES is not a real value.
      reset();
  }

  nextStep++;
  SERIAL_PRINT("nextStep=");
  SERIAL_PRINTLN(nextStep);
  
  #ifdef ADVANCE_AUTOMATICALLY
  if (nextStep >= cycles * cycleLength) {
    pause(INTER_SEQUENCE_DELAY);  //  Add a pause to enjoy the effect
    pressed = true;
  }
  #endif
}
