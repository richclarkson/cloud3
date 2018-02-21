
/*
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

    TODO:
    Fix sometimes freezeing in lamp mode to off mode?

*/

#include <Arduino.h>
#include <FastLED.h>
#include <Audio.h>   
#include <EEPROM.h>
#include "TapPressButton.h"
#include <CircularBuffer.h>

FASTLED_USING_NAMESPACE
TapPressButton capSensor;

//Global Varriables
uint8_t buttonPushCounter;
int channel;
int sensitivity;  // 0-8 where 8 = maximum sensitivity
int Bvariable;  // brightness
int hueSelect;

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

static const int preview[] = {
  10, 4, 10, 12, 50, 60, 25, 3, 2, 5, 5, 100, 60, 10, 5, 6, 7, 12, 5, 6, 9, 8
};
int previewCounter = 0;


//Lamp Mode Variables
int rainbowCounter = 0;
int timeSpeed = 100;
uint8_t gHue = 180;           // rotating "base color" used by many of the patterns
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
//#define FRAMES_PER_SECOND 120

//TAP HOLD Varriables 
//uint8_t buttonState;
CircularBuffer<int, 40> buffer;

const int capPin = 19;
const int touchTime = 1000;
unsigned long loopTime;
bool isTouch;
int holding = 0;
int modeColor;
int indcatorDots;
uint8_t ledCimber = 8;
int variableCounter = 9;  //global
uint8_t normal = 1;  // 1 = normal modes, 0 = settings modes
uint8_t pushAndHold = 0;
uint8_t buttonPushCounterDemo;
uint8_t dotBrightness = 250;
uint8_t dotBrightnessDirection = 1;
int newEpprom;
uint8_t automatedIndicator;
uint8_t reset = 0;
int numberLoops = 200;
int capReading;
int aveCapReading;

// Prototype Functions:
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
void buttonChecker();
void assignValue();
void fetchValue(); 
void indicators(int variableSet);
void eepromSet();
void indicatorModes();
void lampMode(); 
void fetchSoundData();
void indicatorDemo(int loops);
void tap();
void press();
void prepareModes();
void runMode();
void readSensor();



void setup()
{ 
  AudioMemory(12);     // Audio requires memory to work.
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLK_PIN, COLOR_ORDER, DATA_RATE_MHZ(2)>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(90);
  turnoffLEDs();
  FastLED.show();
  Serial.begin(9600);
  delay(1000);  // Sanity Delay
  Serial.println("Saber v1.2");
  eepromSet();
  loopTime = 0;
  capSensor = TapPressButton(50, 300, 1000, 1000);
  isTouch = false;
  //pinMode(capPin, INPUT);
}


void loop()
{ 
  readSensor();

  isTouch = aveCapReading > touchTime;
  loopTime = millis();
    //Serial.println(touchRead(capPin));     // use for callibration
  capSensor.update(isTouch, loopTime);
  if (capSensor.isTap()) {
    Serial.println("tap");
    tap();
  }
  if (capSensor.isPress()) {
    Serial.println("press");
    press();
  }
  if (buttonPushCounter < 90){
    prepareModes();               // load in startup values for each mode
  }
  runMode();                      // run the loop using selected mode
  
  // soundLevel = map(aveCapReading,0,3000,0,115);
  // soundLevel = constrain(soundLevel,0,115);
  // musicmode1();
}   






//*******************************       Music Modes    ******************************************//

void fetchSoundData()    // fetch readings
{
  if (channel < 8){
    analyzeFFT();
  }
  else if (channel == 8){
    analyzeLevel();
  }
}

void analyzeLevel() 
{
  if (rms1.available()) {
    soundLevel = (rms1.read() - 0.0006) * scale[sensitivity];     // remove noise and scale
    soundLevel = constrain(soundLevel, 0, NUM_LEDS);              // limit
  }
}

void analyzeFFT() 
{
  if (fft1024.available()) {
    FFTreading(channel);
    soundLevel = fftArray[channel];
  }
}

void analyzeFFTall() 
{
  if (fft1024.available()) {
    for (int i = 0; i < 8; i++) {
      FFTreading(i);
    }
  }
}

void FFTreading(int FFTchannel) 
{
  reading[FFTchannel] =  fft1024.read(binStart[FFTchannel], binEnd[FFTchannel]);
  reading[FFTchannel] = reading[FFTchannel] - (noise[FFTchannel] * 0.0001);           // remove noise
  fftArray[FFTchannel] = (reading[FFTchannel] * scale[sensitivity]) * eq[FFTchannel]; // scale
  fftArray[FFTchannel] = constrain(fftArray[FFTchannel], 0, NUM_LEDS);                // limit
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

  if (++dotCount >= 20) {                   // make the dot fall slowly
    dotCount = 0;
    if (dot > 1) {
      dot--;
    }
  }
}

