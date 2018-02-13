
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
*/

#include <Arduino.h>
#include <FastLED.h>
#include <Audio.h>     // v1.0.5

FASTLED_USING_NAMESPACE

//Global Varriables
int channel = 8;
int sensitivity = 4;  // 0-8 where 8 = maximum sensitivity
int brightness = 4;

//Sound Variables
int soundLevel;          // this is the output of the FFT after being EQ
int dot = 100;           // this is a slowly falling value based on the peaks of soundLevel, used by musicMode1 and musicMode4
int dotCount = 1;
int lampMode2Count = 1;

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=139,91
AudioAnalyzeFFT1024      fft1024;        //xy=467,147
AudioAnalyzeRMS          rms1;           //xy=411,381
AudioConnection          patchCord1(i2s1, 0, fft1024, 0);
AudioConnection          patchCord2(i2s1, 0, rms1, 0);
// GUItool: end automatically generated code

// An array to hold the 8 frequency bands
float reading[8];
int fftArray[8];
int fftSingle;

// This is low-level noise that's subtracted from each frequency band:
static const int noise[8] = {   // numbers generated using serial plotter at room tone at x10000 factor
  20, 05, 04, 06, 22, 64, 68, 89            // these are converted x 0.0001 later on before subtraction
};
static const int binStart[8] = {   //
  0, 2, 3, 5, 9, 27, 99, 227            //
};
static const int binEnd[8] = {   //
  1, 2, 4, 8, 26, 98, 226, 511            //
};
static const int scale[9] = {
  300, 1000, 3000, 6000, 10000, 20000, 30000, 50000, 100000   // sensitivity setting (mulitplication factor)
};
float eq[8] = {
  1.0, 1.0, 0.7, 0.5, 1.0, 1.0, 1.5, 2.0   // individual channel scaling factor
};


//Lamp Mode Variables
int rainbowCounter = 0;
int timeSpeed = 100;
uint8_t gHue = 0;           // rotating "base color" used by many of the patterns
int COOLINGarray[] = {100, 90, 85, 80, 75, 90, 85, 100, 90}; // Fire Mode varriable
int COOLING = 90;         // Fire Mode varriable
#define SPARKING 100      // Fire Mode varriable
//static byte heat[100];
int capture[100];

//LED Variables
#define DATA_PIN 2 //MOSI  //7 Green
#define CLK_PIN 3  //SCK  //14 Blue
#define LED_TYPE APA102
#define COLOR_ORDER BGR
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
void turnoffLEDs();
void analyzeLevel();
void analyzeFFT();
void analyzeFFTall();
void FFTreading(int FFTchannel);



void setup()
{
  AudioMemory(12);     // Audio requires memory to work.
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(90);
  turnoffLEDs();
  FastLED.show();
  delay(1000);  // Sanity Delay
}


void loop()
{ 
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
}




void musicmode1()   // Falling Dot
{ 
  if (soundLevel > dot)  dot = soundLevel; // Keep dot on top of soundLevel
  if (dot > NUM_LEDS)    dot = NUM_LEDS; // Keep dot from going out of frame

  turnoffLEDs();
  
  for (int led = 0; led < soundLevel; led++)
  { // Start by Filling LEDS up to the soundLevel with dim white
    leds[led].setRGB(80, 80, 80);
  }
  leds[dot].setRGB(0, 0, 255);    // Fill in the 'peak' pixel with BLUE
  
  for (int led = dot + 1; led < NUM_LEDS; led++)
  { //make everything above the dot black
    leds[led].setRGB(0, 0, 0);
  }
  FastLED.show(); // send data to LEDs to display

  if (++dotCount >= 60) {                   // make the dot fall slowly
    dotCount = 0;
    if (dot > 1) {
      dot--;
    }
  }
}



void musicmode2()   // Middle Out - similar to musicmode1 but from the center and no dot
{ 
  turnoffLEDs();
  for (int led = (NUM_LEDS - soundLevel) / 2; led < (soundLevel / 2) + (NUM_LEDS / 2); led++)
  {
    leds[led].setRGB(50, 50, 50);
  }
  if (soundLevel <= 0)  // NO SOUND
  {                                    // If no sound (dot = 0)
    turnoffLEDs();
    leds[NUM_LEDS / 2].setRGB(80, 80, 80); // keep center dot illuminated
  }
  FastLED.show(); // send data to LEDs to display
}






