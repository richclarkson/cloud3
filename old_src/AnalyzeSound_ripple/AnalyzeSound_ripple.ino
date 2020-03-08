
//Adapted from: Spectrum Analyzer Basic Example

/*
   Key varriables and Outputs:

     Sensitivity - from 0-8 to scale the sound readings accordingly

     Mode - ripple vs other music modes, if ripple then ignor specific channel setting and use FFT to output all 8 channel values

     Channel - all vs specific channel, if all then output RMS, if specific channel read and output only that channel

     soundLevel - the cleaned and scaled RMS or single channel FFT output 0-115 to feed into the LED code

     fftArray[8] - the cleaned and scaled sound FFT output 0-115 to feed ripple mode

*/

#include <Audio.h>
#include <FastLED.h>      // v3.1.6
FASTLED_USING_NAMESPACE

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=139,91
AudioAnalyzeFFT1024      fft1024;        //xy=467,147
AudioAnalyzeRMS          rms1;           //xy=411,381
AudioConnection          patchCord1(i2s1, 0, fft1024, 0);
AudioConnection          patchCord2(i2s1, 0, rms1, 0);
// GUItool: end automatically generated code

//Global Varriables
int channel = 8;
int sensitivity = 4;  // 0-8 where 8 = maximum sensitivity


//Sound Varriables
// An array to hold the 8 frequency bands
float reading[8];
int fftArray[8];
int soundLevel = 0;
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



//LED Variables
#define DATA_PIN    2 // MOSI - Green on my strand
#define CLK_PIN     3 // SCK - Blue on my strand
#define LED_TYPE    APA102
#define COLOR_ORDER BGR
#define NUM_LEDS    115
CRGB leds[NUM_LEDS];

int dot;                 // A 'dot' to float on the top
int dotCount = 1;






void setup() {
  AudioMemory(12);     // Audio requires memory to work.
  Serial.begin(9600);
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(20);  // to save your eyeballs during testing
  FastLED.show();             // clear out any old data in the LEDs
}


void loop() {

  if (channel < 8) {    // channel specified and not ripple mode
    analyzeFFT();
    Serial.print(soundLevel);
    Serial.print(" ");
  }

  else if (channel == 8) {          // ripple mode
    analyzeFFTall();
    for (int i = 0; i < 8; i++) {
      Serial.print(fftArray[i]);
      Serial.print(" ");
    }
  }

  else if (channel == 9) {        //  channel not specified and not ripple mode
    analyzeLevel();
    Serial.print(soundLevel);
    Serial.print("     ");
  }

  Serial.println(NUM_LEDS);    // useful when stopping the serial plotter from autoscalling
  musicmode3();
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





void updateLEDs() {
  for (int led = 0; led < NUM_LEDS; led++) {  // set all LEDs to off
    leds[led].setRGB( 0, 0, 0);
  }

  for (int led = 0; led < soundLevel; led++) {  // fill LEDs up to rmsReading with white
    leds[led].setRGB( 80, 80, 80);
  }

  if (soundLevel > dot) {
    dot = soundLevel;  // keep a 'dot' on top of rmsReading
  }

  leds[dot].setRGB( 0, 0, 255);             // set the dot as a blue pixel

  if (++dotCount >= 30) {                   // make the dot fall slowly
    dotCount = 0;
    if (dot > 1) {
      dot--;
    }
  }
  FastLED.show();
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


void turnoffLEDs()
{
    for (int led = 0; led < NUM_LEDS; led++)        
  { //turn off LEDs
    leds[led] = CHSV( 100, 0, 0);
  }
}