void musicmode2()   // Middle Out 
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
    if (dot > 20) {
      dot--;
    }
  }
}

void musicmode5()     // Rainbow 
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

//*******************************      Lamp Modes    ******************************************//

void turnoffLEDs()
{
    for (int led = 0; led < NUM_LEDS; led++)        
  { //turn off LEDs
    leds[led] = CHSV( 100, 0, 0);
  }
}

void lampMode()      // collection of all the lamp modes
{
  if (hueSelect == 8) {                                              //solid color change
    lampmode1();
  }
  else if (hueSelect == 9) {                                         //white
    lampmode2();
  }
  else if (hueSelect == 10) {                                         //rainbow
    lampmode3();
  }
  else if (hueSelect == 11) {                                         //fire
    lampmode4();
  }
  else {
    hueSelect = 8;
  }
}

void lampmode1()  // Neon
{
  rainbow(0, NUM_LEDS, 0.1);
  //rainbow(0, NUM_LEDS, 0.1);
  FastLED.show();
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
  //rainbow(0, NUM_LEDS, 1);
  FastLED.show();
}

void lampmode4()  // Fire
{
  random16_add_entropy( random());
  if (++dotCount >= 60) {                   // make the dot fall slowly
    dotCount = 0;
    Fire2012();
    FastLED.show(); // display this frame
    //FastLED.delay(1000 / 60);
  }
}

