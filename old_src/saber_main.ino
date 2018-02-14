/*

  Saber 2.1 by Richard Clarkson 2017

  Hardware:
  Adafruit Metro Mini 328 - 5V 16MHz
  Standalone Momentary Capacitive Touch Sensor Breakout - AT42QT1010
  Electret Microphone Amplifier - MAX4466 with Adjustable Gain
  APA102 LED Strip with 144 LEDs per meter
  5v 4amp Power Supply

  Urgent TODO:
  Clean up code
  Establish debugging structure
  Stop "board freezing" from sound spike in Music Modes
  Stop touch sensor jumping out of mode in Lamp Modes
  Reduce sketch size

  Future TODO:
  Improve sound accuracy (especially on low end frequeny)
  Improve sensitivity range
  Make reversable (able to hang upside down)
  Wireless APP
  Single Color Neon Mode
  Rain Mode
*/


// IMPORTANT: FFT_N should be #defined as 128 in ffft.h.

//#define DEVMODE 1;       //  UNCOMMENT TO WORK IN DEVMODE (COMMAND + /)

#include <SPI.h>
#include <EEPROM.h>
#include <avr/pgmspace.h>
#include <ffft.h>
#include <math.h>
#include <Wire.h>
#include "FastLED.h"
FASTLED_USING_NAMESPACE

//extern const int16_t tbl_window[];

#define ADC_CHANNEL 0

int16_t       capture[FFT_N];    // Audio capture buffer
complex_t     bfly_buff[FFT_N];  // FFT "butterfly" buffer
uint16_t      spectrum[FFT_N / 2]; // Spectrum output buffer
volatile byte samplePos = 0;     // Buffer position counter

byte newEpprom;

// Fire Mode
int COOLINGarray[] = {100, 90, 85, 80, 75, 90, 85, 100, 90};
int COOLING = 90;
//#define COOLING 90
#define SPARKING 100
//int SPARKING = 120;
// static byte heat[115];

//Button Variables
const int buttonPin = 7;
uint8_t buttonState;
int lastButtonState;
uint8_t buttonPushCounter;
uint8_t prevButtonPushCounter;
const int numberOfModes = 7;                  //number of modes
uint8_t mode = 0;
int indicatorTime = 200;
uint8_t buttonHeld = 0;


//Music Variables
int BinsBinsBins[7]; // My array for averaging all channels
long highSoundlLvel;

long checking = 0;

uint8_t dotBrightness = 250;
uint8_t dotBrightnessDirection = 1;

uint8_t automatedIndicator;
int randomFade;
int prevRandomFade;
int autoPeak;
uint8_t variableCounter = 9;  //global
uint8_t singlePress;
int milisCounter = 0;
int timeCounter = 0;
int threashold = 1500;
int buttonCounter = 0;
uint8_t reset = 0;
//uint8_t demo;
uint8_t timeSpeed = 9;
uint8_t rainbowCounter = 0;

int firstPressHold = 1;

int rainDrop = 114;
int newDrop = 1;
int dropSpeedCounter;
int dropBrightness;


int musicVariable3;

int modeColor;
int indcatorDots;
uint8_t ledCimber = 8;
int prevVariableSet = 0;

uint8_t normal;
uint8_t demoAuto;

uint8_t pushAndHold = 0;
uint8_t numberOfSettingsModes = 4;
int pushAndHoldTimer = 1000;
uint8_t buttonPushCounterDemo;

int fallingDot;
int color;

uint8_t sensitivity;

int globalfade = 0;
int hueSelect;

int rippleV[8];

int allBins;
int peakAll;
uint8_t channel;        // change this to set defualt frequency channel!

uint8_t gHue = 0;           // rotating "base color" used by many of the patterns

uint8_t soundLevel;


//LED Variables
#define DATA_PIN    11 //MOSI  //11 Green
#define CLK_PIN     13 //SCK  //13 Blue
#define LED_TYPE    APA102
#define COLOR_ORDER BRG
#define NUM_LEDS    30   //115 for normal Saber, 48 for half saber 
CRGB leds[NUM_LEDS];


uint8_t minMaxThershold = 8;     // same as NUM_LED
int Bvariable = 4;
//int BRIGHTNESS;

#define FRAMES_PER_SECOND  120



//FFFT Variables
byte
peak[8],      // Peak level of each column; used for falling dots
      dotCount = 0, // Frame counter for delaying dot-falling speed
      colCount = 0; // Frame counter for storing past column data
int
col[8][10],   // Column levels for the prior 10 frames
    minLvlAvg[8], // For dynamic adjustment of low & high ends of graph,
    maxLvlAvg[8], // pseudo rolling averages for the prior few frames.
    colDiv[8];    // Used when filtering FFT output to 8 columns


/*
  These tables were arrived at through testing, modeling and trial and error,
  exposing the unit to assorted music and sounds.  But there's no One Perfect
  EQ Setting to Rule Them All, and the graph may respond better to some
  inputs than others.  The software works at making the graph interesting,
  but some columns will always be less lively than others, especially
  comparing live speech against ambient music of varying genres.
*/
static const uint8_t PROGMEM
//  heat[80]={}
//,
// This is low-level noise that's subtracted from each FFT output column:
noise[64] = {
  10, 7, 6, 5, 3, 4, 4, 4, 3, 4, 4, 3, 2, 3, 3, 4,
  2, 1, 2, 1, 3, 2, 3, 2, 1, 2, 3, 1, 2, 3, 4, 4,
  3, 2, 2, 2, 2, 2, 2, 1, 3, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 4
}
,

