
/*
  Cloud 3.1
  2020 Richard Clarkson Studio

  TODO fix LED flickering: For some reason any LED quantity over 25 results in LED flickering, symptoms also result when the Speed of animation setting is active.
   - temporary workaround limit LEDs to 25 and remove Speed of Animation setting.
  
*/


#include <Arduino.h>
#include <FastLED.h>
#include <Audio.h>   
#include <EEPROM.h>
#include <IRremote.h>

FASTLED_USING_NAMESPACE
#define NUM_LEDS 50        //     T=10 S=17 M=25 L=50* L= 2 sets of 25

int LED_ADJUSTED = 25;     

const int shunt1Pin = 5;     // We will use shunts to change quantity of LEDs on the fly
const int shunt2Pin = 6;
int shunt1;
int shunt2;


//Global Varriables
int channel = 8;
int sensitivity;  // 0-8 where 8 = maximum sensitivity
int Bvariable;  // brightness

//Sound Variables
int soundLevel;          // this is the output of the FFT after being EQ
int dot = 100;           // this is a slowly falling value based on the peaks of soundLevel, used by musicMode1 and musicMode4
int dotCount = 1;

int Bass;
int Mid;
int High;
int prevBass;
int prevMid;
int prevHigh;

const int numReadings = 60;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

int readingsM[numReadings];      // the readings from the analog input
int readIndexM = 0;              // the index of the current reading
int totalM = 0;                  // the running total
int averageM = 0;                // the average

int readingsH[numReadings];      // the readings from the analog input
int readIndexH = 0;              // the index of the current reading
int totalH = 0;                  // the running total
int averageH = 0;                // the average


//IR Varriables
#define NUM_BUTTONS 18 // The remote has 9 buttons

int RECV_PIN = 15;       //

// Prototype Functions:
void remote();
void upDownLeftRightReturn();
void upDownLeftRightRemote();
void upDownLeftRightRemoteHeld();


//*******************************       BUTTON CONSTANTS    ******************************************//

const uint16_t BUTTON_1 = 0xD827; // i.e. 0x10EFD827  //55335  - was Button Power
const uint16_t BUTTON_1_HELD = 23;

const uint16_t BUTTON_2 = 0x7887;  // was button B
const uint16_t BUTTON_3 = 0x58A7;  // was button C
const uint16_t BUTTON_4 = 0xF807;  // was button A
const uint16_t BUTTON_5 = 0x609F;   // was button B held
const uint16_t BUTTON_6 = 0xE01F;   // was button C held
const uint16_t BUTTON_7 = 0x20DF; // hex = 4815   was button circle
const uint16_t BUTTON_8 = 0x906F;   // was button circle held  
const uint16_t BUTTON_9 = 0x08F7;   // was button A HELD

const uint16_t BUTTON_AUP = 0xA05F;
const uint16_t BUTTON_ADOWN = 0x00FF;   
//const uint16_t BUTTON_ARESET = 0x28D7;  //na 

const uint16_t BUTTON_BUP = 0xC03F;
const uint16_t BUTTON_BDOWN = 0x40BF;
//const uint16_t BUTTON_BRESET = 0xA857; //na

const uint16_t BUTTON_CUP = 0x10EF;     
const uint16_t BUTTON_CDOWN = 0x807F;   
//const uint16_t BUTTON_CRESET = 0x6897;  //na 

const uint16_t BUTTON_HELD = 0xFFFF;

uint16_t BUTTON_ARRAY[15] = {BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4, BUTTON_5, BUTTON_6, BUTTON_7, BUTTON_8, BUTTON_9, BUTTON_AUP, BUTTON_ADOWN, BUTTON_BUP, BUTTON_BDOWN, BUTTON_CUP, BUTTON_CDOWN};

uint16_t BUTTON_ARRAY2[9] = {BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4, BUTTON_5, BUTTON_6, BUTTON_7, BUTTON_8, BUTTON_9,};


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
int remotEeprom;

int previousRemoteState;

int butStateCounter = 1;
int demoCounter = 1;

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

static const int wheelH[] = {
    0,  25,  50,  75, 100, 125, 150, 175, 200, 225, 250, 255, 255
};

static const int wheelS[] = {
  255, 255, 255, 255, 255, 255, 255, 255, 255, 200, 100,  50,   0
};
int wheelPosition;

int speedOfAnimation[] = {
  30, 20, 17, 12, 5, 1
};
int timeSpeed;

//Lamp Mode Variables
int rainbowCounter = 0;

uint8_t gHue = 180;           // rotating "base color" used by many of the patterns

//LED Variables
#define DATA_PIN 4 //MOSI  // Green
#define CLK_PIN 3  //SCK  // Blue
#define LED_TYPE WS2801 //APA102
#define COLOR_ORDER RGB
CRGB leds[NUM_LEDS];

int minLEDvalue[NUM_LEDS];
int goingUp[NUM_LEDS];
int currentValue[NUM_LEDS];

int currentValueFade = 15;
int goingUpFade = 1;

