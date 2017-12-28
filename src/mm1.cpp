
/*
 Music Mode One - Falling Dot: White bar filled to soundpoint with floating blue dot at top.

 Variables:
  SoundLevel - the current sound level reading 
  dot - the slowing falling upper level pixel

 Basic Structure:
  Fill in LEDs with dim white up to soundLevel
  Fade out all LEDs over a few loops
  Fill in Blue pixel dot that floats at the top of soundLevel

 States:
  NO SOUND
  SOME SOUND
  MAX SOUND
 
*/


#include <Arduino.h>
#include "FastLED.h"
#include <SPI.h>

FASTLED_USING_NAMESPACE

//Sound Variables
int soundLevel;    // this is the output of the FFT after being EQ
int dot = 100;

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

 // MAX SOUND
  else if (soundLevel == NUM_LEDS){     // If MAX sound (dot = NUM_LEDS) 
    for (int led = 0; led < soundLevel; led++) {
      leds[led].setRGB( 80, 80, 80); // fill in all leds to dim white
    }
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