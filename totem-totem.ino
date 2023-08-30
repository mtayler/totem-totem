#include <Adafruit_NeoPixel.h>


#define TOTEM_ORANGE 255, 43, 0

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED1_PIN    7
#define LED2_PIN    6

// How many NeoPixels are attached to the Arduino?
#define LED1_COUNT 23
#define LED2_COUNT 23

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip1(LED1_COUNT, LED1_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(LED2_COUNT, LED2_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


// setup() function -- runs once at startup --------------------------------

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  strip1.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip2.begin();

  strip1.show();            // Turn OFF all pixels ASAP
  strip2.show();

  strip1.setBrightness(255); // Set BRIGHTNESS to about 1/5 (max = 255)
  strip2.setBrightness(255);
}


// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {
  // Fill along the length of the strip in various colors...
  colorWipe(strip1.Color(TOTEM_ORANGE), strip2.Color(TOTEM_ORANGE), 255); // Orange
//   colorWipe(strip.Color(  0, 255,   0), 50); // Green
//   colorWipe(strip.Color(  0,   0, 255), 50); // Blue

  // Do a theater marquee effect in various colors...
  theaterChase(strip1.Color(TOTEM_ORANGE), strip2.Color(TOTEM_ORANGE), 255); // Orange, half brightness
//   theaterChase(strip.Color(127,   0,   0), 50); // Red, half brightness
//   theaterChase(strip.Color(  0,   0, 127), 50); // Blue, half brightness

//   rainbow(10);             // Flowing rainbow cycle along the whole strip
//   theaterChaseRainbow(50); // Rainbow-enhanced theaterChase variant
}


// Some functions of our own for creating animated effects -----------------

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color1, uint32_t color2, int wait) {
  for(int i=0; i<strip1.numPixels(); i++) { // For each pixel in strip...
    strip1.setPixelColor(i, color1);         //  Set pixel's color (in RAM)
    strip2.setPixelColor(i, color2);         //  Set pixel's color (in RAM
    strip1.show();                          //  Update strip to match
    strip2.show();
    delay(wait);                           //  Pause for a moment
  }
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color1,uint32_t color2, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip1.clear();         //   Set all pixels in RAM to 0 (off)
      strip2.clear();
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip1.numPixels(); c += 3) {
        strip1.setPixelColor(c, color1); // Set pixel 'c' to value 'color'
        strip2.setPixelColor(c, color2);
      }
      strip1.show(); // Update strip with new contents
      strip2.show();
      delay(wait);  // Pause for a moment
    }
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    // strip.rainbow() can take a single argument (first pixel hue) or
    // optionally a few extras: number of rainbow repetitions (default 1),
    // saturation and value (brightness) (both 0-255, similar to the
    // ColorHSV() function, default 255), and a true/false flag for whether
    // to apply gamma correction to provide 'truer' colors (default true).
    strip1.rainbow(firstPixelHue);
    strip2.rainbow(firstPixelHue);
    // Above line is equivalent to:
    // strip.rainbow(firstPixelHue, 1, 255, 255, true);
    strip1.show(); // Update strip with new contents
    strip2.show();
    delay(wait);  // Pause for a moment
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip1.clear();         //   Set all pixels in RAM to 0 (off)
      strip2.clear();
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<strip1.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip1.numPixels();
        uint32_t color = strip1.gamma32(strip1.ColorHSV(hue)); // hue -> RGB
        strip1.setPixelColor(c, color); // Set pixel 'c' to value 'color'
        strip2.setPixelColor(c, color);
      }
      strip1.show();                // Update strip with new contents
      strip2.show();
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}