int ledSingle1;
int ledSingle2;
int ledSingle3;

int modeColor;
int indcatorDots;
uint8_t ledCimber = 8;
int variableCounter = 9;  //global
uint8_t pushAndHold = 0;
uint8_t dotBrightness = 250;
uint8_t dotBrightnessDirection = 1;
int newEpprom;
uint8_t automatedIndicator;
int numberLoops = 50;
int capReading;
int aveCapReading;
bool flag = 0;
int flashCount = 1;
int flashCountA = 1;
int calm = 10;


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
void turnoffLEDs();
void analyzeLevel();
void analyzeFFT();
void analyzeFFTall();
void FFTreading(int FFTchannel);
void eepromSet();
void fetchSoundData();
void timedLightening(int gap);
void demo();
void soundLightening();
void strom();
void flash(int hue, int saturation);
void reset();


void setup()
{ 
  irrecv.enableIRIn(); // Start the IR receiver
  AudioMemory(12);
  Serial.println("Cloud v3.0");

  pinMode(shunt1Pin, INPUT_PULLUP);    
  pinMode(shunt2Pin, INPUT_PULLUP);  

  shunt1 = digitalRead(shunt1Pin);  
  shunt2 = digitalRead(shunt2Pin); 

  if (shunt1 == LOW && shunt2 == HIGH)
  { 
    LED_ADJUSTED = 50; //used for RGBH Clouds that have all 50 LEDs
    Serial.println("Pin 5");
    }

  else if (shunt1 == HIGH && shunt2 == LOW)
  { 
    LED_ADJUSTED = 17; //used for RGBM Clouds that only have 17 LEDs
    Serial.println("Pin 6");
    }

  else if (shunt1 == LOW && shunt2 == LOW)
  { 
    LED_ADJUSTED = 10; //used for RGBS Clouds that have only 10 LEDs
    Serial.println("BOTH");
    }


  FastLED.addLeds<LED_TYPE, DATA_PIN, CLK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  turnoffLEDs();
  FastLED.show();
  turnoffLEDs();
  FastLED.show();
  Serial.begin(9600);
  delay(1000);  // Sanity Delay
  turnoffLEDs();
  FastLED.show();
  for (int i = 0; i < LED_ADJUSTED; i++) {    
    minLEDvalue[i] = random(1,150);       //fill up the minimum LED value array for Fairy Light Mode
    currentValue[i] = random(1,254);      //fill up the current value array for Fairy Light Mode
    goingUp[i] = random(0,1);             //fill up the going up value array for Fairy Light Mode
  }
  for (int i = 0; i < 1000; i++) {
    for (int x = 0; x < LED_ADJUSTED; x++) {
      if(goingUp[x] == 1){
        currentValue[x]++;
        if (currentValue[x] >= 255) {goingUp[x] = 0;}
      }
      else{
        currentValue[x]--;
        if (currentValue[x] <= minLEDvalue[x]) {goingUp[x] = 1;}
      }
    }
  }
  flash(100, 0);
  turnoffLEDs();
  FastLED.show();
  
  //EEPROM.update(0, 1);       // uncomment to load default EPROM values
  eepromSet();
    // butStateCounter = 1;                  //
    // timeSpeed = 2;                          //
    // Bvariable = 8;                          //
    // sensitivity = 4;                        //
    // remoteState = BUTTON_2;
}