// These are scaling quotients for each FFT output column, sort of a
// graphic EQ in reverse.  Most music is pretty heavy at the bass end.
eq[64] = {
  255, 245, 218, 225, 220, 198, 147, 99, 68, 47, 33, 22, 14,  8,  4,  2,
  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
}
,
// When filtering down to 8 columns, these tables contain indexes
// and weightings of the FFT spectrum output values to use.  Not all
// buckets are used -- the bottom-most and several at the top are
// either noisy or out of range or generally not good for a graph.
col0data[] = {
  2,  1,  // # of spectrum bins to merge, index of first
  90,   8
}
,           // Weights for each bin
col1data[] = {
  4,  1,  // 4 bins, starting at index 1
  19, 186,  38,   2
}
, // Weights for 4 bins.  Got it now?
col2data[] = {
  5,  2,
  11, 156, 118,  16,   1
}
,
col3data[] = {
  8,  3,
  5,  55, 165, 164,  71,  18,   4,   1
}
,
col4data[] = {
  11,  5,
  3,  24,  89, 169, 178, 118,  54,  20,   6,   2,   1
}
,
col5data[] = {
  17,  7,
  2,   9,  29,  70, 125, 172, 185, 162, 118, 74,
  41,  21,  10,   5,   2,   1,   1
}
,
col6data[] = {
  25, 11,
  1,   4,  11,  25,  49,  83, 121, 156, 180, 185,
  174, 149, 118,  87,  60,  40,  25,  16,  10,   6,
  4,   2,   1,   1,   1
}
,
col7data[] = {
  37, 16,
  1,   2,   5,  10,  18,  30,  46,  67,  92, 118,
  143, 164, 179, 185, 184, 174, 158, 139, 118,  97,
  77,  60,  45,  34,  25,  18,  13,   9,   7,   5,
  3,   2,   2,   1,   1,   1,   1
}
,
// And then this points to the start of the data for each of the columns:
* const colData[] = {
  col0data, col1data, col2data, col3data,
  col4data, col5data, col6data, col7data
};










void setup() {

#if defined(DEVMODE)
  Serial.begin(9600);
#endif


  analogReference(EXTERNAL);
  pinMode(buttonPin, INPUT);           // set pin to input
  // tell FastLED about the LED strip configuration
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );

  FastLED.setBrightness(90);

  uint8_t i, j, nBins, binNum, *data;

  memset(peak, 0, sizeof(peak));
  memset(col , 0, sizeof(col));

  for (i = 0; i < 8; i++) {
    minLvlAvg[i] = 0;
    maxLvlAvg[i] = 512;
    data         = (uint8_t *)pgm_read_word(&colData[i]);
    nBins        = pgm_read_byte(&data[0]) + 2;
    binNum       = pgm_read_byte(&data[1]);
    for (colDiv[i] = 0, j = 2; j < nBins; j++)
      colDiv[i] += pgm_read_byte(&data[j]);
  }


  // Init ADC free-run mode; f = ( 16MHz/prescaler ) / 13 cycles/conversion
  ADMUX  = ADC_CHANNEL; // Channel sel, right-adj, use AREF pin
  ADCSRA = _BV(ADEN)  | // ADC enable
            _BV(ADSC)  | // ADC start
            _BV(ADATE) | // Auto trigger
            _BV(ADIE)  | // Interrupt enable
            _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0); // 128:1 / 13 = 9615 Hz
  ADCSRB = 0;                // Free run mode, no high MUX bit
  DIDR0  = 1 << ADC_CHANNEL; // Turn off digital input for ADC pin
  TIMSK0 = 0;                // Timer0 off

  sei(); // Enable interrupts

  // LOAD EEPROM

  TIMSK0 = 1;                // Timer0 back on
  ADCSRA &= ~_BV(ADIE);      //Stop Interupt


  eepromSet();       //Load EPROM Values
}







