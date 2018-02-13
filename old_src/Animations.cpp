
/*
 Animations

 Muisc Modes:
    1 - Falling Dot: White bar filled to soundpoint with floating blue dot at top.
    2 - Middle Out: White bar starting at middle filled in both directions to soundpoints
    3 - Ripple: 8 small Middle out type visualisations stacked from lowest to highest channel.
    4 - Fade: Whole saber filled to brightness level scaled with soundpoint. Has a minimum brightness point.
    5 - Rainbow: Rainbow colored bar filled to soundpoint. *Color can be changed by a push and hold.

  Lamp Modes:
    1 - Neon: Rainbow - similar to musicmode1 but with color and no dot
    2 - White: Solid white light across whole Saber
    3 - Ombre: Rainbow color gradient with linear cycle
    4 - Fire: Visualization of linear fire

  Fucntions used that are from FastLED library:
   External (i assume are somewhere in lib?):
     fadeToBlackBy
     fill_rainbow
     fill_solid
     blur1d
     setRGB
     CHSV
   Internal (writen in here):
     rainbow
     Fire2012
     HeatColor2 - required as part of Fire2012

*/

#include <Arduino.h>
#include <FastLED.h>
//#include <SPI.h>

FASTLED_USING_NAMESPACE

//Sound Variables
int soundLevel;          // this is the output of the FFT after being EQ
int dot = 100;           // this is a slowly falling value based on the peaks of soundLevel, used by musicMode1 and musicMode4
int singleChannel[8];    // array of FFT values for each channel used in musicMode3
int RainbowVariable = 0; // start position of Rainbow color wheel used in musicMode5

//Lamp Mode Variables
int rainbowCounter = 0;
int timeSpeed;
uint8_t gHue = 0;           // rotating "base color" used by many of the patterns
int COOLINGarray[] = {100, 90, 85, 80, 75, 90, 85, 100, 90}; // Fire Mode varriable
int COOLING = 90;         // Fire Mode varriable
#define SPARKING 100      // Fire Mode varriable
static byte heat[100];
int capture[100];

//LED Variables
#define DATA_PIN 2 //MOSI  //11 Green
#define CLK_PIN 3  //SCK  //13 Blue
#define LED_TYPE APA102
#define COLOR_ORDER BRG
#define NUM_LEDS 115 //115 for normal Saber, 48 for half saber
CRGB leds[NUM_LEDS];
#define FRAMES_PER_SECOND 120

void musicmode1();
void musicmode2();
void musicmode3();
void musicmode4();
void musicmode5();

void lampmode1();
void lampmode2();
void lampmode3();
void lampmode4();

void rainbow(int startPos, int number, float deltaHue);
void Fire2012();
CRGB HeatColor2( uint8_t temperature);


void setup()
{
  // tell FastLED about the LED strip configuration
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(90);

  musicmode1();
  musicmode2();
  musicmode3();
  musicmode4();
  musicmode5();

  lampmode1();
  lampmode2();
  lampmode3();
  lampmode4();

  rainbow(0,NUM_LEDS,1);
  Fire2012();
  HeatColor2(10);

}

void loop()         //random loop code
{
  // musicmode1();
  // musicmode2();
  // musicmode3();
  // musicmode4();
  // musicmode5();

  //lampmode1();
  //lampmode2();
  //lampmode3();
  lampmode4();

  delay(10);
}



void musicmode1()   // Falling Dot
{ 

  if (soundLevel > dot)
    dot = soundLevel; // Keep dot on top of soundLevel
  if (dot > NUM_LEDS)
    dot = NUM_LEDS; // Keep dot from going out of frame

  for (int led = 0; led < soundLevel; led++)
  { // Start by Filling LEDS up to the soundLevel with dim white
    leds[led].setRGB(80, 80, 80);
  }

  fadeToBlackBy(leds, NUM_LEDS, 30); // fade all LEDs to black slowly (new readings will maintain soundLevel at a constant level of fade)

  // Fill in the 'peak' pixel with BLUE
  leds[dot].setRGB(0, 0, 255);
  for (int led = dot + 1; led < NUM_LEDS; led++)
  { //make everything above the dot black
    leds[led].setRGB(0, 0, 0);
  }

  FastLED.show(); // send data to LEDs to display

  if (dot > 0)
  { // Let the dot drop by one each loop untill it is eventually bumped up by soundLevel
    dot--;
  }
}



void musicmode2()   // Middle Out - similar to musicmode1 but from the center and no dot
{ 

  //Start by filling in the LEDs with the soundLevel either side of the center with dim white
  for (int led = (NUM_LEDS - soundLevel) / 2; led < (soundLevel / 2) + NUM_LEDS / 2; led++)
  {
    leds[led].setRGB(50, 50, 50);
  }

  // NO SOUND
  if (soundLevel <= 0)
  {                                    // If no sound (dot = 0)
    fadeToBlackBy(leds, NUM_LEDS, 30); // fade all leds to black slowly
    leds[NUM_LEDS / 2].setRGB(80, 80, 80); // keep center dot illuminated
  }

  // SOME SOUND
  if (soundLevel < NUM_LEDS)
  {                                    // Partial column
    fadeToBlackBy(leds, NUM_LEDS, 30); // fade all LEDs to black slowly (new readings will maintain soundLevel at a constant level of fade)
  }

  FastLED.show(); // send data to LEDs to display
}



