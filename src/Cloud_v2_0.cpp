
/*

  Cloud 2.0
  2018 Richard Clarkson Studio

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

    Layout:
    LED DATA_PIN 3 //MOSI  // Green
    LED CLK_PIN 4  //SCK  // Blue
    Button: pin 19

*/
//#define FASTLED_FORCE_SOFTWARE_SPI 1
#include <Arduino.h>
#include <FastLED.h>
#include <Audio.h>   
#include <EEPROM.h>
#include "TapPressButton.h"
#include <IRremote.h>

FASTLED_USING_NAMESPACE
TapPressButton capSensor;

//Global Varriables
uint8_t buttonPushCounter;
int channel;
int sensitivity;  // 0-8 where 8 = maximum sensitivity
int Bvariable;  // brightness

//Sound Variables
int soundLevel;          // this is the output of the FFT after being EQ
int dot = 100;           // this is a slowly falling value based on the peaks of soundLevel, used by musicMode1 and musicMode4
int dotCount = 1;
int lampMode2Count = 1;

//IR Varriables
#define NUM_BUTTONS 9 // The remote has 9 buttons

int RECV_PIN = 15;       //

// Prototype Functions:
void remote();
void upDownLeftRightReturn();
void upDownLeftRightRemote();
void upDownLeftRightRemoteHeld();


//*******************************       BUTTON CONSTANTS    ******************************************//

const uint16_t BUTTON_POWER = 0xD827; // i.e. 0x10EFD827  //55335
const uint16_t BUTTON_POWER_HELD = 23;

const uint16_t BUTTON_A = 0xF807;
const uint16_t BUTTON_A_HELD = 20;

const uint16_t BUTTON_A1 = 13;
const uint16_t BUTTON_A2 = 14;
const uint16_t BUTTON_A3 = 15;
const uint16_t BUTTON_A4 = 19;

const uint16_t BUTTON_B = 0x7887;
const uint16_t BUTTON_B_HELD = 21;

const uint16_t BUTTON_C = 0x58A7;
const uint16_t BUTTON_C_HELD = 22;

const uint16_t BUTTON_C1 = 16;
const uint16_t BUTTON_C2 = 17;
const uint16_t BUTTON_C3 = 29;
const uint16_t BUTTON_C4 = 30;


const uint16_t BUTTON_UP = 0xA05F;
const uint16_t BUTTON_DOWN = 0x00FF;
const uint16_t BUTTON_LEFT = 0x10EF;
const uint16_t BUTTON_RIGHT = 0x807F;

const uint16_t BUTTON_CIRCLE = 0x20DF;  // hex = 4815
const uint16_t BUTTON_CIRCLE_HELD = 24;

const uint16_t BUTTON_HELD = 0xFFFF;
const uint16_t BUTTON_B1 = 9;
const uint16_t BUTTON_B2 = 25;
const uint16_t BUTTON_B3 = 26;
const uint16_t BUTTON_B4 = 27;
const uint16_t BUTTON_B5 = 28;

uint16_t BUTTON_ARRAY[9] = {BUTTON_POWER, BUTTON_A, BUTTON_B, BUTTON_C, BUTTON_UP, BUTTON_DOWN, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_CIRCLE};


//*******************************       IR CONSTANTS    ******************************************//

int colourVariable = 20;
int variableState = 0;
unsigned long previousMillisSensor = 0;
unsigned long variableMillis = millis();


long checking = 0;
int newButtonPress = 0;
char currentButton;

uint16_t resultCode;
int buttonHeld = 0;
int remoteState;
int stateCounter = 0;

int previousRemoteState;


int mode;

int prevButtonHeld;

/* Initialize the irrecv part of the IRremote  library */
IRrecv irrecv(RECV_PIN);
decode_results results; // This will store our IR received codes
uint16_t lastCode = 0; // This keeps track of the last code RX'd








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
#define DATA_PIN 4 //MOSI  //7 Green
#define CLK_PIN 3  //SCK  //14 Blue
#define LED_TYPE WS2801 //APA102
#define COLOR_ORDER RGB
#define NUM_LEDS 25 //115 for normal Saber, 48 for half saber
CRGB leds[NUM_LEDS];
//#define FRAMES_PER_SECOND 120