void loop() {


  buttonChecker();

  if (normal == 1) {

    if      (buttonPushCounter == 0) {    // falling dot
      EEPROM.update(1, buttonPushCounter);
      buttonPushCounter = 100;
      buttonPushCounterDemo = 100;
      pushAndHold = 0;
      indicatorDemo(20);
      allBins = 0;
      TIMSK0 = 0;                // Timer0 off
    }

    else if (buttonPushCounter == 1) {    // middle out
      EEPROM.update(1, buttonPushCounter);
      buttonPushCounter = 101;
      buttonPushCounterDemo = 101;
      pushAndHold = 0;
      indicatorDemo(20);
      TIMSK0 = 0;                // Timer0 off
      allBins = 0;
    }

    else if (buttonPushCounter == 2) {    // ripple
      EEPROM.update(1, buttonPushCounter);
      buttonPushCounter = 102;
      buttonPushCounterDemo = 102;
      pushAndHold = 0;
      indicatorDemo(20);
      TIMSK0 = 0;                // Timer0 off
      allBins = 0;
    }

    else if (buttonPushCounter == 3) {    // fade
      EEPROM.update(1, buttonPushCounter);
      buttonPushCounter = 103;
      buttonPushCounterDemo = 103;
      pushAndHold = 0;
      indicatorDemo(20);
      allBins = 0;
      peakAll = 35;
      //minMaxThershold = NUM_LEDS;
      TIMSK0 = 0;                // Timer0 off
    }

    else if (buttonPushCounter == 4) {    // music rainbow
      EEPROM.update(1, buttonPushCounter);
      buttonPushCounter = 104;
      buttonPushCounterDemo = 104;
      pushAndHold = 2;
      indicatorDemo(20);
      TIMSK0 = 0;                // Timer0 off
      allBins = 0;
    }

    else if (buttonPushCounter == 5) {   // lamp mode
      EEPROM.update(1, buttonPushCounter);
      buttonPushCounter = 105;
      buttonPushCounterDemo = 105;
      pushAndHold = 3;
      TIMSK0 = 1;                // Timer0 on
      ADCSRA &= ~_BV(ADIE);
    }




    else if (buttonPushCounter == 6) { //      OFF
      EEPROM.update(1, buttonPushCounter);
      buttonPushCounter = 106;
      pushAndHold = 4;
      ledCimber = 8;
      indcatorDots = 3;
      TIMSK0 = 1;                // Timer0 on
      ADCSRA &= ~_BV(ADIE);
      for (int led = 0; led < NUM_LEDS; led++) {
        leds[led].setRGB( 0, 0, 0);
      }
      for (int i = 0; i < 8; i++) {
        BinsBinsBins[i] = 0; // sum of all the bins
      }
      FastLED.show();
      FastLED.show();
    }


    // set normal loop modes here:


    if (buttonPushCounter == 100) {                 // falling dot
      musicMode();
    }

    else if (buttonPushCounter == 101) {                // middle out
      musicMode();
      //minMaxThershold = 10;
    }

    else if (buttonPushCounter == 102) {                // ripple
      musicMode();
      //minMaxThershold = NUM_LEDS;
    }

    else if (buttonPushCounter == 103) {                // fade
      musicMode();
    }

    else if (buttonPushCounter == 104) {                // rainbow music
      musicMode();
      //musicVariable3 changes color
    }

    else if (buttonPushCounter == 105) {                // lamp modes
      lampMode();
      delay (2);
      //hueSelect changes color & type of lamp
    }

    else if (buttonPushCounter == 106) {              // Off
    }
  } // end normal modes start settings modes






  else {     // if noraml = 0  //  Settings Modes start here

    if (buttonPushCounter == 7) { //        frequency
      buttonPushCounter = 107;
      pushAndHold = 1;
      indcatorDots = 8;
      //EEPROM.update(1, buttonPushCounter);
    }

    else if (buttonPushCounter == 8) { //        sensitivity
      buttonPushCounter = 108;
      pushAndHold = 1;
      indcatorDots = 8;
      //EEPROM.update(1, buttonPushCounter);
    }

    else if (buttonPushCounter == 9) { //           brightness
      buttonPushCounter = 109;
      pushAndHold = 1;
      indcatorDots = 8;
      //EEPROM.update(1, buttonPushCounter);
    }

    //    else if (buttonPushCounter == 10){  //        demo on switch
    //    EEPROM.update(1, buttonPushCounter);
    //    buttonPushCounter = 110;
    //    pushAndHold = 4;
    //    ledCimber = 8;
    //    indcatorDots = 3;
    //    for(int led = 0; led < NUM_LEDS; led++) {
    //      leds[led].setRGB( 0, 0, 0);
    //    }
    //    FastLED.show();
    //  }
    else if (buttonPushCounter == 10) { //        reset
      buttonPushCounter = 110;
      pushAndHold = 4;
      ledCimber = 8;
      indcatorDots = 3;
      for (int led = 0; led < NUM_LEDS; led++) {
        leds[led].setRGB( 0, 0, 0);
      }
      FastLED.show();
    }
    else if (buttonPushCounter == 11) { //        off
      buttonPushCounter = 111;
      pushAndHold = 4;
      ledCimber = 8;
      indcatorDots = 3;
      for (int led = 0; led < NUM_LEDS; led++) {
        leds[led].setRGB( 0, 0, 0);
      }
      FastLED.show();
      FastLED.show();
      //EEPROM.update(1, buttonPushCounter);
    }


    if (buttonPushCounter == 107) {              // Frequency
      indicators(channel);
    }

    else if (buttonPushCounter == 108) {              // Sensitvity
      indicators(sensitivity);
    }

    else if (buttonPushCounter == 109) {              // Brightness
      indicators(Bvariable);
    }

    else if (buttonPushCounter == 110) {              // reset
      indicators(reset);
    }

    else if (buttonPushCounter == 111) {              // off
      //indicators(off);
    }

    else if (buttonPushCounter == 112) {              // off
      indicatorDemo(20);
    }

  }    //end settings modes here
}     //end loop here