void musicmode3()    // Ripple
{ 
  fadeToBlackBy( leds, NUM_LEDS, 1);
  //turnoffLEDs();
  for (int y = 0; y < 8; y++) // create 8 different LED sections of saber each based on the 8 FFT channels
  {
    int bottomOfRipple = ((y * 15) + 6) - (fftArray[y] / 10);
    if (bottomOfRipple <= 0)
    {
      bottomOfRipple = 0;
    }
    int topOfRipple = ((y * 15) + 6) + (fftArray[y] / 10);
    if (topOfRipple >= NUM_LEDS - 1)
    {
      topOfRipple = NUM_LEDS - 1;
    }
    int rippleBrightness = constrain(fftArray[y] * 3, 0, 254);
    for (int led = bottomOfRipple; led < topOfRipple; led++)
    {
      leds[led] = CHSV(0, 0, rippleBrightness); // fill in LEDs according to the top and bottom of each section deffined above
    }
    blur1d(leds, NUM_LEDS, fftArray[y]);  // blur LEDs for smoother transitions
  }
  FastLED.show();
}



void musicmode4()   // Fade
{ 
  if (soundLevel > dot){    dot = soundLevel;  } // Keep dot on top of soundLevel
  if (dot > NUM_LEDS){    dot = NUM_LEDS-1;  }// Keep dot from going out of frame

  for (int led = 0; led < NUM_LEDS; led++)
  {
    leds[led] = CHSV( 100, 0, dot);
  }
  FastLED.show();
  if (++dotCount >= 10) {                   // make the dot fall slowly
    dotCount = 0;
    if (dot > 5) {
      dot--;
    }
  }
}



void musicmode5()     // Rainbow - similar to musicmode1 but with color and no dot
{ 
  fill_gradient(leds, 0, CHSV(96, 255,255) , NUM_LEDS, CHSV(0,255,255), SHORTEST_HUES);
    for (int led = soundLevel; led < NUM_LEDS; led++)        
  { //turn off LEDs
    leds[led] = CHSV( 100, 0, 0);
  }
  if (soundLevel <= 0)  // NO SOUND
  {                                    // If no sound (dot = 0)
   turnoffLEDs();
  }
  FastLED.show(); // send data to LEDs to display
}





void turnoffLEDs()
{
    for (int led = 0; led < NUM_LEDS; led++)        
  { //turn off LEDs
    leds[led] = CHSV( 100, 0, 0);
  }
}





void lampmode1()  // Neon
{
  rainbow(0, NUM_LEDS, 0.1);
}

void lampmode2()  // White
{
  for (int led = 0; led < NUM_LEDS; led++) {
        leds[led] = CHSV( 100, 0, 200);
      }
      FastLED.show();
      lampMode2Count = 0;
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
  static byte heat[115];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber = j;
      leds[pixelnumber] = color;
    }
}



void analyzeLevel() {
  if (rms1.available()) {
    soundLevel = (rms1.read() - 0.0006) * scale[sensitivity];     // remove noise and scale
    soundLevel = constrain(soundLevel, 0, NUM_LEDS);              // limit
  }
}

void analyzeFFT() {
  if (fft1024.available()) {
    FFTreading(channel);
    soundLevel = fftArray[channel];
  }
}

void analyzeFFTall() {
  if (fft1024.available()) {
    for (int i = 0; i < 8; i++) {
      FFTreading(i);
    }
  }
}

void FFTreading(int FFTchannel) {
  reading[FFTchannel] =  fft1024.read(binStart[FFTchannel], binEnd[FFTchannel]);
  reading[FFTchannel] = reading[FFTchannel] - (noise[FFTchannel] * 0.0001);           // remove noise
  fftArray[FFTchannel] = (reading[FFTchannel] * scale[sensitivity]) * eq[FFTchannel]; // scale
  fftArray[FFTchannel] = constrain(fftArray[FFTchannel], 0, NUM_LEDS);                // limit
}
  