int minLEDvalue[NUM_LEDS];
int goingUp[NUM_LEDS];
int currentValue[NUM_LEDS];

int currentValueFade = 15;
int goingUpFade = 1;

//TAP HOLD Varriables 
//uint8_t buttonState;


const int capPin = 19;
int touchTime = 1000;
unsigned long loopTime;
bool isTouch;
int holding = 0;
uint8_t prevButtonPushCounter;
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
int numberLoops = 50;
int capReading;
int aveCapReading;
bool flag = 0;
int flashCount = 1;


// Prototype Functions:
void musicmode1();
void musicmode2();
void musicmode3();
void musicmode4();
void musicmode5();
void lampMode1();
void lampMode2();
void lampMode3();
void lampMode4();
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
void previewModes();
void lampMode(); 
void fetchSoundData();
void previewController(int loops);
void tap();
void press();
void prepareModes();
void runMode();



void setup()
{ 
  irrecv.enableIRIn(); // Start the receiver
  AudioMemory(12);
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  //FastLED.addLeds<LED_TYPE, DATA_PIN, CLK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE, DATA_PIN, CLK_PIN, COLOR_ORDER, DATA_RATE_MHZ(2)>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(255);
  turnoffLEDs();
  FastLED.show();
  Serial.begin(9600);
  delay(3000);  // Sanity Delay
  for (int led = 0; led < NUM_LEDS; led++) {
    leds[led] = CHSV( 100, 0, 255);
  }
  for (int i = 0; i < NUM_LEDS; i++) {    //fill up the minimum LED value array for Fairy Light Mode
    minLEDvalue[i] = random(20,100);
  } 
  for (int i = 0; i < NUM_LEDS; i++) {    //fill up the going up value array for Fairy Light Mode
    goingUp[i] = random(0,1);
  }
    for (int i = 0; i < NUM_LEDS; i++) {    //fill up the current value array for Fairy Light Mode
    currentValue[i] = random(101,254);
  }

  FastLED.show();
  delay(250);
  turnoffLEDs();
  FastLED.show();

  // while (!Serial) {
  //   ; // wait for serial port to connect. Needed for native USB
  // }
  Serial.println("Cloud v2.0");
  //EEPROM.update(0, 1);       // uncomment to load default EPROM values
  //eepromSet();

    buttonPushCounter = 0;                  //
    prevButtonPushCounter = buttonPushCounter;
    channel = 8;                            //
    Bvariable = 8;                          //
    sensitivity = 1;                        //

  // aveCapReading = 0;
  // for (unsigned int i = 0; i < baselinelenght; i++) {
  //   baseLineReadings[i] = touchRead(capPin);
  //   //Serial.println(baseLineReadings[i]);
  //   aveCapReading = aveCapReading + baseLineReadings[i];
  // }
  // Serial.print("TotalCapReading");
  // Serial.println(aveCapReading);
  // aveCapReading = aveCapReading / baselinelenght;
  // Serial.print("aveCapReading");
  // Serial.println(aveCapReading);

  // touchTime = aveCapReading + 100;
  // Serial.print("Base line: ");
  // Serial.println(touchTime);
  pinMode(capPin, INPUT_PULLUP);
  isTouch = false;
  loopTime = 0;
  capSensor = TapPressButton(1, 499, 500, 500);
  remoteState = BUTTON_B_HELD;
}