void musicMode() {     // all, channel if all, fallingDot vs Bang & fade, color vs white
  uint8_t  i, x, L, *data, nBins, binNum, weighting, c;
  uint16_t minLvl, maxLvl;
  int      level, y, sum;

  while (ADCSRA & _BV(ADIE)); // Wait for audio sampling to finish

  fft_input(capture, bfly_buff);   // Samples -> complex #s
  samplePos = 0;                   // Reset sample counter
  ADCSRA |= _BV(ADIE);             // Resume sampling interrupt
  fft_execute(bfly_buff);          // Process complex data
  fft_output(bfly_buff, spectrum); // Complex -> spectrum

  // Remove noise and apply EQ levels
  for (x = 0; x < FFT_N / 2; x++) {
    L = pgm_read_byte(&noise[x]);
    spectrum[x] = (spectrum[x] <= L) ? 0 :
                  (((spectrum[x] - L) * (256L - pgm_read_byte(&eq[x]))) >> 8);
  }

  allBins = 0;
  // Downsample spectrum output to 8 columns:
  for (x = 0; x < 8; x++) {


    if (buttonPushCounter != 102) {     // if not ripple
      if (channel != 8) {                // if not all channels selected
        x = channel;   // listen to only one channel specified in the settings
        //maxLvl = 0;
      }
    }




    data   = (uint8_t *)pgm_read_word(&colData[x]);
    nBins  = pgm_read_byte(&data[0]) + 2;
    binNum = pgm_read_byte(&data[1]);
    for (sum = 0, i = 2; i < nBins; i++)
      sum += spectrum[binNum++] * pgm_read_byte(&data[i]); // Weighted
    col[x][colCount] = sum / colDiv[x];                    // Average by divide by number of bins
    minLvl = maxLvl = col[x][0];


    for (i = 1; i < 10; i++) { // Get range of prior 10 frames
      if (col[x][i] < minLvl)      minLvl = col[x][i];
      else if (col[x][i] > maxLvl) maxLvl = col[x][i];
    }

    //    Serial.print("       ");
    //    Serial.print(maxLvl);

    // minLvl and maxLvl indicate the extents of the FFT output, used
    // for vertically scaling the output graph (so it looks interesting
    // regardless of volume level).  If they're too close together though
    // (e.g. at very low volume levels) the graph becomes super coarse
    // and 'jumpy'...so keep some minimum distance between them (this
    // also lets the graph go to zero when no sound is playing):

    if (buttonPushCounter == 102) {
      minMaxThershold = 30;
      if (x == 0) {
        minMaxThershold = 60;
      }
    }
    else {
      minMaxThershold = 8;
      //if (x == 0){ minMaxThershold = 115;  }
    }



    if ((maxLvl - minLvl) < 8) maxLvl = minLvl + 8;  // was 8, was 60, was 115, 10 for headlights mode 115 for all others
    minLvlAvg[x] = unsigned(minLvlAvg[x] * 7 + minLvl) >> 3; // Dampen min/max levels
    maxLvlAvg[x] = unsigned(maxLvlAvg[x] * 7 + maxLvl) >> 3; // (fake rolling average)


    // Second fixed-point scale based on dynamic min/max levels:  //was 10 L = noise, col[x][colCount] = levels of past frames?
    //level = ((sensitivity+1)*10) * L * (col[x][colCount] - minLvlAvg[x]) / (long)(maxLvlAvg[x] - minLvlAvg[x]);
    level = 10 * L * (col[x][colCount] - minLvlAvg[x]) / (long)(maxLvlAvg[x] - minLvlAvg[x]);

    if (level > 115) {
      level = 115;
    }

#if defined(DEVMODE)
    if (level > 1) {
      Serial.println("       ");
      Serial.print(level);
    }
#endif

    /*
      // Clip output and convert to byte:
      if(level < 0L)      c = 0;
      else if(level > NUM_LEDS+2) c = NUM_LEDS+2; // Allow dot to go a couple pixels off top
      else                c = (uint8_t)level;
    */

    //    Serial.print("       ");
    //    Serial.print(c);

    if (buttonPushCounter == 102) {
      rippleV[x] = level;
    }



    //average all channels here:

    else {

      if (channel == 8) {                    //all bins

        //       if (c >= NUM_LEDS){    c = NUM_LEDS;     }
        //        if (c < 0){    c = 0;     }

        //array
        BinsBinsBins[x] = level;


        //allBins = (allBins + c)/8;


      }

      else {                  //single channel break
        allBins = level;
        if (allBins >= 115) {
          allBins = 115;
        }
        if (allBins < 0) {
          allBins = 0;
        }
        break;
      }
    }



  }  //end main forloop here

  if (channel == 8) {
    // allBins = 0;
    for (int i = 0; i < 8; i++) {
      allBins = allBins + BinsBinsBins[i]; // sum of all the bins
    }
    if (allBins > 0) {
      allBins = allBins / 8; // average of all the bins
    }
    //if (allBins >= NUM_LEDS){    allBins = NUM_LEDS;     } //constrain
    if (allBins < 0) {
      allBins = 0;
    }
  }



  soundLevel = allBins * (sensitivity + 1);
  if (soundLevel > 115) {
    soundLevel = 115;
  }



  if (buttonPushCounter == 102) {                                //start ripple mode here

    fadeToBlackBy(leds, NUM_LEDS, 10);
    //fill_solid( leds, NUM_LEDS, CRGB(0,0,0));

    for (int y = 0; y < 8; y++) {
      int bottomOfRipple = ((y * 15) + 6) - (rippleV[y] / 10);
      if (bottomOfRipple <= 0) {
        bottomOfRipple = 0;
      }

      int topOfRipple = ((y * 15) + 6) + (rippleV[y] / 10);
      if (topOfRipple >= NUM_LEDS - 1) {
        bottomOfRipple = NUM_LEDS - 1;
      }

      int rippleBrightness = constrain(rippleV[y] * 3, 0, 255);

      for (int led = bottomOfRipple ; led < topOfRipple; led++) {
        leds[led] = CHSV(0, 0, rippleBrightness);
      }
      blur1d (leds, NUM_LEDS, rippleV[y]);
    }

    FastLED.show();
  } //end ripple mode here

  TIMSK0 = 1;                // Timer0 on
  ADCSRA &= ~_BV(ADIE);


  if (buttonPushCounter == 100 || buttonPushCounter == 101  || buttonPushCounter == 104) {       // falling dot, rainbow & ripple

    if (soundLevel > peakAll) peakAll = soundLevel; // Keep dot on top
    if (peakAll > NUM_LEDS) peakAll = NUM_LEDS; // Keep dot on top

    // Color LEDs here: variables   c = bin level   y = dot
    if (buttonPushCounter == 100) {
      for (int led = 0; led < soundLevel; led++) {
        leds[led].setRGB( 80, 80, 80);
      }
    }


    else if (buttonPushCounter == 101) {                                                            // fill middle out

      for (int led = (NUM_LEDS - soundLevel) / 2; led < (soundLevel / 2) + NUM_LEDS / 2; led++) {
        leds[led].setRGB( 50, 50, 50);
      }
    }




    else if (buttonPushCounter == 104) {                                                             // rainbow color mode here
      fill_rainbow( leds, soundLevel, (musicVariable3 * 30), 1);
    }



    if (peakAll <= 0) {       // Empty column

      if (buttonPushCounter == 100) {
        fadeToBlackBy( leds, NUM_LEDS, 30);   // was 30
      }
      if (buttonPushCounter == 101) {
        fadeToBlackBy( leds, NUM_LEDS, 30);
        leds[NUM_LEDS / 2].setRGB( 80, 80, 80);
      }
      if (buttonPushCounter == 104) {
        fadeToBlackBy( leds, NUM_LEDS, 50);    //
      }
    }

    else if (soundLevel < NUM_LEDS) { // Partial column?

      if (buttonPushCounter == 100) {               // dot
        fadeToBlackBy( leds, NUM_LEDS, 30); //was 30
        // for(int led = soundLevel; led < NUM_LEDS; led++) {
        //    leds[led].setRGB( 0, 0, 0);
        //   }

      }

      if (buttonPushCounter == 104) {               // rainbow music
        fadeToBlackBy( leds, NUM_LEDS, 50);
        //        for(int led = soundLevel; led < NUM_LEDS; led++) {
        //          leds[led].setRGB( 0, 0, 0);
        //        }
      }


      else if (buttonPushCounter == 101) {                                      //middle out
        for (int led = 0; led < (NUM_LEDS - soundLevel) / 2; led++) {
          leds[led].setRGB( 0, 0, 0);
        }
        for (int led = (soundLevel / 2) + NUM_LEDS / 2; led < NUM_LEDS; led++) {
          leds[led].setRGB( 0, 0, 0);
        }
        leds[NUM_LEDS / 2].setRGB( 80, 80, 80);
      }



    }

    // The 'peak' dot
    y = peakAll;


    if (buttonPushCounter == 100) {                      //add dot in falling dot mode
      leds[y].setRGB( 0, 0, 255);
      for (int led = y + 1; led < NUM_LEDS; led++) {
        leds[led].setRGB( 0, 0, 0);
      }
    }


    FastLED.show();

    //if(++dotCount >= 1) {    //was 3
    //  dotCount = 0;
    if (peakAll > 0) {
      peakAll--;
    }
    //}


  } //end falling dot

  else if (buttonPushCounter == 103) { // start fade


    if (soundLevel > peakAll) peakAll = soundLevel; // Keep value on top

    for (int led = 0; led < NUM_LEDS; led++) {
      fill_solid( leds, NUM_LEDS, CRGB(peakAll, peakAll, peakAll));
    }

    FastLED.show();

    if (peakAll >= 35) {
      peakAll = peakAll - 4;
    }
    //    if (peakAll < 4){
    //    peakAll = 0;
    //    }
    //    if (peakAll < 0){
    //    peakAll = 0;
    //    }


  }       //end bang & fade



  TIMSK0 = 0;                // Timer0 off

  if (++colCount >= 10) colCount = 0;
}


















