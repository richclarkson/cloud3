
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
     fill_rainbow
     fill_gradient
     blur1d
     setRGB
     CHSV
   Internal (writen in here):
     rainbow
     Fire2012
*/

#include <Arduino.h>
#include <FastLED.h>
#include <Audio.h>     // v1.0.5

FASTLED_USING_NAMESPACE

//Sound Variables
int soundLevel;          // this is the output of the RMS after being EQ
int prevrmsReading = 1;   // store the previous RMS reading
int dot = 100;           // this is a slowly falling value based on the peaks of soundLevel, used by musicMode1 and musicMode4
int dotCount = 1;
int lampMode2Count = 1;
int fftArray[8];    // array of FFT values for each channel used in musicMode3
int RainbowVariable = 2; // start position of Rainbow color wheel used in musicMode5



// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=139,91
AudioAnalyzeRMS          rms1;           //xy=411,381
AudioConnection          patchCord1(i2s1, 0, rms1, 0);
// GUItool: end automatically generated code


//Lamp Mode Variables
int rainbowCounter = 0;
int timeSpeed = 100;
uint8_t gHue = 0;           // rotating "base color" used by many of the patterns
int COOLINGarray[] = {100, 90, 85, 80, 75, 90, 85, 100, 90}; // Fire Mode varriable
int COOLING = 90;         // Fire Mode varriable
#define SPARKING 100      // Fire Mode varriable
//static byte heat[100];
//int capture[100];


//LED Variables
#define DATA_PIN 2 //MOSI  //7 Green
#define CLK_PIN 3  //SCK  //14 Blue
#define LED_TYPE APA102
#define COLOR_ORDER BGR
#define NUM_LEDS 115 //115 for normal Saber, 48 for half saber
CRGB leds[NUM_LEDS];
#define FRAMES_PER_SECOND 60


void setup()
{
  AudioMemory(12);     // Audio requires memory to work.
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(90);

    turnoffLEDs();  // turns off all LEDs
    FastLED.show();
    delay(1000);  // Sanity Delay
}



void loop()         //uncomment specific mode to test it
{ 
  
  RMS();
  if (soundLevel != prevrmsReading) {
    Serial.print(soundLevel);
    Serial.print("     ");
    Serial.println(NUM_LEDS);    // useful when stopping the serial plotter from autoscalling
    prevrmsReading = soundLevel;
  }
  
  musicmode1();     // great!
  
  // musicmode2();    // great!
   
  // musicmode3();    // untested (needs FFT input to test)
  
   //musicmode4();    // great!
  
  // musicmode5();    // great!


                                           // Lamp Modes
   // lampmode1();    // great!
    
//    if (lampMode2Count == 1){    // lampmode2() should only be run once, great!
//       lampmode2(); 
//       lampMode2Count = 0;
//    }

   // lampmode3();    // great!
   
   // lampmode4();     // great!

  
  }



void musicmode1()   // Falling Dot
{ 

  if (soundLevel > dot)
    dot = soundLevel; // Keep dot on top of soundLevel
  if (dot > NUM_LEDS)
    dot = NUM_LEDS; // Keep dot from going out of frame

  turnoffLEDs();



  for (int led = 0; led < soundLevel; led++)
  { // Start by Filling LEDS up to the soundLevel with dim white
    leds[led].setRGB(80, 80, 80);
  }
  
  // Fill in the 'peak' pixel with BLUE
  leds[dot].setRGB(0, 0, 255);
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

  // NO SOUND
  if (soundLevel <= 0)
  {                                    // If no sound (dot = 0)
    turnoffLEDs();
    leds[NUM_LEDS / 2].setRGB(80, 80, 80); // keep center dot illuminated
  }
  FastLED.show(); // send data to LEDs to display
}





void musicmode3()    // Ripple
{ 
   turnoffLEDs();

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
      bottomOfRipple = NUM_LEDS - 1;
    }

    int rippleBrightness = constrain(fftArray[y] * 3, 0, 255);

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
  
  if (soundLevel > dot){
    dot = soundLevel;
    } // Keep dot on top of soundLevel
  if (dot > NUM_LEDS){
    dot = NUM_LEDS-1;
  }// Keep dot from going out of frame
      

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

  // NO SOUND
  if (soundLevel <= 0)
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


  void RMS() {
  if (rms1.available()) {

    //rms1.read() gives a Float value between 0.0 and 1.0
    //Scale this by x2000 to roughly fit the 115 leds
    soundLevel = rms1.read() * 3000;     

    if (soundLevel > NUM_LEDS) {
      soundLevel = NUM_LEDS;              // upper limit
    }
    if (soundLevel < 0) {
      soundLevel = 0;                     // lower limit
    }
  }
}

  