void rainbow(int startPos, int number, float deltaHue) 
{
    if (++rainbowCounter >= timeSpeed) {
      gHue++;
      rainbowCounter = 0;
    } // slowly cycle the "base color" through the rainbow
    fill_rainbow( &(leds[startPos]), number, gHue, deltaHue);
    //FastLED.show();
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

//*******************************      State Modes    ******************************************//


void assignValue()                                                   //assign
{
  if      (buttonPushCounter == 100) {  // falling dot
  }

  else if (buttonPushCounter == 101) {  // middle out
  }

  else if (buttonPushCounter == 102) {  // ripple
  }

  else if (buttonPushCounter == 103) {  //fade
  }

  else if (buttonPushCounter == 104) {  // rainbow music
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
    if (ledCimber >= 107) {
      normal = 0;
      buttonPushCounter = 7;
    }
  }

  else if (buttonPushCounter == 107) {     // frequency
    channel = variableCounter;
    Serial.print("channel = ");
    Serial.println(channel);
    EEPROM.update(2, channel);
  }
  else if (buttonPushCounter == 108) {     // sensitivity
    sensitivity = variableCounter;
    Serial.print("sensitivity = ");
    Serial.println(sensitivity);
    EEPROM.update(5, sensitivity);
  }
  else if (buttonPushCounter == 109) {     //brightness
    Bvariable = variableCounter;
    Serial.print("Bvariable = ");
    Serial.println(Bvariable);
    FastLED.setBrightness(((Bvariable * Bvariable) * 3) + 20);
    EEPROM.update(3, Bvariable);
  }

  else if (buttonPushCounter == 110) {     // reset
    if (ledCimber >= 107) {
      Serial.print("RESET!");
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
      newEpprom = 1;
      EEPROM.update(0, newEpprom);
      eepromSet();
    }
    else {
      //ledCimber = 8;
      fill_solid( leds, NUM_LEDS, CRGB(0, 0, 0));
      leds[8] = CHSV(modeColor, 255, 150);
      FastLED.show();
    }
  }
  else if (buttonPushCounter == 111) {    // off
    if (ledCimber >= 107) {
      normal = 1;
      buttonPushCounter = 0;

    }
    else {
      ledCimber = 8;
      fill_solid( leds, NUM_LEDS, CRGB(0, 0, 0));
      FastLED.show();
    }
  }
}

void fetchValue()                                                  //fetch only for push and hold variables
{
  if      (buttonPushCounter == 100) {  // falling dot
  }
  else if (buttonPushCounter == 101) {  // middle out
  }
  else if (buttonPushCounter == 102) {  // ripple
  }
  else if (buttonPushCounter == 103) {  // fade
  }
  else if (buttonPushCounter == 104) {  // rainbow
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
  else if (buttonPushCounter == 110) {  //reset
  }
  else if (buttonPushCounter == 111) {  // off
  }
}

void indicators(int variableSet) 
{
  if (indcatorDots == 8) {
    if      (buttonPushCounter == 107) {  modeColor =  100; }  // green
    else if (buttonPushCounter == 108) {  modeColor =  230; }  // pink
    else if (buttonPushCounter == 109) {  modeColor = 160;  }  // blue

    if (dotBrightnessDirection == 1)  dotBrightness++;
    else if (dotBrightnessDirection == 0)  dotBrightness--;

    if (dotBrightness >= 255) dotBrightnessDirection = 0;
    else if (dotBrightness <= 0) dotBrightnessDirection = 1;

    fill_solid( leds, NUM_LEDS, CRGB(0, 0, 0));

    for (int led = 8; led < NUM_LEDS; led = led + ((NUM_LEDS) / 8)) {                                  // all dots dim
      leds[led] = CHSV(modeColor, 255, 100);
    }

    if (variableSet > 7 && isTouch == HIGH) {
      for (int led = 8; led < NUM_LEDS; led = led + ((NUM_LEDS) / 8)) {                                // all dots on
        leds[led] = CHSV(modeColor, 255, 255);
      }
      //Serial.println("a");
    }
    else if (variableSet > 7 && isTouch == LOW) {
      for (int led = 8; led < NUM_LEDS; led = led + ((NUM_LEDS) / 8)) {                                // all dots on
        leds[led] = CHSV(modeColor, 255, dotBrightness);
      }
      //Serial.println("b");
    }
    else if (variableSet <= 7 && isTouch == HIGH) {                                                 //dot                                                        // indicator dot
      leds[((NUM_LEDS / 8)*variableSet) + 8] = CHSV(modeColor, 255, 255);
      //Serial.println("c");
    }
    else if (variableSet <= 7 && isTouch == LOW) {                                                    //dot                                                     // indicator dot
      leds[((NUM_LEDS / 8)*variableSet) + 8] = CHSV(modeColor, 255, dotBrightness);
      //Serial.println("d");
    }
    FastLED.show();
  }

  else if (indcatorDots == 3) {
    if      (buttonPushCounter == 106) {  modeColor = 120;  } // light blue
    else if (buttonPushCounter == 110) {  modeColor = 10;   } // red
    else if (buttonPushCounter == 111) {  modeColor = 120;  } // light blue

    if (ledCimber <= 8) {
      fill_solid( leds, NUM_LEDS, CRGB(0, 0, 0));
      if (buttonPushCounter == 110) { leds[8] = CHSV(modeColor, 255, 150); }
      FastLED.show();
    }

    leds[ledCimber] = CHSV(modeColor, 255, 150);
    FastLED.show();
  }
}

void eepromSet() 
{ //newEpprom = 1;          // first run eprom data save
  newEpprom = EEPROM.read(0);          // first run eprom data save
  if (newEpprom != 73) {
    
    Serial.println("New EPROM!");
    // eeprom values:
    newEpprom = 73;
    buttonPushCounter = 0;                  //
    channel = 8;                            //
    Bvariable = 3;                          //
    hueSelect = 8;                          //
    sensitivity = 3;                        //


    normal = 1;
    FastLED.setBrightness(((Bvariable * Bvariable) * 3) + 20); // set master brightness control

    EEPROM.update(0, newEpprom);
    EEPROM.update(1, buttonPushCounter);
    EEPROM.update(2, channel);
    EEPROM.update(3, Bvariable);
    EEPROM.update(4, hueSelect);
    EEPROM.update(5, sensitivity);

  }

  else { 
    Serial.println("Old EPROM!");                           //not new eeprom
    buttonPushCounter =  (int)EEPROM.read(1);
    channel =            (int)EEPROM.read(2);
    Bvariable =          (int)EEPROM.read(3);
    hueSelect =          (int)EEPROM.read(4);
    sensitivity =        (int)EEPROM.read(5);


    Serial.print("buttonPushCounter :   ");
    Serial.println(buttonPushCounter);
    Serial.print("channel :   ");
    Serial.println(channel);
    Serial.print("hueSelect :   ");
    Serial.println(hueSelect);
    Serial.print("sensitivity :   ");
    Serial.println(sensitivity);
    Serial.print("Bvariable :   ");
    Serial.println(Bvariable);

    buttonPushCounter =  (int)EEPROM.read(1);
    channel =            (int)EEPROM.read(2);
    Bvariable =          (int)EEPROM.read(3);
    hueSelect =          (int)EEPROM.read(4);
    sensitivity =        (int)EEPROM.read(5);


    if (buttonPushCounter < 0 || buttonPushCounter > 6){    // safety in case bad eprom reading
      buttonPushCounter = 0;
    }
    if (sensitivity < 0 || sensitivity > 9){    // safety in case bad eprom reading
      sensitivity = 3;
    }
    if (Bvariable < 0 || Bvariable > 9){    // safety in case bad eprom reading
      Bvariable = 3;
    }
    if (channel < 0 || channel > 9){    // safety in case bad eprom reading
      channel = 8;
    }
    normal = 1;
    FastLED.setBrightness(((Bvariable * Bvariable) * 3) + 20); // set master brightness control
  }
}

void indicatorModes() 
{

  //soundLevel = random(0, NUM_LEDS * 0.4); //was 80
  if (++dotCount >= 10) {    
    dotCount = 0;
    if (++previewCounter >= 20) {                   
    previewCounter = 0;
    }
  }
  soundLevel = preview[previewCounter];

  for (int i = 0; i < 8; i++)        
  {
    fftArray[i] = preview[random(20)]; 
  }

  if (buttonPushCounterDemo == 100) {
    musicmode1(); 
  }
  if (buttonPushCounterDemo == 101) {
    musicmode2(); 
  }
  if (buttonPushCounterDemo == 102) {
    musicmode3(); 
  }
  if (buttonPushCounterDemo == 103) {
    musicmode4(); 
  }
  if (buttonPushCounterDemo == 104) {
    musicmode5(); 
  }
}

void indicatorDemo(int loops)
{
  for (int i = 0; i < loops; i++) {
    //if (++dotCount >= 3) {                   // make the dot fall slowly
     // dotCount = 0;
      indicatorModes();
    //}  
    //delay(1);
    holding = touchRead(capPin);
    if  (holding > touchTime) {
     break;
    }
  }
}

void tap()
{ 
  if (normal == 1) {
    if (buttonPushCounter > 99) {
      buttonPushCounter = buttonPushCounter - 99;             //got to next mode
      Serial.print("buttonPushCounter Normal Modes = ");
      Serial.println(buttonPushCounter);
    }
    else {
      buttonPushCounter ++;
    }
    if (buttonPushCounter == 7) { buttonPushCounter = 0;  }   //number of modes
  }
  
  else if (normal == 0) {      //  settings modes
    if (buttonPushCounter > 99) {
      buttonPushCounter = buttonPushCounter - 99;             //got to next mode
      Serial.print("buttonPushCounter Settings Modes = ");
      Serial.println(buttonPushCounter);
    }
    else { 
      buttonPushCounter ++;
    }
    if (buttonPushCounter == 12) {  buttonPushCounter = 7; }
  }
}

void press()
{   
  fetchValue();
  //ledCimber = 8;

  if  (pushAndHold == 0) {        // no push and holds modes = M1 M2 M3 M4  
    indicatorModes();
  }

  else if (pushAndHold == 1) {        // settings & off modes     
    variableCounter++;
    if (variableCounter >= 9) { variableCounter = 0; }
    assignValue();
    indicators(variableCounter);     //  change indicator
  }

  else if (pushAndHold == 2) {       // rainbow = M5           
    variableCounter++;
    if (variableCounter > 8) {  variableCounter = 0;  }
    if (variableCounter < 0) {  variableCounter = 0;  }
    assignValue();
    indicatorModes();
  }

  else if (pushAndHold == 3) {       // lamp modes = L1      
    variableCounter++;
    if (variableCounter > 11) {   variableCounter = 8; }
    if (variableCounter < 8)  {   variableCounter = 8; }
    Serial.print("hueSelect = ");
    Serial.println(hueSelect);
    assignValue();
    lampMode();
  }

  if  (pushAndHold == 4) { 
    holding = touchRead(capPin);
    while( holding > touchTime ){
      ledCimber = ledCimber + 1;
      if (ledCimber > 107){         
        ledCimber = 108;
        Serial.println("ledClimber activate");
        assignValue();
        while( holding > touchTime){
          //do nothing
          //delay(1000);
          //isTouch = false;
          //break; 
        }
      }
      //Serial.print("ledCimber = ");
      //Serial.println(ledCimber);
      indicators(ledCimber);
      holding = touchRead(capPin);
    }
    ledCimber = 8;
    indicators(ledCimber);
  }
}

void prepareModes()
{
  if (normal == 1) {

    if      (buttonPushCounter == 0) {    // falling dot
      EEPROM.update(1, buttonPushCounter);
      buttonPushCounter = 100;
      buttonPushCounterDemo = 100;
      pushAndHold = 0;
      Serial.println("Falling Dot");
      indicatorDemo(numberLoops/2);
      previewCounter = 14;
    }

    else if (buttonPushCounter == 1) {    // middle out
      EEPROM.update(1, buttonPushCounter);
      buttonPushCounter = 101;
      buttonPushCounterDemo = 101;
      pushAndHold = 0;
      Serial.println("Middle Out");
      indicatorDemo(numberLoops*2);
      previewCounter = 0;
    }

    else if (buttonPushCounter == 2) {    // ripple
      EEPROM.update(1, buttonPushCounter);
      buttonPushCounter = 102;
      buttonPushCounterDemo = 102;
      pushAndHold = 0;
      Serial.println("Ripple");
      indicatorDemo(50);
    }

    else if (buttonPushCounter == 3) {    // fade
      EEPROM.update(1, buttonPushCounter);
      buttonPushCounter = 103;
      buttonPushCounterDemo = 103;
      pushAndHold = 0;
      Serial.println("Fade");
      indicatorDemo(numberLoops);
      previewCounter = 0;
    }

    else if (buttonPushCounter == 4) {    // music rainbow
      EEPROM.update(1, buttonPushCounter);
      buttonPushCounter = 104;
      buttonPushCounterDemo = 104;
      pushAndHold = 0;
      Serial.println("Rainbow");
      indicatorDemo(numberLoops*2);
    }

    else if (buttonPushCounter == 5) {   // lamp mode
      EEPROM.update(1, buttonPushCounter);
      buttonPushCounter = 105;
      buttonPushCounterDemo = 105;
      Serial.println("Lamp Mode");
      pushAndHold = 3;
    }

    else if (buttonPushCounter == 6) { //      OFF
      EEPROM.update(1, buttonPushCounter);
      buttonPushCounter = 106;
      pushAndHold = 4;
      ledCimber = 8;
      indcatorDots = 3;
      Serial.println("Off");
      for (int led = 0; led < NUM_LEDS; led++) {
        leds[led].setRGB( 0, 0, 0);
      }
      FastLED.show();
    }
  } // end normal modes start settings modes

  else {     // if noraml = 0  //  Settings Modes start here

    if (buttonPushCounter == 7) { //        frequency
      buttonPushCounter = 107;
      pushAndHold = 1;
      indcatorDots = 8;
      Serial.println("Freqency Setting");
    }

    else if (buttonPushCounter == 8) { //        sensitivity
      buttonPushCounter = 108;
      pushAndHold = 1;
      indcatorDots = 8;
      Serial.println("Sensitivity Setting");
    }

    else if (buttonPushCounter == 9) { //           brightness
      buttonPushCounter = 109;
      pushAndHold = 1;
      indcatorDots = 8;
      Serial.println("Brightness Setting");
    }
    else if (buttonPushCounter == 10) { //        reset
      buttonPushCounter = 110;
      pushAndHold = 4;
      ledCimber = 8;
      indcatorDots = 3;
      Serial.println("Reset Setting");
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
      Serial.println("Off Setting");
      for (int led = 0; led < NUM_LEDS; led++) {
        leds[led].setRGB( 0, 0, 0);
      }
      FastLED.show();
    }
  }
}

void runMode()
{
    // set normal loop modes here:
  if (normal == 1){  

    if (buttonPushCounter == 100) {                 // falling dot
      fetchSoundData();
      musicmode1(); 
    }
    else if (buttonPushCounter == 101) {                // middle out
      fetchSoundData();
      musicmode2(); 
    }
    else if (buttonPushCounter == 102) {                // ripple
      analyzeFFTall(); 
      musicmode3(); 
    }
    else if (buttonPushCounter == 103) {                // fade
      fetchSoundData();
      musicmode4(); 
    }
    else if (buttonPushCounter == 104) {                // rainbow music
      fetchSoundData();
      musicmode5(); 
    }
    else if (buttonPushCounter == 105) {                // lamp modes
      lampMode();
      //hueSelect changes color & type of lamp
    }
    else if (buttonPushCounter == 106) {              // Off
      //analyzeFFTall();
    }
  } // end normal modes start settings modes


  else {     // if noraml = 0  //  Settings Modes start here

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
      //analyzeFFTall();
      //indicators(off);
    }
    else if (buttonPushCounter == 112) {              // off
      //analyzeFFTall();
      //indicatorDemo(2);
    }
  }    //end settings modes here
}


void readSensor(){
  //isTouch = touchRead(capPin) > touchTime;
  //if (capReading == HIGH) { Serial.print("."); }
  //if (capReading > touchTime) { Serial.print("."); }

  capReading = touchRead(capPin);

  buffer.push(capReading);
  aveCapReading = 0;
	for (unsigned int i = 0; i < buffer.size(); i++) {
		aveCapReading += buffer[i] / buffer.size();
	}
	// Serial.print("Average is ");
	//Serial.println(aveCapReading);
  //if (aveCapReading > touchTime) { Serial.print("."); }
}