ISR(ADC_vect) { // Audio-sampling interrupt
  static const int16_t noiseThreshold = 4;
  int16_t              sample         = ADC; // 0-1023

  capture[samplePos] =
    ((sample > (512 - noiseThreshold)) &&
      (sample < (512 + noiseThreshold))) ? 0 :
    sample - 512; // Sign-convert for FFT; -512 to +511

  if (++samplePos >= FFT_N) ADCSRA &= ~_BV(ADIE); // Buffer full, interrupt off
}






void lampMode() {

  if (hueSelect == 8) {                                              //solid color change
    rainbow(0, NUM_LEDS, 0.1);
  }

  //    else if (hueSelect < 8) {                                         //single color
  //          fill_solid( leds, NUM_LEDS, CHSV(hueSelect*30,255,200));
  //          FastLED.show();
  //   }

  else if (hueSelect == 9) {                                         //white


    for (int led = 0; led < NUM_LEDS; led++) {
      leds[led].setRGB( 150, 150, 150);
    }
    FastLED.show();

    delay (100);
  }


  else if (hueSelect == 10) {                                         //rainbow
    rainbow(0, NUM_LEDS, 1);
  }

  else if (hueSelect == 11) {                                         //fire
    random16_add_entropy( random());
    Fire2012();
    FastLED.show(); // display this frame
    FastLED.delay(1000 / 60);
  }

  else if (hueSelect > 11) {
    hueSelect = 8;
  }
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
  capture[NUM_LEDS - 15];



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





void indicatorDemo(int loops) {  // mini automated display of each mode.
  TIMSK0 = 1;                // Timer0 on
  ADCSRA &= ~_BV(ADIE);
  //if (buttonPushCounterDemo == 103){  loops = loops/2; }               //fade

  for (int i = 0; i < loops; i++) {


    indicatorModes();


    buttonState = digitalRead(buttonPin);
    if  (buttonState == HIGH) {
      buttonChecker();
      break;

    }


    //delay(40);
  }
}




void indicatorModes() {

  //fadeToBlackBy( leds, NUM_LEDS, 255);
  automatedIndicator = random(0, NUM_LEDS * 0.7); //was 80
  randomFade = random(100);
  prevRandomFade = randomFade;

  if (automatedIndicator <= 0) {       // Empty column

    if (buttonPushCounterDemo == 100) {
      fadeToBlackBy( leds, NUM_LEDS, 30);
    }
    if (buttonPushCounterDemo == 101) {
      fadeToBlackBy( leds, NUM_LEDS, 30);
      leds[NUM_LEDS / 2].setRGB( 50, 50, 50);
    }
    if (buttonPushCounterDemo == 104) {
      fadeToBlackBy( leds, NUM_LEDS, 50);
    }
  }

  else if (automatedIndicator < NUM_LEDS) { // Partial column?

    if (buttonPushCounterDemo == 100) {               // dot
      fadeToBlackBy( leds, NUM_LEDS, 50);

    }

    if (buttonPushCounterDemo == 104) {               // rainbow music
      fadeToBlackBy( leds, NUM_LEDS, 30);
      //        for(int led = soundLevel; led < NUM_LEDS; led++) {
      //          leds[led].setRGB( 0, 0, 0);
      //        }
    }


    else if (buttonPushCounterDemo == 101) {                                      //middle out
      for (int led = 0; led < (NUM_LEDS - automatedIndicator) / 2; led++) {
        leds[led].setRGB( 0, 0, 0);
      }
      for (int led = (automatedIndicator / 2) + NUM_LEDS / 2; led < NUM_LEDS; led++) {
        leds[led].setRGB( 0, 0, 0);
      }
      leds[NUM_LEDS / 2].setRGB( 50, 50, 50);
    }
  }



  if (autoPeak < automatedIndicator) {
    autoPeak = automatedIndicator + 1;
  }
  else {
    autoPeak = autoPeak - 2;
  }




  if (buttonPushCounterDemo == 100) {                 //dot
    for (int led = 0; led < automatedIndicator; led++) {
      leds[led].setRGB( 50, 50, 50);
    }
    leds[autoPeak].setRGB( 0, 0, 255);
    for (int led = autoPeak + 1; led < NUM_LEDS; led++) {
      leds[led].setRGB( 0, 0, 0);
    }
    delay(50);
  }

  else if (buttonPushCounterDemo == 101) {                 // middle out
    automatedIndicator = automatedIndicator / 2 ;

    for (int led = NUM_LEDS / 2; led < NUM_LEDS / 2 + automatedIndicator; led++) {
      leds[led].setRGB( 50, 50, 50);
    }

    for (int led = NUM_LEDS / 2; led > NUM_LEDS / 2 - automatedIndicator; led--) {
      leds[led].setRGB( 50, 50, 50);
    }
    delay(50);
  }

  if (buttonPushCounterDemo == 102) {                 //ripple                                       //


    fadeToBlackBy(leds, NUM_LEDS, 10);

    for (int y = random(8); y < 8; y = y + random(1, 8)) {
      rippleV[y] = random(NUM_LEDS / 2); // TODO need to check the random number range here was 50

      for (int led = ((y * 18) + 6) - (rippleV[y] / 15) ; led < ((y * 18) + 6) + (rippleV[y] / 15); led++) {
        leds[led].setRGB( rippleV[y], rippleV[y], rippleV[y]);
      }
      blur1d (leds, NUM_LEDS, rippleV[y]);
      delay (30);
    }
    //delay(10);
  }



  if (buttonPushCounterDemo == 103) {                //fade
    if (prevRandomFade >= randomFade && prevRandomFade >= 35) {
      prevRandomFade--;
    }
    else {
      prevRandomFade = randomFade;
    }
    for (int led = 0; led < NUM_LEDS; led++) {
      leds[led].setRGB( prevRandomFade, prevRandomFade, prevRandomFade);
    }
    delay(80);

  }

  if (buttonPushCounterDemo == 104) {                //rainbow
    fadeToBlackBy( leds, NUM_LEDS, 30);
    fill_rainbow( leds, automatedIndicator, (musicVariable3 * 30), 1);
    //      for(int led = automatedIndicator; led < NUM_LEDS; led++) {
    //          leds[led].setRGB( 0, 0, 0);
    //        }


    delay(50);
  }

  FastLED.show();
}








void buttonChecker() {                                                    //
  buttonState = digitalRead(buttonPin);


  if (buttonState == HIGH) {  // button is on:

    ADCSRA &= ~_BV(ADIE);      // stop interrupt
    TIMSK0 = 1;                // Timer0 on

    while ((buttonState == HIGH) && (++timeCounter < 100)) {             // while for Tap, freeze LEDs as they were
      buttonCounter = 0;
      //lockCounter = 0;
      for (int i = 0; i < threashold; i++) {
        buttonState = digitalRead(buttonPin);
        if  (buttonState == HIGH) {
          buttonCounter++;
        }
      }
      if (buttonCounter > threashold * 0.8) {
        buttonState = HIGH;
      }
      else {
        buttonState = LOW;
      }

    }
    timeCounter = 0;

    if (buttonState == LOW) {
      //Serial.println("mode switch");
      //next button mode
      firstPressHold = 1;

      if (normal == 1) {
        if ((buttonPushCounter > 99) && (singlePress = 1)) {
          buttonPushCounter = buttonPushCounter - 99;             //got to next mode
        }
        else {
          buttonPushCounter ++;
        }
        if (buttonPushCounter == numberOfModes) {    //limit counter to number of normal modes
          buttonPushCounter = 0;
        }
      }

      else if (normal == 0) {      //  if (normal == 0)  settings modes

        if ((buttonPushCounter > 99) && (singlePress = 1)) {
          buttonPushCounter = buttonPushCounter - 99;             //got to next mode
        }
        else {
          buttonPushCounter ++;
        }
        if (buttonPushCounter == 13) {
          buttonPushCounter = 7;    // loop in settings modes
        }

      }
    }

    else {  // button is still high after 1 second

      firstPressHold = 0;

      //initilize push and hold mode
      //every n seconds add one to value
      fetchValue();
      ledCimber = 8;

      if (pushAndHold == 1) { //settings
        indicators(variableCounter);     // TODO check if this needs adding back in
      }


      while (buttonState == HIGH) {                         // while holding
        buttonCounter = 0;
        for (int i = 0; i < threashold; i++) {
          buttonState = digitalRead(buttonPin);
          if  (buttonState == HIGH) {
            buttonCounter++;
          }
        }
        if (buttonCounter > threashold * 0.8) {
          buttonState = HIGH;
        }
        else {
          buttonState = LOW;
        }

        if  (pushAndHold == 4) {

          // timmer4();
          ledCimber++;

          if (ledCimber >= (NUM_LEDS - 5)) {
            ledCimber = ((NUM_LEDS / 8) * 7) + 8;
          }

          //delay(4);

          indicators(ledCimber);

        }

        else {

          EVERY_N_MILLISECONDS( 1000 ) {    // TODO check if this needs to go up to 1000
            //fetchValue();

            if      (pushAndHold == 0) {        // no push and holds modes = M1 M2 M3 M4      = lock change only

              indicatorModes();   // TODO may need for loop
              //lockUnlock();

            }


            else if (pushAndHold == 1) {        // settings & off modes                       = variable change only


              variableCounter++;
              if (variableCounter >= 9) {
                variableCounter = 0;
              }

            }


            else if (pushAndHold == 2) {       // rainbow = M5              = variable + lock

              variableCounter++;

              if (variableCounter > 8) {
                variableCounter = 0;
              }
              if (variableCounter < 0) {
                variableCounter = 0;
              }
              assignValue();
            }

            else if (pushAndHold == 3) {       // lamp modes = L1               = variable + lock

              variableCounter++;

              if (variableCounter > 11) {   //
                variableCounter = 8;
              }
              if (variableCounter < 8) {
                variableCounter = 8;
              }
              assignValue();
            } // end pushAndHold = 3


            //indicators(variableCounter);     //  change indicator
          }  //end EVERY_N_MILLISECONDS
        }
        if (pushAndHold == 1) {
          indicators(variableCounter);     //  change indicator
        }

        else if (pushAndHold ==  2 || pushAndHold == 0) {       //
          indicatorModes();
        }

        else if (pushAndHold == 3) {
          lampMode();
        }

        //ledCimber++;

      }  //end while holding


      if (pushAndHold == 1 || pushAndHold == 4) { //settings
        //variableCounter--;
        assignValue();



      } //end settings pushAndHold

    }  //end button being held
  }  //end button inital press
} //button checking








void assignValue() {                                                  //assign

  if      (buttonPushCounter == 100) {  // falling dot
  }

  else if (buttonPushCounter == 101) {  // middle out
  }

  else if (buttonPushCounter == 102) {  // ripple
  }

  else if (buttonPushCounter == 103) {  //fade
  }

  else if (buttonPushCounter == 104) {  // rainbow music
    musicVariable3 = variableCounter;
    EEPROM.update(3, musicVariable3);
  }

  else if (buttonPushCounter == 105) {   //lamp
    hueSelect = variableCounter;
    //      if (singleHues == 0){
    //        if (hueSelect < 8){
    //          hueSelect = 8;
    //        }
    //      }
    EEPROM.update(4, hueSelect);
  }


  else if (buttonPushCounter == 106) {   // off
    if (ledCimber >= ((NUM_LEDS / 8) * 7) + 7) {
      normal = 0;
      FastLED.setBrightness(255);
      buttonPushCounter = 7;
    }
    else {
      ledCimber = 8;
      fill_solid( leds, NUM_LEDS, CRGB(0, 0, 0));
      FastLED.show();
      FastLED.show();
    }
  }

  else if (buttonPushCounter == 107) {     // frequency
    //indicators(variableCounter);     //  change indicator
    channel = variableCounter;
    //COOLING   =  COOLINGarray[channel];
    EEPROM.update(2, channel);
  }
  else if (buttonPushCounter == 108) {     // sensitivity
    sensitivity = variableCounter;
    //if (sensitivity == 0) sensitivity = 1;
    EEPROM.update(5, sensitivity);
  }
  else if (buttonPushCounter == 109) {     //brightness
    Bvariable = variableCounter;
    EEPROM.update(6, Bvariable);
  }

  else if (buttonPushCounter == 110) {     // reset
    if (ledCimber >= ((NUM_LEDS / 8) * 7) + 7) {

      fill_solid( leds, NUM_LEDS, CHSV(255, 255, 200));
      FastLED.show();
      delay(200);
      fadeToBlackBy( leds, NUM_LEDS, 255);
      FastLED.show();
      fill_solid( leds, NUM_LEDS, CHSV(255, 255, 200));
      FastLED.show();
      delay(200);
      fadeToBlackBy( leds, NUM_LEDS, 255);
      FastLED.show();
      newEpprom = 255;
      EEPROM.update(0, newEpprom);
      eepromSet();
    }
    else {
      ledCimber = 8;
      fill_solid( leds, NUM_LEDS, CRGB(0, 0, 0));
    }
  }
  else if (buttonPushCounter == 111) {    // off
    if (ledCimber >= ((NUM_LEDS / 8) * 7) + 7) {
      normal = 1;
      FastLED.setBrightness(((Bvariable * Bvariable) * 3) + 20);
      buttonPushCounter = 0;

    }
    else {
      ledCimber = 8;
      fill_solid( leds, NUM_LEDS, CRGB(0, 0, 0));
      FastLED.show();
      FastLED.show();
    }
  }
}







void fetchValue() {                                                 //fetch only for push and hold variables

  if      (buttonPushCounter == 100) {  // falling dot
  }
  else if (buttonPushCounter == 101) {  // middle out
  }
  else if (buttonPushCounter == 102) {  // ripple
  }
  else if (buttonPushCounter == 103) {  // fade
  }
  else if (buttonPushCounter == 104) {  // rainbow
    variableCounter = musicVariable3;
  }
  else if (buttonPushCounter == 105) {  // lamp
    variableCounter = hueSelect;
  }
  else if (buttonPushCounter == 106) {  // off
  }


  else if (buttonPushCounter == 107) {  // frequncy
    variableCounter = channel;
  }
  else if (buttonPushCounter == 108) {  // sensitivity
    variableCounter = sensitivity;
  }
  else if (buttonPushCounter == 109) {  // brightness
    variableCounter = Bvariable;
  }
  //  else if (buttonPushCounter == 110){   //demo
  //    demoAuto = 9;
  //  }
  else if (buttonPushCounter == 110) {  //reset
  }
  else if (buttonPushCounter == 111) {  // off
  }
}






void indicators(int variableSet) {

  if      (buttonPushCounter == 106) {
    modeColor = 210;
  }
  else if (buttonPushCounter == 107) {
    modeColor =  10;  //green
  }
  else if (buttonPushCounter == 108) {
    modeColor =  100; //pink
  }
  else if (buttonPushCounter == 109) {
    modeColor = 160;  // blue
  }
  //else if (buttonPushCounter == 110) { modeColor =  60; }  // yellow
  else if (buttonPushCounter == 110) {
    modeColor = 95;   // red
  }
  else if (buttonPushCounter == 111) {
    modeColor = 210;
  }


  if (indcatorDots == 8) {



    if (dotBrightnessDirection == 1)  dotBrightness++;
    else if (dotBrightnessDirection == 0)  dotBrightness--;


    if (dotBrightness >= 255) dotBrightnessDirection = 0;
    else if (dotBrightness <= 0) dotBrightnessDirection = 1;


    fill_solid( leds, NUM_LEDS, CRGB(0, 0, 0));

    for (int led = 8; led < NUM_LEDS; led = led + ((NUM_LEDS) / 8)) {                                  // all dots dim
      leds[led] = CHSV(modeColor, 255, 100);
    }

    if (variableSet > 7 && buttonState == HIGH) {
      for (int led = 8; led < NUM_LEDS; led = led + ((NUM_LEDS) / 8)) {                                // all dots on
        leds[led] = CHSV(modeColor, 255, 255);
      }
    }
    else if (variableSet > 7 && buttonState == LOW) {
      for (int led = 8; led < NUM_LEDS; led = led + ((NUM_LEDS) / 8)) {                                // all dots on
        leds[led] = CHSV(modeColor, 255, dotBrightness);
      }
    }
    else if (variableSet <= 7 && buttonState == HIGH) {                                                 //dot                                                        // indicator dot
      leds[((NUM_LEDS / 8)*variableSet) + 8] = CHSV(modeColor, 255, 255);
    }
    else if (variableSet <= 7 && buttonState == LOW) {                                                    //dot                                                     // indicator dot
      leds[((NUM_LEDS / 8)*variableSet) + 8] = CHSV(modeColor, 255, dotBrightness);
    }
    FastLED.show();
  }


  else if (indcatorDots == 3) {

    if (ledCimber <= 8) {
      fill_solid( leds, NUM_LEDS, CRGB(0, 0, 0));
      leds[8] = CHSV(modeColor, 255, 150);
    }


    leds[ledCimber] = CHSV(modeColor, 255, 150);
    FastLED.show();
  }
}




void eepromSet() {

  //newEpprom = 255;          // first run eprom data save
  newEpprom = EEPROM.read(0);          // first run eprom data save
  if (newEpprom == 255) {

    // eeprom values:
    newEpprom = 1;

    buttonPushCounter = 0;                  //
    channel = 8;                            //
    //locked = 0;                             //
    musicVariable3 = 0;                     //
    hueSelect = 8;                          //
    //demo = 0;                               //
    //demoAuto = 9;                           //
    sensitivity = 4;                        //  3 for normal saber, 1 for half saber
    Bvariable = 4;                          //

    //other values:
    COOLING   =  COOLINGarray[channel];
    normal = 1;
    FastLED.setBrightness(((Bvariable * Bvariable) * 3) + 20); // set master brightness control

    EEPROM.update(0, newEpprom);
    EEPROM.update(1, buttonPushCounter);
    EEPROM.update(2, channel);
    //EEPROM.update(3, locked );
    EEPROM.update(3, musicVariable3);
    EEPROM.update(4, hueSelect);
    //EEPROM.update(6, demo);
    //EEPROM.update(7, demoAuto);
    EEPROM.update(5, sensitivity);
    EEPROM.update(6, Bvariable);
  }

  else {                                                    //not new eeprom
    buttonPushCounter =  (int)EEPROM.read(1);
    channel =            (int)EEPROM.read(2);
    //locked =             (int)EEPROM.read(3);
    musicVariable3 =     (int)EEPROM.read(3);
    hueSelect =          (int)EEPROM.read(4);
    //demo  =              (int)EEPROM.read(6);
    //demoAuto =           (int)EEPROM.read(7);
    sensitivity =        (int)EEPROM.read(5);
    Bvariable =          (int)EEPROM.read(6);

    //COOLING   =  COOLINGarray[channel];
    FastLED.setBrightness(((Bvariable * Bvariable) * 3) + 20); // set master brightness control
    if (buttonPushCounter == 10 || buttonPushCounter == 110) {
      normal = 0;
    }
    else {
      normal = 1;
    }
  }
}