void loop()
{ 
  remote();

  //   shunt1 = digitalRead(shunt1Pin);  
  // if (shunt1 == LOW)
  // { 
  //   //LED_ADJUSTED = 10; //used for RGBS Clouds that only have 10 LEDs
  //   Serial.println("Pin 5");
  //   }

  //   shunt2 = digitalRead(shunt2Pin);  
  // if (shunt2 == LOW)
  // { 
  //   //LED_ADJUSTED = 17; //used for RGBM Clouds that only have 17 LEDs
  //   Serial.println("Pin 6");
  //   }

  if      (remoteState == BUTTON_1){
     if (flashCount == 1){  
       flash(100,0); 
       Serial.print("flash white");
       turnoffLEDs();
       FastLED.show(); 
       flashCount = 0; 
       }
  }
  else if (remoteState == BUTTON_1_HELD){
      //reset();
  }
  else if (remoteState == BUTTON_8){
    analyzeFFTall(); 
    musicmode3(); 
  }
  else if (remoteState == BUTTON_9){
    lampMode4();  
  }
  else if (remoteState == BUTTON_2){
    fetchSoundData();
    musicmode4();  
  }
  else if (remoteState == BUTTON_5){
    analyzeFFTall(); 
    musicmode5();  
  }
  else if (remoteState == BUTTON_3){
    lampMode2(); 
  }
  else if (remoteState == BUTTON_6){
    lampMode3();
  }
  else if (remoteState == BUTTON_4){    
    if (flashCount == 1){  
       strom();
       flashCount = 0;  
      }
    timedLightening(30);
  }

  else if (remoteState == BUTTON_7){
    fetchSoundData();
    soundLightening();
  }


  else if (remoteState == BUTTON_AUP)            // Global brightness
  {
    if(millis() - variableMillis > 1000){
        upDownLeftRightReturn();
     }
      if(variableState == 0){             
          if (Bvariable < 8){        Bvariable++;   }
          FastLED.setBrightness(map(Bvariable,0,8,20,255));
          fill_solid( leds, LED_ADJUSTED, CHSV(60,150,(map(Bvariable,0,8,20,250))));
          FastLED.show();
          Serial.print("Bvariable = ");
          Serial.println(Bvariable);
          EEPROM.update(3, Bvariable);
        variableState = 1;
    } 
  }

  else if (remoteState == BUTTON_BUP)            // sensitivity
  {
    if(millis() - variableMillis > 1000){
        upDownLeftRightReturn();
     }
      if(variableState == 0){
          if (sensitivity < 8){     sensitivity++;  }
          fill_solid( leds, LED_ADJUSTED, CHSV(180,150,(map(sensitivity,0,8,20,250))));
          FastLED.show();
          Serial.print("sensitivity = ");
          Serial.println(sensitivity);
          EEPROM.update(4, sensitivity);
        variableState = 1;
    }    
  }

  // else if (remoteState == BUTTON_CUP)           // Speed of Animation
  // {
  //   if(millis() - variableMillis > 1000){
  //       upDownLeftRightReturn();
  //    }
  //     if(variableState == 0){
        
  //         if (timeSpeed < 5){          timeSpeed++;     }
  //         fill_solid( leds, LED_ADJUSTED, CHSV(110,150,(map(timeSpeed,0,5,20,250))));
  //         FastLED.show();
  //         Serial.print("timeSpeed = ");
  //         Serial.println(speedOfAnimation[timeSpeed]);
  //         EEPROM.update(2, timeSpeed);
  //       variableState = 1;
  //   }    
  // }

  else if (remoteState == BUTTON_ADOWN)              // Global brightness
  {
    if(millis() - variableMillis > 1000){
        upDownLeftRightReturn();
     }
      if(variableState == 0){
          if (Bvariable > 0){     Bvariable--;   }
          FastLED.setBrightness(map(Bvariable,0,8,20,255));
          fill_solid( leds, LED_ADJUSTED, CHSV(60,150,(map(Bvariable,0,8,20,250))));
          FastLED.show();
          Serial.print("Bvariable = ");
          Serial.println(Bvariable);
          EEPROM.update(3, Bvariable);
        variableState = 1;
    }    
  }

  else if (remoteState == BUTTON_BDOWN)               // sensitivity
  {
    if(millis() - variableMillis > 1000){
        upDownLeftRightReturn();
     }
      if(variableState == 0){
          if (sensitivity > 0){     sensitivity--;  }
          fill_solid( leds, LED_ADJUSTED, CHSV(180,150,(map(sensitivity,0,8,20,250))));
          FastLED.show();
          Serial.print("sensitivity = ");
          Serial.println(sensitivity);
          EEPROM.update(4, sensitivity);
        variableState = 1;
    }
  }

  // else if (remoteState == BUTTON_CDOWN)             // Speed of Animation
  // {
  //   if(millis() - variableMillis > 1000){
  //       upDownLeftRightReturn();
  //    }
  //     if(variableState == 0){
  //         if (timeSpeed > 0){          timeSpeed--;     }
  //         fill_solid( leds, LED_ADJUSTED, CHSV(110,150,(map(timeSpeed,0,5,20,250))));
  //         FastLED.show();
  //         Serial.print("timeSpeed = ");
  //         Serial.println(speedOfAnimation[timeSpeed]);
  //         EEPROM.update(2, timeSpeed);
  //       variableState = 1;
  //   }  
  // }

  else if ((remoteState == BUTTON_CDOWN) || (remoteState == BUTTON_CUP))
  {
    for (int x = 0; x < LED_ADJUSTED; x++) {
      leds[x] = CHSV(wheelH[wheelPosition], wheelS[wheelPosition], 200);
     }
    FastLED.show();

    if(millis() - variableMillis > 1000){
        upDownLeftRightReturn();
     }
  }
}


  // prepareModes();               // load in startup values for each mode
  // runMode();                      // run the loop using selected mode
  // soundLevel = map(aveCapReading,0,3000,0,115);   // visual debugging backup
  // soundLevel = constrain(soundLevel,0,115);       // visual debugging backup
  // musicmode1();                                   // visual debugging backup

//*******************************       Cloud Modes    ******************************************//

void timedLightening(int gap)
{
  EVERY_N_SECONDS(calm){
    strom();
    calm = random(gap);
  }
}

void soundLightening()
{ 
  //  if (dot != soundLevel){
  //  Serial.println(soundLevel);
  //  dot = soundLevel;
  //  }
  if (soundLevel > 180){    //was 250
  strom();
 }
}