void loop()
{ 
  remote();
  //isTouch = digitalRead(capPin);
  //loopTime = millis();

  if      (remoteState == BUTTON_POWER){
     if (flashCount == 1){
     for (int led = 0; led < NUM_LEDS; led++) {
        leds[led] = CHSV( 100, 0, 255);
      }
      FastLED.show();
      delay(200);
      turnoffLEDs();
      FastLED.show();
      flashCount = 0;
  }
    delay(3);
  }
  else if (remoteState == BUTTON_POWER_HELD){
    //reset();
  }
  else if (remoteState == BUTTON_A){
    fetchSoundData();
    musicmode2();
  }
  else if (remoteState == BUTTON_A_HELD){
    fetchSoundData();
    musicmode1();
  }
  else if (remoteState == BUTTON_B){
    fetchSoundData();
    musicmode4();
  }
  else if (remoteState == BUTTON_B_HELD){
    analyzeFFTall(); 
    musicmode3();
  }
  else if (remoteState == BUTTON_C){
    lampMode2();
  }
  else if (remoteState == BUTTON_C_HELD){
    lampMode3();
  }
}


  // prepareModes();               // load in startup values for each mode
  // runMode();                      // run the loop using selected mode
  // soundLevel = map(aveCapReading,0,3000,0,115);   // visual debugging backup
  // soundLevel = constrain(soundLevel,0,115);       // visual debugging backup
  // musicmode1();                                   // visual debugging backup


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

  if (++dotCount >= 10) {                   // make the dot fall slowly
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
    //turnoffLEDs();
    //fadeToBlackBy( leds, NUM_LEDS, 1);
    leds[NUM_LEDS / 2].setRGB(80, 80, 80); // keep center dot illuminated
  }
  FastLED.show(); // send data to LEDs to display
  delay(10);
}

void musicmode3()    // Ripple
{ 
  fadeToBlackBy( leds, NUM_LEDS, 1);
  //turnoffLEDs();
  for (int y = 0; y < 8; y++) // create 8 different LED sections of saber each based on the 8 FFT channels
  {
    int bottomOfRipple = ((NUM_LEDS / 8) * y);
    if (bottomOfRipple <= 0)
    {
      bottomOfRipple = 0;
    }
    int topOfRipple = ((NUM_LEDS / 8) * y) + (NUM_LEDS/8);
    if (topOfRipple >= NUM_LEDS)
    {
      topOfRipple = NUM_LEDS;
    }
    int rippleBrightness = constrain(fftArray[y] * 20, 0, 254);
    for (int led = bottomOfRipple; led < topOfRipple; led++)
    {
      leds[led] = CHSV(0, 0, rippleBrightness); // fill in LEDs according to the top and bottom of each section deffined above
    }
    //blur1d(leds, NUM_LEDS, fftArray[y]);  // blur LEDs for smoother transitions
    
    // readSensor();
    // if (aveCapReading > touchTime){      //EXIT TEST
    //   break;                             
    // }
  }
  FastLED.show();
}