void musicmode3()    // Ripple
{ 

  fadeToBlackBy(leds, NUM_LEDS, 10); // start by fading out old values

  for (int y = 0; y < 8; y++) // create 8 different LED sections of saber each based on the 8 FFT channels
  {
    int bottomOfRipple = ((y * 15) + 6) - (singleChannel[y] / 10);
    if (bottomOfRipple <= 0)
    {
      bottomOfRipple = 0;
    }

    int topOfRipple = ((y * 15) + 6) + (singleChannel[y] / 10);
    if (topOfRipple >= NUM_LEDS - 1)
    {
      bottomOfRipple = NUM_LEDS - 1;
    }

    int rippleBrightness = constrain(singleChannel[y] * 3, 0, 255);

    for (int led = bottomOfRipple; led < topOfRipple; led++)
    {
      leds[led] = CHSV(0, 0, rippleBrightness); // fill in LEDs according to the top and bottom of each section deffined above
    }
    blur1d(leds, NUM_LEDS, singleChannel[y]);  // blur LEDs for smoother transitions
  }

  FastLED.show();
}



void musicmode4()   // Fade
{ 
  
  if (soundLevel > dot)
    dot = soundLevel; // Keep dot on top of soundLevel
  if (dot > NUM_LEDS)
    dot = NUM_LEDS; // Keep dot from going out of frame
      

  for (int led = 0; led < NUM_LEDS; led++)
  {
    fill_solid(leds, NUM_LEDS, CRGB(dot, dot, dot));
  }

  FastLED.show();

  if (dot >= 35)
  {
    dot = dot - 4;
  }
}



void musicmode5()     // Rainbow - similar to musicmode1 but with color and no dot
{ 

  for (int led = 0; led < soundLevel; led++)
  { // Start by Filling LEDS up to the soundLevel with coulor spectrum
    fill_rainbow(leds, soundLevel, (RainbowVariable * 30), 1);
  }

  // NO SOUND
  if (soundLevel <= 0)
  {                                    // If no sound (dot = 0)
    fadeToBlackBy(leds, NUM_LEDS, 50); // fade all leds to black somewhat slowly
  }

  // SOME SOUND
  if (soundLevel < NUM_LEDS)
  {                                    // Partial column
    fadeToBlackBy(leds, NUM_LEDS, 50); // fade all LEDs to black somewhat slowly (new readings will maintain soundLevel at a constant level of fade)
  }

  FastLED.show(); // send data to LEDs to display
}







void lampmode1()  // Neon
{
  rainbow(0, NUM_LEDS, 0.1);
}


void lampmode2()  // White
{
  for (int led = 0; led < NUM_LEDS; led++) {
        leds[led].setRGB( 150, 150, 150);
      }
      FastLED.show();
}


void lampmode3()  // Ombre
{
  rainbow(0, NUM_LEDS, 1);
}


void lampmode4()  // Fire
{
  random16_add_entropy( random());
      Fire2012();
      FastLED.show(); // display this frame
      FastLED.delay(1000 / 60);
}




void rainbow(int startPos, int number, float deltaHue) {
  
    if (++rainbowCounter >= timeSpeed) {
      gHue++;
      rainbowCounter = 0;
    } // slowly cycle the "base color" through the rainbow
  
    fill_rainbow( &(leds[startPos]), number, gHue, deltaHue);
    FastLED.show();
  }
  
  
  
  
  
  void Fire2012()
  {
    // Array of temperature readings at each simulation cell
    //static byte heat[100];
    //capture[NUM_LEDS - 15];
  
  
    // Step 1.  Cool down every cell a little
    for ( uint8_t i = 0; i < NUM_LEDS; i++) {
      capture[i] = ( capture[i] -  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
      if (capture[i] < 0) {
        capture[i] = 0;
      }
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for ( uint8_t k = NUM_LEDS - 3; k > 0; k--) {
      capture[k] = (capture[k - 1] + capture[k - 2] + capture[k - 2] ) / 3;
    }
  
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if ( random8() < SPARKING ) {
      uint8_t y = random8(7);
      capture[y] = ( capture[y] + random8(160, 255) );
      if (capture[y] > 255) {
        capture[y] = 255;
      }
    }
  
    // Step 4.  Map from heat cells to LED colors
    for ( uint8_t j = 0; j < NUM_LEDS; j++) {
      leds[j] = HeatColor2( capture[j]);
    }
  }
  
  
  CRGB HeatColor2( uint8_t temperature)
  {
    CRGB heatcolor2;
  
    // Scale 'heat' down from 0-255 to 0-191,
    // which can then be easily divided into three
    // equal 'thirds' of 64 units each.
    uint8_t t192 = scale8_video( temperature, 192);
  
    // calculate a value that ramps up from
    // zero to 255 in each 'third' of the scale.
    uint8_t heatramp = t192 & 0x3F; // 0..63
    heatramp <<= 2; // scale up to 0..252
  
    // now figure out which third of the spectrum we're in:
    if ( t192 & 0x80) {
      // we're in the hottest third
      heatcolor2.g = 255; // full red
      heatcolor2.r = 255; // full green
      heatcolor2.b = heatramp; // ramp up blue
  
    } else if ( t192 & 0x40 ) {
      // we're in the middle third
      heatcolor2.g = 255; // full red
      heatcolor2.r = (heatramp * 0.5); // ramp up green
      heatcolor2.b = 0; // no blue
  
    } else {
      // we're in the coolest third
      heatcolor2.g = heatramp; // ramp up red
      heatcolor2.r = 0; // no green
      heatcolor2.b = 0; // no blue
    }
  
    return heatcolor2;
  }

  