void demo()
{
  if(demoCounter < 3000) { demoCounter++; }
  else {demoCounter = 0 ; }

  if(demoCounter > 0 && demoCounter < 15000){
    lampMode1();
  }
  else if(demoCounter >= 1500 && demoCounter < 29990){
    lampMode3();
  }
  else if(demoCounter >= 29990 && demoCounter < 30000){
    timedLightening(30);
  }
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
    soundLevel = constrain(soundLevel, 0, 255);              // limit
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
  fftArray[FFTchannel] = constrain(fftArray[FFTchannel], 0, 255);                // limit
}

void musicmode1()   // Falling Dot
{ 
  //fadeToBlackBy( leds, LED_ADJUSTED, 3);
  turnoffLEDs();

  soundLevel = (map(soundLevel,0,150,0,LED_ADJUSTED+1))-1;    //250
  if (soundLevel > dot)  dot = soundLevel; // Keep dot on top of soundLevel
  if (dot > LED_ADJUSTED)    dot = LED_ADJUSTED; // Keep dot from going out of frame

  //turnoffLEDs();
  if (soundLevel > 1){
  for (int led = 0; led < dot; led++)
  { // Start by Filling LEDS up to the soundLevel with dim white
    leds[led] = CHSV( 100, 0, 255);
   } 
  }
  //leds[dot] = CHSV( 100, 150, 255);
  
  // for (int led = dot; led < LED_ADJUSTED; led++)
  // { //make everything above the dot black
  //   leds[led].setRGB(0, 0, 0);
  // }
  FastLED.show(); // send data to LEDs to display

  if (++dotCount >= 40) {                   // make the dot fall slowly
    dotCount = 0;
    if (dot > 0) {
      dot--;
    }
  }
}

void musicmode2()   // Middle Out 
{ 
  soundLevel = map(soundLevel, 0, 255,0, LED_ADJUSTED);              // limit
  turnoffLEDs();
  for (int led = (LED_ADJUSTED - soundLevel) / 2; led < (soundLevel / 2) + (LED_ADJUSTED / 2); led++)
  {
    leds[led].setRGB(50, 50, 50);
  }
  if (soundLevel <= 0)  // NO SOUND
  {                                    // If no sound (dot = 0)
    leds[LED_ADJUSTED / 2].setRGB(80, 80, 80); // keep center dot illuminated
  }
  FastLED.show(); // send data to LEDs to display
  delay(10);
}

void musicmode3()    // Ripple
{ 
  for (int y = 0; y < 8; y++) // create 8 different LED sections of the Cloud each based on the 8 FFT channels
  {
    int bottomOfRipple = ((LED_ADJUSTED / 8) * y);                        //break the LEDs into groups of 8
    if (bottomOfRipple <= 0){      bottomOfRipple = 0;    }           //lower limit

    int topOfRipple = ((LED_ADJUSTED / 8) * y) + (LED_ADJUSTED/8);            //break the LEDs into groups of 8
    if (topOfRipple >= LED_ADJUSTED){      topOfRipple = LED_ADJUSTED;  }     //upper limit

    int rippleBrightness = fftArray[y] * 5;                           //scale the brightness up
    rippleBrightness = constrain( rippleBrightness, 0, 255 );         //limit to floor and celling
    for (int led = bottomOfRipple; led < topOfRipple; led++)
    {
      leds[led] = CHSV(wheelH[wheelPosition], wheelS[wheelPosition], rippleBrightness); // fill in LEDs according to the top and bottom of each section deffined above
    }
  }
  FastLED.show();      //send data to LEDs
}

void musicmode4()   // Fade
{ 
  if (soundLevel * 3 > dot){    dot = soundLevel * 3;  } // scale up by 3
  if (dot > 255){    dot = 255;  }                       // upper limit

  for (int led = 0; led < LED_ADJUSTED; led++)
  {
    leds[led] = CHSV( wheelH[wheelPosition], wheelS[wheelPosition], dot);    // prepare data for LEDs
  }
  FastLED.show();                                        // send data to LEDS
  if (++dotCount >= 5) {                                 // make the brightness fall 1 level each five frames
    dotCount = 0;
    if (dot > 0) {
      dot--;
    }
  }
}