void musicmode4()   // Fade
{ 
  if (soundLevel * 5 > dot){    dot = soundLevel * 5;  } // Keep dot on top of soundLevel
  if (dot > 255){    dot = 255;  }// Keep dot from going out of frame

  for (int led = 0; led < NUM_LEDS; led++)
  {
    leds[led] = CHSV( 200, 0, dot);
  }
  FastLED.show();
  if (++dotCount >= 5) {                   // make the dot fall slowly
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
  {
    leds[led] = CHSV( 100, 0, 0);
  }
  if (soundLevel <= 0)  // NO SOUND
  {                                    // If no sound (dot = 0)
   turnoffLEDs();
   //fadeToBlackBy( leds, NUM_LEDS, 1);
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

void lampMode1()  // Neon
{
  if (lampMode2Count == 0){
    lampMode2Count = 1;
  }
  rainbow(0, NUM_LEDS, 0.1);
  //rainbow(0, NUM_LEDS, 0.1);
  FastLED.show();
}

void lampMode2()  // Fairy Light
{
  if (lampMode2Count == 1){
   for (int led = 0; led < NUM_LEDS; led++) {
        leds[led] = CHSV( 100, 0, 255);
      }
      FastLED.show();
      lampMode2Count = 0;
  }
    EVERY_N_MILLISECONDS(30) {
    for (int x = 0; x < NUM_LEDS; x++) {
    if(goingUp[x] == 1){
      currentValue[x]++;
      if (currentValue[x] >= 255) {goingUp[x] = 0;}
    }
    else{
      currentValue[x]--;
      if (currentValue[x] <= minLEDvalue[x]) {goingUp[x] = 1;}
    }
    leds[x] = CHSV( 100, 0, currentValue[x]);
  }
  FastLED.show();
 }
}

void lampMode3()  // Ombre
{
  rainbow(0, NUM_LEDS, 5);
  //rainbow(0, NUM_LEDS, 1);
  FastLED.show();
}

void lampMode4()  // Breathing Light
{
  EVERY_N_MILLISECONDS(30) {
    if(goingUpFade == 1){
      currentValueFade++;
      if (currentValueFade >= 255) {goingUpFade = 0;}
    }
    else{
      currentValueFade--;
      if (currentValueFade <= 10) {goingUpFade = 1;}
    }
    for (int x = 0; x < NUM_LEDS; x++) {
    leds[x] = CHSV(100, 0, currentValueFade);
  }
  FastLED.show();
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

//*******************************      State Modes    ******************************************//


void assignValue()                                                   //assign
{
  if      (buttonPushCounter == 100) {  // falling dot
    if (ledCimber >= 107) {
      normal = 0;
      buttonPushCounter = 10;
    }
  }
  else if (buttonPushCounter == 101) {  // middle out
    if (ledCimber >= 107) {
      normal = 0;
      buttonPushCounter = 10;
    }
  }
  else if (buttonPushCounter == 102) {  // ripple
    if (ledCimber >= 107) {
      normal = 0;
      buttonPushCounter = 10;
    }
  }
  else if (buttonPushCounter == 103) {  //fade
    if (ledCimber >= 107) {
      normal = 0;
      buttonPushCounter = 10;
    }
  }
  else if (buttonPushCounter == 104) {  // rainbow music
    if (ledCimber >= 107) {
      normal = 0;
      buttonPushCounter = 10;
    }
  }
  else if (buttonPushCounter == 105) {   // white
    if (ledCimber >= 107) {
      normal = 0;
      buttonPushCounter = 10;
    }
  }
  else if (buttonPushCounter == 106) {   // neon
    if (ledCimber >= 107) {
      normal = 0;
      buttonPushCounter = 10;
    }
  }
  else if (buttonPushCounter == 107) {   // ombre
    if (ledCimber >= 107) {
      normal = 0;
      buttonPushCounter = 10;
    }
  }
  else if (buttonPushCounter == 108) {   // fire
    if (ledCimber >= 107) {
      normal = 0;
      buttonPushCounter = 10;
    }
  }
  else if (buttonPushCounter == 109) {   // off
    if (ledCimber >= 107) {
      normal = 0;
      buttonPushCounter = 10;
    }
  }
  else if (buttonPushCounter == 110) {     // frequency
    channel = variableCounter;
    Serial.print("channel = ");
    Serial.println(channel);
    EEPROM.update(2, channel);
  }
  else if (buttonPushCounter == 111) {     // sensitivity
    sensitivity = variableCounter;
    Serial.print("sensitivity = ");
    Serial.println(sensitivity);
    EEPROM.update(4, sensitivity);
  }
  else if (buttonPushCounter == 112) {     //brightness
    Bvariable = variableCounter;
    Serial.print("Bvariable = ");
    Serial.println(Bvariable);
    FastLED.setBrightness(((Bvariable * Bvariable) * 3) + 20);
    EEPROM.update(3, Bvariable);
  }

  else if (buttonPushCounter == 113) {     // reset
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
  else if (buttonPushCounter == 114) {    // off
    if (ledCimber >= 107) {
      normal = 1;
      buttonPushCounter = prevButtonPushCounter;

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
  else if (buttonPushCounter == 105) {  // white
  }
  else if (buttonPushCounter == 106) {  // neon
  }
  else if (buttonPushCounter == 107) {  // ombre
  }
  else if (buttonPushCounter == 108) {  // fire
  }
  else if (buttonPushCounter == 109) {  // off
  }
  else if (buttonPushCounter == 110) {  // frequncy
    variableCounter = channel;
  }
  else if (buttonPushCounter == 111) {  // sensitivity
    variableCounter = sensitivity;
  }
  else if (buttonPushCounter == 112) {  // brightness
    variableCounter = Bvariable;
  }
  else if (buttonPushCounter == 113) {  //reset
  }
  else if (buttonPushCounter == 114) {  // off
  }
}

void indicators(int variableSet) 
{
  if (indcatorDots == 8) {
    if      (buttonPushCounter == 110) {  modeColor =  100; }  // green
    else if (buttonPushCounter == 111) {  modeColor =  230; }  // pink
    else if (buttonPushCounter == 112) {  modeColor = 160;  }  // blue

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
    if      (buttonPushCounter == 109) {  modeColor = 120;  } // light blue
    else if (buttonPushCounter == 113) {  modeColor = 10;   } // red
    else if (buttonPushCounter == 114) {  modeColor = 120;  } // light blue
    else modeColor = 120;

    if (ledCimber <= 8) {
      fill_solid( leds, NUM_LEDS, CRGB(0, 0, 0));
      if (buttonPushCounter == 113) { leds[8] = CHSV(modeColor, 255, 150); }
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
    prevButtonPushCounter = buttonPushCounter;
    channel = 8;                            //
    Bvariable = 8;                          //
    sensitivity = 3;                        //


    normal = 1;
    //FastLED.setBrightness(((Bvariable * Bvariable) * 3) + 20); // set master brightness control

    EEPROM.update(0, newEpprom);
    EEPROM.update(1, buttonPushCounter);
    EEPROM.update(2, channel);
    EEPROM.update(3, Bvariable);
    EEPROM.update(4, sensitivity);

  }

  else { 
    Serial.println("Old EPROM!");                           //not new eeprom
    buttonPushCounter =  (int)EEPROM.read(1);
    channel =            (int)EEPROM.read(2);
    Bvariable =          (int)EEPROM.read(3);
    sensitivity =        (int)EEPROM.read(4);


    Serial.print("buttonPushCounter :   ");
    Serial.println(buttonPushCounter);
    Serial.print("channel :   ");
    Serial.println(channel);
    Serial.print("sensitivity :   ");
    Serial.println(sensitivity);
    Serial.print("Bvariable :   ");
    Serial.println(Bvariable);

    buttonPushCounter =  (int)EEPROM.read(1);
    channel =            (int)EEPROM.read(2);
    Bvariable =          (int)EEPROM.read(3);
    sensitivity =        (int)EEPROM.read(4);


    if (buttonPushCounter < 0 || buttonPushCounter > 9){    // safety in case bad eprom reading
      buttonPushCounter = 0;
    }
    prevButtonPushCounter = buttonPushCounter;

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
    //FastLED.setBrightness(((Bvariable * Bvariable) * 3) + 20); // set master brightness control
  }
}

void previewModes() 
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

void previewController(int loops)
{
  for (int i = 0; i < loops; i++) {
    //if (++dotCount >= 3) {                   // make the dot fall slowly
     // dotCount = 0;
      previewModes();
    //}  
    //delay(1);
    //readSensor();
    //holding = touchRead(capPin);
    // if  (aveCapReading > touchTime) {  // EXIT
    //  break;
    // }
  }
}

void tap()
{ 
  if (normal == 1) {
    if (buttonPushCounter > 99) {
      buttonPushCounter = buttonPushCounter - 99;             //got to next mode
      // Serial.print("buttonPushCounter Normal Modes = ");
      // Serial.println(buttonPushCounter);
    }
    else {
      buttonPushCounter ++;
    }
    if (buttonPushCounter == 10) { buttonPushCounter = 0;  }   //number of modes
  }
  
  else if (normal == 0) {      //  settings modes
    if (buttonPushCounter > 99) {
      buttonPushCounter = buttonPushCounter - 99;             //got to next mode
      // Serial.print("buttonPushCounter Settings Modes = ");
      // Serial.println(buttonPushCounter);
    }
    else { 
      buttonPushCounter ++;
    }
    if (buttonPushCounter == 15) {  buttonPushCounter = 10; }
  }
}

void press()
{   
  fetchValue();
  //ledCimber = 8;

  if  (pushAndHold == 0) {        // no push and holds modes = M1 M2 M3 M4  
    previewModes();
  }

  else if (pushAndHold == 1) {        // settings & off modes     
    variableCounter++;
    if (variableCounter >= 9) { variableCounter = 0; }
    assignValue();
    indicators(variableCounter);     //  change indicator
  }

  if  (pushAndHold == 4) { 
    //readSensor();
    turnoffLEDs();
    FastLED.show();
    while(digitalRead(capPin) == false){     // while held
      ledCimber = ledCimber + 1;
      if (ledCimber > 107){        
        ledCimber = 108;
        Serial.println("ledClimber activate");
        assignValue();
        while(digitalRead(capPin) == false){   // wait untill not held anymore
          //isTouch = digitalRead(capPin);
          //readSensor();
          //delay(1000);
          //isTouch = false;
          //break; 
        }
        delay(1000);
      }
      //Serial.print("ledCimber = ");
      //Serial.println(ledCimber);
      indicators(ledCimber);
      //readSensor();
      delay(3);
    }
    ledCimber = 8;
    indicators(ledCimber);
  }
}

void prepareModes()
{
  if (normal == 1) {
    EEPROM.update(1, buttonPushCounter);
    if      (buttonPushCounter == 0) {    // falling dot
      prevButtonPushCounter = buttonPushCounter;
      buttonPushCounter = 100;
      buttonPushCounterDemo = 100;
      pushAndHold = 4;
      ledCimber = 8;
      indcatorDots = 3;
      Serial.println("Falling Dot");
      //previewController(numberLoops/2);
      previewCounter = 14;
    }

    else if (buttonPushCounter == 1) {    // middle out
      prevButtonPushCounter = buttonPushCounter;
      buttonPushCounter = 101;
      buttonPushCounterDemo = 101;
      pushAndHold = 4;
      ledCimber = 8;
      indcatorDots = 3;
      Serial.println("Middle Out");
      //previewController(numberLoops);
      previewCounter = 0;
    }

    else if (buttonPushCounter == 2) {    // ripple
      prevButtonPushCounter = buttonPushCounter;
      buttonPushCounter = 102;
      buttonPushCounterDemo = 102;
      pushAndHold = 4;
      ledCimber = 8;
      indcatorDots = 3;
      Serial.println("Ripple");
      previewController(25);
    }

    else if (buttonPushCounter == 3) {    // fade
      prevButtonPushCounter = buttonPushCounter;
      buttonPushCounter = 103;
      buttonPushCounterDemo = 103;
      pushAndHold = 4;
      ledCimber = 8;
      indcatorDots = 3;
      Serial.println("Fade");
      previewController(numberLoops);
      previewCounter = 0;
    }

    else if (buttonPushCounter == 4) {    // music rainbow
      prevButtonPushCounter = buttonPushCounter;
      buttonPushCounter = 104;
      buttonPushCounterDemo = 104;
      pushAndHold = 4;
      ledCimber = 8;
      indcatorDots = 3;
      Serial.println("Rainbow");
      previewController(numberLoops);
    }

    else if (buttonPushCounter == 5) {   // White
      prevButtonPushCounter = buttonPushCounter;
      buttonPushCounter = 105;
      buttonPushCounterDemo = 105;
      Serial.println("White");
      lampMode2Count = 1;
      pushAndHold = 4;
      ledCimber = 8;
      indcatorDots = 3;
    }

    else if (buttonPushCounter == 6) {   // Neon
      prevButtonPushCounter = buttonPushCounter;
      buttonPushCounter = 106;
      buttonPushCounterDemo = 106;
      Serial.println("Neon");
      pushAndHold = 4;
      ledCimber = 8;
      indcatorDots = 3;
    }

    else if (buttonPushCounter == 7) {   // Ombre
      prevButtonPushCounter = buttonPushCounter;
      buttonPushCounter = 107;
      buttonPushCounterDemo = 107;
      Serial.println("Ombre");
      pushAndHold = 4;
      ledCimber = 8;
      indcatorDots = 3;
    }

    else if (buttonPushCounter == 8) {   // Fire
      prevButtonPushCounter = buttonPushCounter;
      buttonPushCounter = 108;
      buttonPushCounterDemo = 108;
      Serial.println("Fire");
      pushAndHold = 4;
      ledCimber = 8;
      indcatorDots = 3;
    }

    else if (buttonPushCounter == 9) { //      OFF
      prevButtonPushCounter = buttonPushCounter;
      buttonPushCounter = 109;
      pushAndHold = 4;
      ledCimber = 8;
      indcatorDots = 3;
      flashCount = 1;
      Serial.println("Off");
      turnoffLEDs();
      FastLED.show();
      //isTouch = false;
    }
  } // end normal modes start settings modes

  else {     // if noraml = 0  //  Settings Modes start here

    if (buttonPushCounter == 10) { //        frequency
      buttonPushCounter = 110;
      pushAndHold = 1;
      indcatorDots = 8;
      Serial.println("Freqency Setting");
    }

    else if (buttonPushCounter == 11) { //        sensitivity
      buttonPushCounter = 111;
      pushAndHold = 1;
      indcatorDots = 8;
      Serial.println("Sensitivity Setting");
    }

    else if (buttonPushCounter == 12) { //           brightness
      buttonPushCounter = 112;
      pushAndHold = 1;
      indcatorDots = 8;
      Serial.println("Brightness Setting");
    }
    else if (buttonPushCounter == 13) { //        reset
      buttonPushCounter = 113;
      pushAndHold = 4;
      ledCimber = 8;
      indcatorDots = 3;
      Serial.println("Reset Setting");
      turnoffLEDs();
      FastLED.show();
    }
    else if (buttonPushCounter == 14) { //        off
      buttonPushCounter = 114;
      pushAndHold = 4;
      ledCimber = 8;
      indcatorDots = 3;
      Serial.println("Off Setting");
      turnoffLEDs();
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
      musicmode4(); 
    }
    else if (buttonPushCounter == 101) {                // middle out
      fetchSoundData();
      musicmode2(); 
    }
    else if (buttonPushCounter == 102) {                // ripple
      analyzeFFTall(); 
      musicmode3(); 
    }
    else if (buttonPushCounter == 103) {                // sectional colour
      
    }
    else if (buttonPushCounter == 104) {                // rainbow music
      fetchSoundData();
      musicmode5(); 
    }
    else if (buttonPushCounter == 105) {                // Fairy Lamp
      lampMode1();
    }
    else if (buttonPushCounter == 106) {                // neon Lamp
      lampMode2();
    }
    else if (buttonPushCounter == 107) {                // ombre Lamp
      lampMode3();
    }
    else if (buttonPushCounter == 108) {                // Breathing Lamp
      lampMode4();
    }
    
    else if (buttonPushCounter == 109) {              // Off
      //analyzeFFTall();
      delay(3);
      //readSensor();
    }
  } // end normal modes start settings modes


  else {     // if noraml = 0  //  Settings Modes start here

    if (buttonPushCounter == 110) {              // Frequency
      indicators(channel);
    }
    else if (buttonPushCounter == 111) {              // Sensitvity
      indicators(sensitivity);
    }
    else if (buttonPushCounter == 112) {              // Brightness
      indicators(Bvariable);
    }
    else if (buttonPushCounter == 113) {              // reset
      indicators(reset);
    }
    else if (buttonPushCounter == 114) {              // off
      delay(3);
      //analyzeFFTall();
      //indicators(off);
      //readSensor();
    }
  }    //end settings modes here
}

//*******************************      IR Functions    ******************************************//


void remote() 
{


  if (millis() - checking > 250)
  {

    if (irrecv.decode(&results))     //check remote
    {

      resultCode = (results.value & 0xFFFF);

      Serial.print("Remote Code");
      Serial.println(resultCode);


      if (resultCode == 0xFFFF) {     //button held check
        buttonHeld++;


        if (currentButton == 'L') {
          upDownLeftRightRemoteHeld();
          remoteState = BUTTON_LEFT;
          colourVariable = 20;
        }
        else if (currentButton == 'R') {
          upDownLeftRightRemoteHeld();
          remoteState = BUTTON_RIGHT;
          colourVariable = 20;
        }
        else if (currentButton == 'U') {
          upDownLeftRightRemoteHeld();
          remoteState = BUTTON_UP;
        }
        else if (currentButton == 'D') {
          upDownLeftRightRemoteHeld();
          remoteState = BUTTON_DOWN;
        }

        else {
          if (buttonHeld >= 4) {                                   // Button Holds
             Serial.print("buttonHeld :    ");
             Serial.println(currentButton);

            if (currentButton == 'O') {
              remoteState = BUTTON_CIRCLE_HELD;
              buttonPushCounter = 13;
            }
            else if (currentButton == 'A') {
              previousRemoteState = remoteState;
              remoteState = BUTTON_A_HELD;
              buttonPushCounter = 0;
            }
            else if (currentButton == 'B') {
              previousRemoteState = remoteState;
              remoteState = BUTTON_B_HELD;
              buttonPushCounter = 2;
            }
            else if (currentButton == 'P') {
              remoteState = BUTTON_POWER_HELD;
              buttonPushCounter = 7;
            }
            else if (currentButton == 'C') {
              previousRemoteState = remoteState;
              remoteState = BUTTON_C_HELD;
            }
          }
        }
      }
      else {
        for (int i = 0; i < 9; i++) 
        {              //compare against each of the button codes
          if (resultCode == BUTTON_ARRAY[i]) {

            // ACTUAL BUTTON

            if (resultCode == BUTTON_POWER) {
              currentButton = 'P';
            }
            else if (resultCode == BUTTON_A) {
              currentButton = 'A';
            }
            else if (resultCode == BUTTON_B) {
              currentButton = 'B';
            }
            else if (resultCode == BUTTON_C) {
              currentButton = 'C';
            }
            else if (resultCode == BUTTON_UP) {
              currentButton = 'U';
                upDownLeftRightRemote();
                remoteState = BUTTON_UP;
              }
            else if (resultCode == BUTTON_DOWN) {
              currentButton = 'D';
                upDownLeftRightRemote();
                remoteState = BUTTON_DOWN;
              }
            else if (resultCode == BUTTON_LEFT) {
              currentButton = 'L';
                upDownLeftRightRemote();
                remoteState = BUTTON_LEFT;
                colourVariable = 4;
              }
            else if (resultCode == BUTTON_RIGHT) {
              currentButton = 'R';
                upDownLeftRightRemote();
                remoteState = BUTTON_RIGHT;
                colourVariable = 4;
              }
            else if (resultCode == BUTTON_CIRCLE) {
              currentButton = 'O';
            }

            newButtonPress = 1;
            //Serial.println("Comparing Button Code");

          }
          else {

            // NOT A REAL BUTTON
            //       #if defined(DEVMODE)
            //       if (newButtonPress == 0){
            //         Serial.print("NOT A REAL BUTTON    ");
            //         Serial.println(currentButton);
            //         }
            //       #endif
          }

        }
        buttonHeld = 0;
      }
        irrecv.resume(); // Receive the next value
    }
    else {

      // THIS IS WHERE BUTTONS ARE SET GIVEN A NEW BUTTON PRESS

      if (newButtonPress == 1) {
        newButtonPress = 0;

        if (buttonHeld > 4) { // number of seconds/ 4-1       // Button Holds
          // Current Button Held

          
        }


        else {                                         // Button Single Presses
            // Current Button
            Serial.println(currentButton);

            if (currentButton == 'P') {
              remoteState = BUTTON_POWER;
              buttonPushCounter = 9;
            }
            else if (currentButton == 'A') {
              remoteState = BUTTON_A;
              buttonPushCounter = 1;
            }
            else if (currentButton == 'B') {
              remoteState = BUTTON_B;
              buttonPushCounter = 3;
            }
            else if (currentButton == 'C') {
              remoteState = BUTTON_C;
              buttonPushCounter = 5;
            }
            else if (currentButton == 'O') {
            previousRemoteState = remoteState;
            remoteState = BUTTON_CIRCLE;
          }
        }
        //Serial.println("Preparing");
        prepareModes();               // load in startup values for each mode and run preview
        //Serial.println("Prepaired");
        buttonHeld = 0;

      }
      //EEPROM.write(0, remoteState);             //fix this later
    }
    checking = millis();
  }
}


void upDownLeftRightReturn() 
{      //go back into prev mode

  variableState = 1;
  remoteState = previousRemoteState;
  variableMillis = millis();
  //flashoff (N_PIXELS, 0, 0, 0, 0);
}


void upDownLeftRightRemote() 
{

  variableState = 0;
  //stateCounter = 0;

  if ((remoteState != BUTTON_RIGHT) && (remoteState != BUTTON_LEFT) && (remoteState != BUTTON_UP) && (remoteState != BUTTON_DOWN))
  {
    if (remoteState == 1000)
    {
      previousRemoteState = BUTTON_POWER;
    }
    else {
      previousRemoteState = remoteState;
    }

  }

  variableMillis = millis();
  irrecv.resume(); // Receive the next value
  remote();
}

void upDownLeftRightRemoteHeld() 
{

  variableState = 0;
  variableMillis = millis();
  irrecv.resume(); // Receive the next value
  remote();
}


