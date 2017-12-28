
/*
 Animations

 Muisc Modes:
    1 - Falling Dot: White bar filled to soundpoint with floating blue dot at top.
    2 - Middle Out: White bar starting at middle filled in both directions to soundpoints
    3 - Ripple: 8 small Middle out type visualisations stacked from lowest to highest channel.
    4 - Fade: Whole saber filled to brightness level scaled with soundpoint. Has a minimum brightness point.
    5 - Rainbow: Rainbow colored bar filled to soundpoint. *Color can be changed by a push and hold.

  Variables:
   SoundLevel - the current sound level reading 
   dot - the slowing falling upper level pixel
   singleChannel - array of FFT values for each channel

  Basic Structure:
    Fill in LEDs with dim white up to soundLevel
    Fade out all LEDs over a few loops
    Fill in Blue pixel dot that floats at the top of soundLevel

  Fucntions from FastLED library:
  fadeToBlackBy
  fill_rainbow
  fill_solid
  blur1d
*/


#include <Arduino.h>
#include "FastLED.h"
#include <SPI.h>

FASTLED_USING_NAMESPACE

//Sound Variables
int soundLevel;    // this is the output of the FFT after being EQ
int dot = 100;    // 
int singleChannel[8];  //

//LED Variables
#define DATA_PIN    11 //MOSI  //11 Green
#define CLK_PIN     13 //SCK  //13 Blue
#define LED_TYPE    APA102
#define COLOR_ORDER BRG
#define NUM_LEDS    115   //115 for normal Saber, 48 for half saber 
CRGB leds[NUM_LEDS];
int Bvariable = 4;
#define FRAMES_PER_SECOND  120





void setup() {
 // tell FastLED about the LED strip configuration
 //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
 FastLED.addLeds<LED_TYPE, DATA_PIN, CLK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
 FastLED.setBrightness(90);
}


void loop() {

  musicmode1();
  //musicmode2();
}






void musicmode1() {   // Falling Dot

 if (soundLevel > dot) dot = soundLevel; // Keep dot on top of soundLevel
 if (dot > NUM_LEDS) dot = NUM_LEDS; // Keep dot from going out of frame
      
 for (int led = 0; led < soundLevel; led++) { // Start by Filling LEDS up to the soundLevel with dim white
    leds[led].setRGB( 80, 80, 80);       
  }

 // NO SOUND
  if (soundLevel <= 0) {       // If no sound (dot = 0)
    fadeToBlackBy( leds, NUM_LEDS, 30);   // fade all leds to black slowly  
  }

 // SOME SOUND
  if (soundLevel < NUM_LEDS) { // Partial column
    fadeToBlackBy( leds, NUM_LEDS, 30); // fade all LEDs to black slowly (new readings will maintain soundLevel at a constant level of fade)
  }

 // Fill in the 'peak' pixel with BLUE 
  leds[dot].setRGB( 0, 0, 255);
  for (int led = dot + 1; led < NUM_LEDS; led++) {   //make everything above the dot black
    leds[led].setRGB( 0, 0, 0);
  }
      
  FastLED.show();  // send data to LEDs to display
  
  if (dot > 0) {   // Let the dot drop by one each loop untill it is eventually bumped up by soundLevel 
    dot--;
  }
}






void musicmode2() {   // Middle Out - similar to musicmode1 but from the center and no dot

 if (soundLevel > dot) dot = soundLevel; // Keep dot on top of soundLevel
 if (dot > NUM_LEDS) dot = NUM_LEDS; // Keep dot from going out of frame

 //Start by filling in the LEDs with the soundLevel either side of the center with dim white     
 for (int led = (NUM_LEDS - soundLevel) / 2; led < (soundLevel / 2) + NUM_LEDS / 2; led++) {
    leds[led].setRGB( 50, 50, 50);
  }

 // NO SOUND
  if (soundLevel <= 0) {       // If no sound (dot = 0)
    fadeToBlackBy( leds, NUM_LEDS, 30);   // fade all leds to black slowly
    leds[NUM_LEDS / 2].setRGB( 80, 80, 80);  
  }

 // SOME SOUND
  if (soundLevel < NUM_LEDS) { // Partial column
    fadeToBlackBy( leds, NUM_LEDS, 30); // fade all LEDs to black slowly (new readings will maintain soundLevel at a constant level of fade)
  }

  FastLED.show();  // send data to LEDs to display
  
  if (dot > 0) {   // Let the dot drop by one each loop untill it is eventually bumped up by soundLevel 
    dot--;
  }
}




void musicmode3() {     // Ripple
  
  fadeToBlackBy(leds, NUM_LEDS, 10);

  for (int y = 0; y < 8; y++) {
    int bottomOfRipple = ((y * 15) + 6) - (singleChannel[y] / 10);
    if (bottomOfRipple <= 0) {
      bottomOfRipple = 0;
    }
  
    int topOfRipple = ((y * 15) + 6) + (singleChannel[y] / 10);
    if (topOfRipple >= NUM_LEDS - 1) {
      bottomOfRipple = NUM_LEDS - 1;
     }
  
    int rippleBrightness = constrain(singleChannel[y] * 3, 0, 255);
  
    for (int led = bottomOfRipple ; led < topOfRipple; led++) {
      leds[led] = CHSV(0, 0, rippleBrightness);
    }
    blur1d (leds, NUM_LEDS, singleChannel[y]);
    }
  
    FastLED.show();
}





void musicmode4() {   // Fade
  if (soundLevel > dot) dot = soundLevel; // Keep value on top
  
  for (int led = 0; led < NUM_LEDS; led++) {
    fill_solid( leds, NUM_LEDS, CRGB(dot, dot, dot));
  }
  
  FastLED.show();
  
  if (dot >= 35) {
    dot = dot - 4;
  }





  void musicmode5() {   // Rainbow - similar to musicmode1 but with color and no dot

 if (soundLevel > dot) dot = soundLevel; // Keep dot on top of soundLevel
 if (dot > NUM_LEDS) dot = NUM_LEDS; // Keep dot from going out of frame
      
 for (int led = 0; led < soundLevel; led++) { // Start by Filling LEDS up to the soundLevel with dim white
    fill_rainbow( leds, soundLevel, (musicVariable3 * 30), 1);       
  }

 // NO SOUND
  if (soundLevel <= 0) {       // If no sound (dot = 0)
    fadeToBlackBy( leds, NUM_LEDS, 50);   // fade all leds to black slowly  
  }

 // SOME SOUND
  if (soundLevel < NUM_LEDS) { // Partial column
    fadeToBlackBy( leds, NUM_LEDS, 50); // fade all LEDs to black slowly (new readings will maintain soundLevel at a constant level of fade)
  }

 // Fill in the 'peak' pixel with BLUE 
  leds[dot].setRGB( 0, 0, 255);
  for (int led = dot + 1; led < NUM_LEDS; led++) {   //make everything above the dot black
    leds[led].setRGB( 0, 0, 0);
  }
      
  FastLED.show();  // send data to LEDs to display
  
  if (dot > 0) {   // Let the dot drop by one each loop untill it is eventually bumped up by soundLevel 
    dot--;
  }
}