void musicmode5()     // Colorful Music Mode
{   
  total = total - readings[readIndex];                 // subtract the last reading
  readings[readIndex] = constrain((fftArray[0] + fftArray[1]),0,255);   // read data from the mic for BASS channels
  total = total + readings[readIndex];                 // add the reading to the total
  readIndex = readIndex + 1;                           // advance to the next position in the array
  if (readIndex >= numReadings) { readIndex = 0; }     // if we're at the end of the array wrap around to the beginning
  average = total / numReadings;                       // calculate the average

  totalM = totalM - readingsM[readIndexM];             // subtract the last reading
  readingsM[readIndexM] = constrain((fftArray[2] + fftArray[3]),0,255);   // read data from the mic for MID channels
  totalM = totalM + readingsM[readIndexM];             // add the reading to the total
  readIndexM = readIndexM + 1;                         // advance to the next position in the array
  if (readIndexM >= numReadings) { readIndexM = 0; }   // if we're at the end of the array wrap around to the beginning
  averageM = totalM / numReadings;                     // calculate the average

  totalH = totalH - readingsH[readIndexH];             // subtract the last reading
  readingsH[readIndexH] = constrain((fftArray[4] + fftArray[5] + fftArray[6] + fftArray[7]),0,255);   // read data from the mic for HIGH channels
  totalH = totalH + readingsH[readIndexH];             // add the reading to the total
  readIndexH = readIndexH + 1;                         // advance to the next position in the array
  if (readIndexH >= numReadings) { readIndexH = 0; }   // if we're at the end of the array wrap around to the beginning
  averageH = totalH / numReadings;                     // calculate the average

    for (int x = 0; x < LED_ADJUSTED; x++) {
    leds[x].setRGB(averageH, averageM, average);       //prepare data to send to LEDs
  }
  FastLED.show();                                      //send data to LEDs
}

//*******************************      Lamp Modes    ******************************************//

void turnoffLEDs()
{
    for (int led = 0; led < NUM_LEDS; led++)        // changed to NUM_LEDS just in case it misses any extra LEDS
  { //turn off LEDs
    leds[led] = CHSV( 100, 0, 0);
  }
}

void flash(int hue, int saturation)
{
     for (int led = 0; led < LED_ADJUSTED; led++) {
        leds[led] = CHSV( hue, saturation, 200);
      }
      FastLED.show();
      delay(200);
      turnoffLEDs();
      FastLED.show();
      flashCount = 0;
}

void lampMode1()  // Neon
{
  rainbow(0, LED_ADJUSTED, 0.1);
  FastLED.show();
}

void lampMode2()  // Fairy Light
{
    EVERY_N_MILLISECONDS_I(thistimer,10) {                //delay to slow down the animation to a calm breathing speed
    thistimer.setPeriod(speedOfAnimation[timeSpeed]);     //setting to adjust the speed of the delay
    for (int x = 0; x < LED_ADJUSTED; x++) {                  //this mode controls each LED individually where each LED has a dynamic setting about its fade level, fade direction and minimum fade value
    if(goingUp[x] == 1){
      currentValue[x]++;                                  //if the fade direction for this LED is up then add one brightness to the LED
      if (currentValue[x] >= 255) {goingUp[x] = 0;}       //if the fade direction for this LED is reaches the maximum (255) change fade direction
    }
    else{
      currentValue[x]--;                                  //if the fade direction for this LED is down then remove one brightness from the LED
      if (currentValue[x] <= minLEDvalue[x]) {goingUp[x] = 1;} //if the fade direction for this LED is reaches the minimum change fade direction
    }
    leds[x] = CHSV( wheelH[wheelPosition], wheelS[wheelPosition], currentValue[x]);   //prepare the data for the LED given the above in global color
  }
  rainbowCounter = 0;                                     //indicator flag
 }
 FastLED.show();                                          //send data to LEDs
}

void lampMode3()  // Ombre
{
  rainbow(0, LED_ADJUSTED, 5);     //color fuction fill LEDs strip with color so that 50% of the colorwheel is visible     
  FastLED.show();              //send data to LEDs
}

void lampMode4()  // Breathing Light
{
  EVERY_N_MILLISECONDS_I(thistimer,10) {                  //delay to slow down the animation to a calm breathing speed
    thistimer.setPeriod(speedOfAnimation[timeSpeed]);     //setting to adjust the speed of the delay
    if(goingUpFade == 1){                                 
      currentValueFade++;                                 //if the animation is going up add one more to the brightness
      if (currentValueFade >= 255) {goingUpFade = 0;}     //if the animation reaches the top start fading out
    }
    else{
      currentValueFade--;                                 //if the animation is still fading out remove one from the brightness
      if (currentValueFade <= 10) {goingUpFade = 1;}      //if the animation reaches the bottom (10) start fading in
    }
    for (int x = 0; x < LED_ADJUSTED; x++) {
      leds[x] = CHSV(wheelH[wheelPosition], wheelS[wheelPosition], currentValueFade);    //HSV data to send to LEDs
    }
    rainbowCounter = 0;                                   //indicator flag
  }
  FastLED.show();                                         //send data to LEDs
}

void rainbow(int startPos, int number, float deltaHue)             //FASTLED function
{
    if (++rainbowCounter >= speedOfAnimation[timeSpeed] * 3) {     //rotate color wheel depending on speed setting variable
      gHue++;
      rainbowCounter = 0;
    } 
    fill_rainbow( &(leds[startPos]), number, gHue, deltaHue);     //send data to LEDs
}

void strom()
{
  for (int led = 0; led < int(random(LED_ADJUSTED)); led++) {          //turn on a random chain of LEDs white
        leds[led] = CHSV( 100, 0, 255);
        FastLED.show();
      }

  for (int led = 0; led < int(random(LED_ADJUSTED)); led++) {  leds[led] = CHSV( 100, 0, 255);    }   //turn on another random chain of LEDs white
      FastLED.show();
      delay(random(10, 40));                                                                      //random delay between 10-40
  for (int led = 0; led < int(random(LED_ADJUSTED)); led++) {  leds[led] = CHSV( 100, 0, 0);      }   //turn off a random chain of LEDs
      FastLED.show();
      delay(random(10, 200));                                                                     //random delay between 10-200
  for (int led = 0; led < LED_ADJUSTED; led++) {          leds[led] = CHSV( 100, 0, 0);      }        //turn off all LEDs
      FastLED.show();
  
  for (int led = 0; led < int(random(LED_ADJUSTED)); led++) {  leds[led] = CHSV( 100, 0, 255);    }   //turn on another random chain of LEDs white
      FastLED.show();
      delay(random(10, 50));                                                                      //random delay between 10-50
  for (int led = 0; led < int(random(LED_ADJUSTED)); led++) {  leds[led] = CHSV( 100, 0, 0);      }   //turn off a random chain of LEDs
      FastLED.show();
      delay(random(10, 100));                                                                     //random delay between 10-100

  for (int led = 0; led < LED_ADJUSTED; led++) {          leds[led] = CHSV( 100, 0, 0);      }        //turn off all LEDs
      FastLED.show();

  ledSingle1 = random(LED_ADJUSTED);                                                  //choose a random LED
  leds[ledSingle1] = CHSV( 100, 0, 255);                                          //turn it on for a random time between 10-100 then turn it off. Do this twice.
  FastLED.show();
  delay(random (10, 100));
  leds[ledSingle1] = CHSV( 100, 0, 0);
  FastLED.show();
  leds[ledSingle1] = CHSV( 100, 0, 255);
  FastLED.show();
  delay(random (40, 100));
  leds[ledSingle1] = CHSV( 100, 0, 0);
  FastLED.show();

  if (random(1, 3) == 2) {                                                        //every 1 out of 3 times flash another LED
    ledSingle1 = random (LED_ADJUSTED);
    ledSingle2 = random (50);
    leds[ledSingle1] = CHSV( 100, 0, 255);
    if(ledSingle2 < LED_ADJUSTED){   leds[ledSingle2] = CHSV( 100, 0, 255);  }
    FastLED.show();
  }
  else {
    ledSingle1 = random (LED_ADJUSTED);                                               //every 2 out of 3 times flash a small group of LEDs on and off
    ledSingle2 = ledSingle1 + 1;
    ledSingle3 = ledSingle2 + 2;
    leds[ledSingle1] = CHSV( 100, 0, 255);
    FastLED.show();
    delay(random (20));
    if(ledSingle2 < LED_ADJUSTED){   leds[ledSingle2] = CHSV( 100, 0, 255); }
    FastLED.show();
    delay(random (20));
    if(ledSingle3 < LED_ADJUSTED){   leds[ledSingle3] = CHSV( 100, 0, 255); }
    FastLED.show();
  }

  for (int led = 0; led < LED_ADJUSTED; led++) {          leds[led] = CHSV( 100, 0, 0);      }    //turn off all LEDS
      FastLED.show();

  if (random(1, 3) == 2) {                                                        //repeat previous small flashes
    ledSingle1 = random (LED_ADJUSTED);
    ledSingle2 = random (50);
    leds[ledSingle1] = CHSV( 100, 0, 255);
    if(ledSingle2 < LED_ADJUSTED){   leds[ledSingle2] = CHSV( 100, 0, 255);  }
    FastLED.show();
  }
  else {
    ledSingle1 = random (LED_ADJUSTED);
    ledSingle2 = ledSingle1 + 1;
    ledSingle3 = ledSingle2 + 2;
    leds[ledSingle1] = CHSV( 100, 0, 255);
    FastLED.show();
    delay(random (20));
    if(ledSingle2 < LED_ADJUSTED){   leds[ledSingle2] = CHSV( 100, 0, 255); }
    FastLED.show();
    delay(random (20));
    if(ledSingle3 < LED_ADJUSTED){   leds[ledSingle3] = CHSV( 100, 0, 255); }
    FastLED.show();
  }

  for (int led = 0; led < LED_ADJUSTED; led++) {          leds[led] = CHSV( 100, 0, 0);      }  //turn off all LEDS
      FastLED.show();
      delay(random(10, 200));                                                               //random delay between 10-100
} 


//*******************************      State Modes    ******************************************//

void eepromSet() 
{ //newEpprom = 1;          // first run eprom data save
  newEpprom = EEPROM.read(0);          // first run eprom data save
  if (newEpprom != 73) {
    
    Serial.println("New EPROM!");
    // eeprom values:
    newEpprom = 73;
    remoteState = 1000;
    remotEeprom = 0;
    previousRemoteState = remoteState;
    butStateCounter = 1;                  //
    timeSpeed = 2;                            //
    Bvariable = 8;                          //
    FastLED.setBrightness(map(Bvariable,0,8,50,255)); // set master brightness control
    sensitivity = 3;                        //
    wheelPosition = 12;

    EEPROM.update(0, newEpprom);
    EEPROM.update(1, butStateCounter);
    EEPROM.update(2, timeSpeed);
    EEPROM.update(3, Bvariable);
    EEPROM.update(4, sensitivity);
    EEPROM.update(5, remotEeprom);
    EEPROM.update(6, wheelPosition);

  }

  else { 
    Serial.println("Old EPROM!");                           //not new eeprom
    butStateCounter =    (int)EEPROM.read(1);
    timeSpeed =          (int)EEPROM.read(2);
    Bvariable =          (int)EEPROM.read(3);
    sensitivity =        (int)EEPROM.read(4);
    remotEeprom =        (int)EEPROM.read(5);
    remoteState = BUTTON_ARRAY2[remotEeprom];  // 0 = BUTTON_1, 1 = BUTTON_4, 2 = BUTTON_2, 3 = BUTTON_3
    wheelPosition =      (int)EEPROM.read(6);

    Serial.print("remotEeprom :   ");
    Serial.println(remotEeprom);
    Serial.print("remoteState :   ");
    Serial.println(remoteState);
    Serial.print("butStateCounter :   ");
    Serial.println(butStateCounter);
    Serial.print("timeSpeed :   ");
    Serial.println(timeSpeed);
    Serial.print("sensitivity :   ");
    Serial.println(sensitivity);
    Serial.print("Bvariable :   ");
    Serial.println(Bvariable);
    Serial.print("wheelPosition :   ");
    Serial.println(wheelPosition);

    if (butStateCounter < 1 || butStateCounter > 4){    // safety in case bad eprom reading
      butStateCounter = 1;
    }
    if (sensitivity < 0 || sensitivity > 9){    // safety in case bad eprom reading
      sensitivity = 3;
    }
    if (Bvariable < 0 || Bvariable > 9){    // safety in case bad eprom reading
      Bvariable = 3;
    }
    if (remotEeprom < 0 || remotEeprom > 9){    // safety in case bad eprom reading
      remotEeprom = 0;
    }
    if (wheelPosition < 0 || wheelPosition > 12){    // safety in case bad eprom reading
      wheelPosition = 12;
    }
    FastLED.setBrightness(map(Bvariable,0,8,50,255)); // set master brightness control
  }
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

        if (buttonHeld >= 8) {                                   // Button Holds
             Serial.print("buttonHeld :    ");
             Serial.println(currentButton);
            
            if (currentButton == 'A') {
              //remoteState = BUTTON_1_HELD;
              reset();
            }
          }
        
      }
      else {
        for (int i = 0; i < 15; i++) 
        {              //compare against each of the button codes
          if (resultCode == BUTTON_ARRAY[i]) {

            // ACTUAL BUTTON
            newButtonPress = 1;

            if (resultCode == BUTTON_1) {
              currentButton = 'A';
            }
            else if (resultCode == BUTTON_4) {
              currentButton = 'D';
              flashCountA = 1;
            }
            else if (resultCode == BUTTON_2) {
              currentButton = 'B';
            }
            else if (resultCode == BUTTON_3) {
              currentButton = 'C';
            }
            else if (resultCode == BUTTON_5) {
              currentButton = 'E';
            }
            else if (resultCode == BUTTON_6) {
              currentButton = 'F';
            }
            else if (resultCode == BUTTON_8) {
              currentButton = 'H';
            }
            else if (resultCode == BUTTON_9) {
              currentButton = 'I';
            }
            else if (resultCode == BUTTON_AUP) {
              currentButton = 'J';
                upDownLeftRightRemote();
                remoteState = BUTTON_AUP;
              }
            else if (resultCode == BUTTON_ADOWN) {
              currentButton = 'P';
                upDownLeftRightRemote();
                remoteState = BUTTON_ADOWN;
              }

            else if (resultCode == BUTTON_BUP) {
              currentButton = 'K';
                upDownLeftRightRemote();
                remoteState = BUTTON_BUP;
              }
            else if (resultCode == BUTTON_BDOWN) {
              currentButton = 'Q';
                upDownLeftRightRemote();
                remoteState = BUTTON_BDOWN;
              }

            else if (resultCode == BUTTON_CUP) {
              currentButton = 'L';
                if (wheelPosition < 12){       
                  wheelPosition++;  
                  EEPROM.update(6, wheelPosition);  // EEPROM Save 
                }
                else {                         
                  wheelPosition = 0; 
                  EEPROM.update(6, wheelPosition);  // EEPROM Save
                  }
              upDownLeftRightRemote();
              remoteState = BUTTON_CUP;
              }
            else if (resultCode == BUTTON_CDOWN) {
                currentButton = 'R';
                if (wheelPosition > 0){        
                  wheelPosition--;   
                  EEPROM.update(6, wheelPosition);  // EEPROM Save
                  }
                else {                        
                  wheelPosition = 12; 
                  EEPROM.update(6, wheelPosition);  // EEPROM Save
                  }
                upDownLeftRightRemote();
                remoteState = BUTTON_CDOWN;
              }

            else if (resultCode == BUTTON_7) {
              currentButton = 'G';
              flashCountA = 1;
            }

            
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

        if (buttonHeld > 8) { // number of seconds/ 4-1       // Button Holds
          // Current Button Held
          
        }


        else {                                         // Button Single Presses
            // Current Button
            Serial.println(currentButton);

            if (currentButton == 'A') {
              EEPROM.update(5, 0);  // EEPROM Save 0 = BUTTON_1
              previousRemoteState = remoteState;
              remoteState = BUTTON_1;
              flashCount = 1;
              Serial.println("Off");
              turnoffLEDs();
              FastLED.show();
            }
            else if (currentButton == 'D') {
              EEPROM.update(5, 3);  // EEPROM Save 1 = BUTTON_4
              previousRemoteState = remoteState;
              remoteState = BUTTON_4;
              flashCount = 1;
              turnoffLEDs();
            }
            else if (currentButton == 'B') {
              EEPROM.update(5, 1);  // EEPROM Save 2 = BUTTON_2
              previousRemoteState = remoteState;
              remoteState = BUTTON_2;
              flash(100,0); 
              turnoffLEDs();
              FastLED.show(); 
            }
            else if (currentButton == 'C') {
              EEPROM.update(5, 2);  // EEPROM Save 3 = BUTTON_3
              previousRemoteState = remoteState;
              remoteState = BUTTON_3;
              turnoffLEDs();
            }
            else if (currentButton == 'G') {
              EEPROM.update(5, 6);  // EEPROM Save 4 = BUTTON_7
              previousRemoteState = remoteState;
              remoteState = BUTTON_7;
              flash(100,0); 
              turnoffLEDs();
              FastLED.show(); 
            }
            else if (currentButton == 'H') {
              previousRemoteState = remoteState;
              remoteState = BUTTON_8;
              EEPROM.update(5, 7);  // EEPROM Save 5 = BUTTON_8
              flash(100,0); 
              turnoffLEDs();
              FastLED.show(); 
            }
            else if (currentButton == 'I') {
              previousRemoteState = remoteState;
              remoteState = BUTTON_9;
              EEPROM.update(5, 8);  // EEPROM Save 6 = BUTTON_9
              turnoffLEDs();
            }
            else if (currentButton == 'E') {
              previousRemoteState = remoteState;
              remoteState = BUTTON_5;
              EEPROM.update(5, 4);  // EEPROM Save 7 = BUTTON_5
              flash(100,0); 
              turnoffLEDs();
              FastLED.show(); 
            }
            else if (currentButton == 'F') {
              previousRemoteState = remoteState;
              remoteState = BUTTON_6;
              EEPROM.update(5, 5);  // EEPROM Save 8 = BUTTON_6
              turnoffLEDs();
            }
        }
        //Serial.println("Preparing");
        //prepareModes();               // load in startup values for each mode and run preview
        //Serial.println("Prepaired");
        buttonHeld = 0;

      }

      //find the relevant remoteState value within BUTTON_ARRAY - save to eeprom as remotEeprom
      // for (int x = 0; x < 3; x++){
      //     if(remoteState == BUTTON_ARRAY[x]){
      //       remotEeprom = x;
      //  }
      //  if (remotEeprom != prevRemotEeprom){
      //     EEPROM.update(5, remotEeprom);
      //     prevRemotEeprom = remotEeprom;
      //  }
      // }
      

    }
    checking = millis();
  }
}


void upDownLeftRightReturn() 
{      //go back into prev mode

  variableState = 1;
  remoteState = previousRemoteState;
  variableMillis = millis();
  turnoffLEDs();
  FastLED.show();
}

void upDownLeftRightRemote() 
{

  variableState = 0;

  if ((remoteState != BUTTON_CDOWN) && (remoteState != BUTTON_CUP) && (remoteState != BUTTON_AUP) && (remoteState != BUTTON_ADOWN) && (remoteState != BUTTON_BUP) && (remoteState != BUTTON_BDOWN))
  {
    if (remoteState == 1000)
    {
      previousRemoteState = BUTTON_1;
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

void reset()
{
  Serial.print("RESET!");
  fill_solid( leds, LED_ADJUSTED, CHSV(0, 0, 0));
  FastLED.show();
  delay(100);
  fill_solid( leds, LED_ADJUSTED, CHSV(255, 255, 200));
  FastLED.show();
  delay(1000);
  fill_solid( leds, LED_ADJUSTED, CHSV(0, 0, 0));
  FastLED.show();
  delay(300);
  fill_solid( leds, LED_ADJUSTED, CHSV(255, 255, 200));
  FastLED.show();
  delay(1000);
  fill_solid( leds, LED_ADJUSTED, CHSV(0, 0, 0));
  FastLED.show();
  newEpprom = 1;
  EEPROM.update(0, newEpprom);
  eepromSet();
      
}

