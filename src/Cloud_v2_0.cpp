
/*
  Cloud 2.0
  2018 Richard Clarkson Studio
*/

//#define FASTLED_FORCE_SOFTWARE_SPI 1
#include <Arduino.h>
#include <FastLED.h>
#include <Audio.h>   
#include <EEPROM.h>
#include <IRremote.h>

FASTLED_USING_NAMESPACE



#define NUM_LEDS 17        //     S=10 M=17 L=25 H=50




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

const uint16_t BUTTON_B = 0x7887;
const uint16_t BUTTON_B_HELD = 21;

const uint16_t BUTTON_C = 0x58A7;
const uint16_t BUTTON_C_HELD = 22;

const uint16_t BUTTON_UP = 0xA05F;
const uint16_t BUTTON_DOWN = 0x00FF;
const uint16_t BUTTON_LEFT = 0x10EF;
const uint16_t BUTTON_RIGHT = 0x807F;

const uint16_t BUTTON_CIRCLE = 0x20DF;  // hex = 4815
const uint16_t BUTTON_CIRCLE_HELD = 24;

const uint16_t BUTTON_HELD = 0xFFFF;


// const uint16_t BUTTON_POWER = 0xE01F; //0xD827; // i.e. 0x10EFD827  //55335
// const uint16_t BUTTON_POWER_HELD = 23;

// const uint16_t BUTTON_A = 0x00FF; //0xF807;
// const uint16_t BUTTON_A_HELD = 20;

// const uint16_t BUTTON_B = 0xC03F; //0x7887;
// const uint16_t BUTTON_B_HELD = 21;

// const uint16_t BUTTON_C = 0x807F; //0x58A7;
// const uint16_t BUTTON_C_HELD = 22;

// const uint16_t BUTTON_UP = 0x10EF; //0xA05F;
// const uint16_t BUTTON_DOWN = 0x30CF; //0x00FF;
// const uint16_t BUTTON_LEFT = 0x9867; //0x10EF;
// const uint16_t BUTTON_RIGHT = 0x708F; //0x807F;

// const uint16_t BUTTON_CIRCLE = 0xA05F; //0x20DF;  // hex = 4815
// const uint16_t BUTTON_CIRCLE_HELD = 24;

// const uint16_t BUTTON_HELD = 0xFFFF;

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
//int thistimer;

//Lamp Mode Variables
int rainbowCounter = 0;

uint8_t gHue = 180;           // rotating "base color" used by many of the patterns

//LED Variables
#define DATA_PIN 4 //MOSI  //7 Green
#define CLK_PIN 3  //SCK  //14 Blue
#define LED_TYPE WS2801 //APA102
#define COLOR_ORDER RGB
CRGB leds[NUM_LEDS];
//#define FRAMES_PER_SECOND 120

int minLEDvalue[NUM_LEDS];
int goingUp[NUM_LEDS];
int currentValue[NUM_LEDS];

int currentValueFade = 15;
int goingUpFade = 1;

int ledSingle1;
int ledSingle2;
int ledSingle3;

//TAP HOLD Varriables 
//uint8_t buttonState;

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
  irrecv.enableIRIn(); // Start the receiver
  AudioMemory(12);
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  //FastLED.addLeds<LED_TYPE, DATA_PIN, CLK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE, DATA_PIN, CLK_PIN, COLOR_ORDER, DATA_RATE_MHZ(2)>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(255);
  turnoffLEDs();
  FastLED.show();
  turnoffLEDs();
  FastLED.show();
  Serial.begin(9600);
  delay(1000);  // Sanity Delay
  turnoffLEDs();
  FastLED.show();
  for (int i = 0; i < NUM_LEDS; i++) {    
    minLEDvalue[i] = random(1,150);       //fill up the minimum LED value array for Fairy Light Mode
    currentValue[i] = random(1,254);      //fill up the current value array for Fairy Light Mode
    goingUp[i] = random(0,1);             //fill up the going up value array for Fairy Light Mode
  }
  for (int i = 0; i < 1000; i++) {
    for (int x = 0; x < NUM_LEDS; x++) {
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
  // while (!Serial) {
  //   ; // wait for serial port to connect. Needed for native USB
  // }
  Serial.println("Cloud v2.0");
  //EEPROM.update(0, 1);       // uncomment to load default EPROM values
  eepromSet();
    // butStateCounter = 1;                  //
    // timeSpeed = 2;                          //
    // Bvariable = 8;                          //
    // sensitivity = 4;                        //
    // remoteState = BUTTON_B;
}



void loop()
{ 
  remote();

  if      (remoteState == BUTTON_POWER){
     if (flashCount == 1){  
       flash(100,0); 
       turnoffLEDs();
       FastLED.show();  
       }
    //delay(3);
  }
  else if (remoteState == BUTTON_POWER_HELD){
      //reset();
  }
  else if (remoteState == BUTTON_A){
    if      (butStateCounter == 1){    
      if (flashCount == 1){ flash(130,250);  }
      fetchSoundData();
      soundLightening();     
      }
    else if (butStateCounter == 2){    
      demo();    
      }
    else if (butStateCounter == 3){    
     if (flashCount == 1){  
       flash(130,250);
       strom();  
       }
      timedLightening(30);    
    }
    else if (butStateCounter == 4){    
     if (flashCount == 1){  
       flash(130,250);
       strom();    
       }
     timedLightening(240);    
     }
  }
  else if (remoteState == BUTTON_A_HELD){
    //fetchSoundData();
    //musicmode1();
  }
  else if (remoteState == BUTTON_B){
    if      (butStateCounter == 1){    
      fetchSoundData();
      musicmode4();     
      }
    else if (butStateCounter == 2){    
      analyzeFFTall(); 
      musicmode5();     
      }
    else if (butStateCounter == 3){    
      fetchSoundData(); 
      musicmode1();     
    }
    else if (butStateCounter == 4){    
      analyzeFFTall(); ;
      musicmode3();     
      }
  }
  else if (remoteState == BUTTON_B_HELD){
    //analyzeFFTall(); 
    //musicmode3();
  }
  else if (remoteState == BUTTON_C){
    if      (butStateCounter == 1){    
      lampMode2();    
      }
    else if (butStateCounter == 2){    
      lampMode1();    
      }
    else if (butStateCounter == 3){    
      lampMode3();   
    }
    else if (butStateCounter == 4){    
      lampMode4();     
      }
  }
  else if (remoteState == BUTTON_C_HELD){
    //lampMode3();
  }
  else if (remoteState == BUTTON_UP)
  {
    if(millis() - variableMillis > 1000){
        upDownLeftRightReturn();
     }
      if(variableState == 0){
        if (previousRemoteState == BUTTON_A) {     // Global brightness
          if (Bvariable < 8){        Bvariable++;   }
          FastLED.setBrightness(map(Bvariable,0,8,20,255));
          fill_solid( leds, NUM_LEDS, CHSV(60,150,(map(Bvariable,0,8,20,250))));
          FastLED.show();
          Serial.print("Bvariable = ");
          Serial.println(Bvariable);
          EEPROM.update(3, Bvariable);
        }
        else if (previousRemoteState == BUTTON_B) {     // sensitivity
          if (sensitivity < 8){     sensitivity++;  }
          fill_solid( leds, NUM_LEDS, CHSV(180,150,(map(sensitivity,0,8,20,250))));
          FastLED.show();
          Serial.print("sensitivity = ");
          Serial.println(sensitivity);
          EEPROM.update(4, sensitivity);
        }
        else if (previousRemoteState == BUTTON_C) {     // Speed of Animation
          if (timeSpeed < 5){          timeSpeed++;     }
          fill_solid( leds, NUM_LEDS, CHSV(110,150,(map(timeSpeed,0,5,20,250))));
          FastLED.show();
          Serial.print("timeSpeed = ");
          Serial.println(speedOfAnimation[timeSpeed]);
          EEPROM.update(2, timeSpeed);
        }
        variableState = 1;
    }    
  }
  else if (remoteState == BUTTON_DOWN)
  {
    if(millis() - variableMillis > 1000){
        upDownLeftRightReturn();
     }
      if(variableState == 0){
        if (previousRemoteState == BUTTON_A) {     // Global brightness
          if (Bvariable > 0){     Bvariable--;   }
          FastLED.setBrightness(map(Bvariable,0,8,20,255));
          fill_solid( leds, NUM_LEDS, CHSV(60,150,(map(Bvariable,0,8,20,250))));
          FastLED.show();
          Serial.print("Bvariable = ");
          Serial.println(Bvariable);
          EEPROM.update(3, Bvariable);
        }
        else if (previousRemoteState == BUTTON_B) {     // sensitivity
          if (sensitivity > 0){     sensitivity--;  }
          fill_solid( leds, NUM_LEDS, CHSV(180,150,(map(sensitivity,0,8,20,250))));
          FastLED.show();
          Serial.print("sensitivity = ");
          Serial.println(sensitivity);
          EEPROM.update(4, sensitivity);
        }
        else if (previousRemoteState == BUTTON_C) {     // Speed of Animation
          if (timeSpeed > 0){          timeSpeed--;     }
          fill_solid( leds, NUM_LEDS, CHSV(110,150,(map(timeSpeed,0,5,20,250))));
          FastLED.show();
          Serial.print("timeSpeed = ");
          Serial.println(speedOfAnimation[timeSpeed]);
          EEPROM.update(2, timeSpeed);
        }
        variableState = 1;
    }    
  }
  else if ((remoteState == BUTTON_RIGHT) || (remoteState == BUTTON_LEFT))
  {
    for (int x = 0; x < NUM_LEDS; x++) {
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
  if (soundLevel > 250){
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
  //fadeToBlackBy( leds, NUM_LEDS, 3);
  turnoffLEDs();

  soundLevel = (map(soundLevel,0,150,0,NUM_LEDS+1))-1;    //250
  if (soundLevel > dot)  dot = soundLevel; // Keep dot on top of soundLevel
  if (dot > NUM_LEDS)    dot = NUM_LEDS; // Keep dot from going out of frame

  //turnoffLEDs();
  if (soundLevel > 1){
  for (int led = 0; led < dot; led++)
  { // Start by Filling LEDS up to the soundLevel with dim white
    leds[led] = CHSV( 100, 0, 255);
   } 
  }
  //leds[dot] = CHSV( 100, 150, 255);
  
  // for (int led = dot; led < NUM_LEDS; led++)
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
  soundLevel = map(soundLevel, 0, 255,0, NUM_LEDS);              // limit
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
  //fadeToBlackBy( leds, NUM_LEDS, 1);
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
    int rippleBrightness = fftArray[y] * 5;    // fftArray[y]
    //ippleBrightness = map( rippleBrightness, 0, 50, 0, 255 );    // fftArray[y]
    rippleBrightness = constrain( rippleBrightness, 0, 255 );    // fftArray[y]
    for (int led = bottomOfRipple; led < topOfRipple; led++)
    {
      leds[led] = CHSV(wheelH[wheelPosition], wheelS[wheelPosition], rippleBrightness); // fill in LEDs according to the top and bottom of each section deffined above
    }
  }
  FastLED.show();
}

void musicmode4()   // Fade
{ 
  if (soundLevel * 3 > dot){    dot = soundLevel * 3;  } // Keep dot on top of soundLevel
  if (dot > 255){    dot = 255;  }// Keep dot from going out of frame

  for (int led = 0; led < NUM_LEDS; led++)
  {
    leds[led] = CHSV( wheelH[wheelPosition], wheelS[wheelPosition], dot);
  }
  FastLED.show();
  if (++dotCount >= 5) {                   // make the dot fall slowly
    dotCount = 0;
    if (dot > 0) {
      dot--;
    }
  }
}

void musicmode5()     // Colorful  
{   
  total = total - readings[readIndex];   // subtract the last reading:
  readings[readIndex] = constrain((fftArray[0] + fftArray[1]),0,255);   // read from the sensor:
  total = total + readings[readIndex];   // add the reading to the total:
  readIndex = readIndex + 1;   // advance to the next position in the array:
  if (readIndex >= numReadings) { readIndex = 0; }     // if we're at the end of the array wrap around to the beginning:
  average = total / numReadings;   // calculate the average:

  totalM = totalM - readingsM[readIndexM];   // subtract the last reading:
  readingsM[readIndexM] = constrain((fftArray[2] + fftArray[3]),0,255);   // read from the sensor:
  totalM = totalM + readingsM[readIndexM];   // add the reading to the total:
  readIndexM = readIndexM + 1;   // advance to the next position in the array:
  if (readIndexM >= numReadings) { readIndexM = 0; }     // if we're at the end of the array wrap around to the beginning:
  averageM = totalM / numReadings;   // calculate the average:

  totalH = totalH - readingsH[readIndexH];   // subtract the last reading:
  readingsH[readIndexH] = constrain((fftArray[4] + fftArray[5] + fftArray[6] + fftArray[7]),0,255);   // read from the sensor:
  totalH = totalH + readingsH[readIndexH];   // add the reading to the total:
  readIndexH = readIndexH + 1;   // advance to the next position in the array:
  if (readIndexH >= numReadings) { readIndexH = 0; }     // if we're at the end of the array wrap around to the beginning:
  averageH = totalH / numReadings;   // calculate the average:

    //Bass = constrain((fftArray[0] + fftArray[1])*2,2,255);
    //Mid = constrain((fftArray[2] + fftArray[3])*2,2,255);
    //High = constrain((fftArray[4] + fftArray[5] + fftArray[6] + fftArray[7])*2,2,255);

    // if (Bass < prevBass){ 
    //   Bass = prevBass - 1;
    //   if(prevBass > 3){
    //     prevBass--;
    //   }
    //  }
    // else {
    //   prevBass = Bass;
    // }

    // if (Mid < prevMid){ 
    //   Mid = prevMid - 1;
    //   if(prevMid > 3){
    //     prevMid--;
    //   }
    //  } 
    // else {
    //   prevMid = Mid;
    // }

    // if (High < prevHigh){ 
    //   High = prevHigh - 1;
    //   if(prevHigh > 3){
    //     prevHigh--;
    //   }
    //  }
    // else {
    //   prevHigh = High;
    // }

    for (int x = 0; x < NUM_LEDS; x++) {
    leds[x].setRGB(averageH, averageM, average);
  }
  FastLED.show();
  //delay(10);
}

//*******************************      Lamp Modes    ******************************************//

void turnoffLEDs()
{
    for (int led = 0; led < NUM_LEDS; led++)        
  { //turn off LEDs
    leds[led] = CHSV( 100, 0, 0);
  }
}

void flash(int hue, int saturation)
{
     for (int led = 0; led < NUM_LEDS; led++) {
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
  rainbow(0, NUM_LEDS, 0.1);
  //rainbow(0, NUM_LEDS, 0.1);
  FastLED.show();
}

void lampMode2()  // Fairy Light
{
    EVERY_N_MILLISECONDS_I(thistimer,10) {
    thistimer.setPeriod(speedOfAnimation[timeSpeed]);
    for (int x = 0; x < NUM_LEDS; x++) {
    if(goingUp[x] == 1){
      currentValue[x]++;
      if (currentValue[x] >= 255) {goingUp[x] = 0;}
    }
    else{
      currentValue[x]--;
      if (currentValue[x] <= minLEDvalue[x]) {goingUp[x] = 1;}
    }
    leds[x] = CHSV( wheelH[wheelPosition], wheelS[wheelPosition], currentValue[x]);
  }
  rainbowCounter = 0;
 }
 FastLED.show();
}

void lampMode3()  // Ombre
{
  rainbow(0, NUM_LEDS, 5);
  //rainbow(0, NUM_LEDS, 1);
  FastLED.show();
}

void lampMode4()  // Breathing Light
{
  EVERY_N_MILLISECONDS_I(thistimer,10) {
    thistimer.setPeriod(speedOfAnimation[timeSpeed]);
    if(goingUpFade == 1){
      currentValueFade++;
      if (currentValueFade >= 255) {goingUpFade = 0;}
    }
    else{
      currentValueFade--;
      if (currentValueFade <= 10) {goingUpFade = 1;}
    }
    for (int x = 0; x < NUM_LEDS; x++) {
      leds[x] = CHSV(wheelH[wheelPosition], wheelS[wheelPosition], currentValueFade);
    }
    rainbowCounter = 0;
  }
  FastLED.show();
}

void rainbow(int startPos, int number, float deltaHue) 
{
    if (++rainbowCounter >= speedOfAnimation[timeSpeed] * 3) {
      gHue++;
      rainbowCounter = 0;
    } // slowly cycle the "base color" through the rainbow
    fill_rainbow( &(leds[startPos]), number, gHue, deltaHue);
    //FastLED.show();
}

void strom()
{
  for (int led = 0; led < int(random(NUM_LEDS)); led++) {
        leds[led] = CHSV( 100, 0, 255);
        FastLED.show();
      }

  for (int led = 0; led < int(random(NUM_LEDS)); led++) {  leds[led] = CHSV( 100, 0, 255);    }
      FastLED.show();
      delay(random(10, 40));
  for (int led = 0; led < int(random(NUM_LEDS)); led++) {  leds[led] = CHSV( 100, 0, 0);      }
      FastLED.show();
      delay(random(10, 200));
  for (int led = 0; led < NUM_LEDS; led++) {          leds[led] = CHSV( 100, 0, 0);      }
      FastLED.show();
  
  for (int led = 0; led < int(random(NUM_LEDS)); led++) {  leds[led] = CHSV( 100, 0, 255);    }
      FastLED.show();
      delay(random(10, 50));
  for (int led = 0; led < int(random(NUM_LEDS)); led++) {  leds[led] = CHSV( 100, 0, 0);      }
      FastLED.show();
      delay(random(10, 100));

  for (int led = 0; led < NUM_LEDS; led++) {          leds[led] = CHSV( 100, 0, 0);      }
      FastLED.show();

  ledSingle1 = random(NUM_LEDS);
  leds[ledSingle1] = CHSV( 100, 0, 255);
  FastLED.show();
  delay(random (10, 100));
  leds[ledSingle1] = CHSV( 100, 0, 0);
  FastLED.show();
  leds[ledSingle1] = CHSV( 100, 0, 255);
  FastLED.show();
  delay(random (40, 100));
  leds[ledSingle1] = CHSV( 100, 0, 0);
  FastLED.show();

  if (random(1, 3) == 2) {
    ledSingle1 = random (NUM_LEDS);
    ledSingle2 = random (50);
    leds[ledSingle1] = CHSV( 100, 0, 255);
    if(ledSingle2 < NUM_LEDS){   leds[ledSingle2] = CHSV( 100, 0, 255);  }
    FastLED.show();
  }
  else {
    ledSingle1 = random (NUM_LEDS);
    ledSingle2 = ledSingle1 + 1;
    ledSingle3 = ledSingle2 + 2;
    leds[ledSingle1] = CHSV( 100, 0, 255);
    FastLED.show();
    delay(random (20));
    if(ledSingle2 < NUM_LEDS){   leds[ledSingle2] = CHSV( 100, 0, 255); }
    FastLED.show();
    delay(random (20));
    if(ledSingle3 < NUM_LEDS){   leds[ledSingle3] = CHSV( 100, 0, 255); }
    FastLED.show();
  }

  for (int led = 0; led < NUM_LEDS; led++) {          leds[led] = CHSV( 100, 0, 0);      }
      FastLED.show();

  if (random(1, 3) == 2) {
    ledSingle1 = random (NUM_LEDS);
    ledSingle2 = random (50);
    leds[ledSingle1] = CHSV( 100, 0, 255);
    if(ledSingle2 < NUM_LEDS){   leds[ledSingle2] = CHSV( 100, 0, 255);  }
    FastLED.show();
  }
  else {
    ledSingle1 = random (NUM_LEDS);
    ledSingle2 = ledSingle1 + 1;
    ledSingle3 = ledSingle2 + 2;
    leds[ledSingle1] = CHSV( 100, 0, 255);
    FastLED.show();
    delay(random (20));
    if(ledSingle2 < NUM_LEDS){   leds[ledSingle2] = CHSV( 100, 0, 255); }
    FastLED.show();
    delay(random (20));
    if(ledSingle3 < NUM_LEDS){   leds[ledSingle3] = CHSV( 100, 0, 255); }
    FastLED.show();
  }

  for (int led = 0; led < NUM_LEDS; led++) {          leds[led] = CHSV( 100, 0, 0);      }
      FastLED.show();
      delay(random(10, 200));
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
    remoteState = BUTTON_ARRAY[remotEeprom];  // 0 = BUTTON_POWER, 1 = BUTTON_A, 2 = BUTTON_B, 3 = BUTTON_C
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


        if (currentButton == 'L') {
          if (wheelPosition < 12){        
            wheelPosition++;    
            EEPROM.update(6, wheelPosition);  // EEPROM Save
            }
          else {                          
            wheelPosition = 0;
            EEPROM.update(6, wheelPosition);  // EEPROM Save  
            }
        }
        else if (currentButton == 'R') {
          if (wheelPosition > 0){         
            wheelPosition--;
            EEPROM.update(6, wheelPosition);  // EEPROM Save    
            }
          else {                          
            wheelPosition = 12; 
            EEPROM.update(6, wheelPosition);  // EEPROM Save
            }
        }
        // else if (currentButton == 'U') {
        //   upDownLeftRightRemoteHeld();
        //   remoteState = BUTTON_UP;
        // }
        // else if (currentButton == 'D') {
        //   upDownLeftRightRemoteHeld();
        //   remoteState = BUTTON_DOWN;
        // }

        else {
          if (buttonHeld >= 4) {                                   // Button Holds
             Serial.print("buttonHeld :    ");
             Serial.println(currentButton);

            if (currentButton == 'O') {
              remoteState = BUTTON_CIRCLE_HELD;
            }
            else if (currentButton == 'A') {
              //previousRemoteState = remoteState;
              //remoteState = BUTTON_A_HELD;
            }
            else if (currentButton == 'B') {
              //previousRemoteState = remoteState;
              //remoteState = BUTTON_B_HELD;
            }
            else if (currentButton == 'P') {
              //remoteState = BUTTON_POWER_HELD;
              reset();
            }
            else if (currentButton == 'C') {
              //previousRemoteState = remoteState;
              //remoteState = BUTTON_C_HELD;
            }
          }
        }
      }
      else {
        for (int i = 0; i < 9; i++) 
        {              //compare against each of the button codes
          if (resultCode == BUTTON_ARRAY[i]) {

            // ACTUAL BUTTON
            newButtonPress = 1;

            if (resultCode == BUTTON_POWER) {
              currentButton = 'P';
            }
            else if (resultCode == BUTTON_A) {
              currentButton = 'A';
              flashCount = 1;
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
                if (wheelPosition < 12){       
                  wheelPosition++;  
                  EEPROM.update(6, wheelPosition);  // EEPROM Save 
                }
                else {                         
                  wheelPosition = 0; 
                  EEPROM.update(6, wheelPosition);  // EEPROM Save
                  }
              upDownLeftRightRemote();
              remoteState = BUTTON_LEFT;
              }
            else if (resultCode == BUTTON_RIGHT) {
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
                remoteState = BUTTON_RIGHT;
                
              }
            else if (resultCode == BUTTON_CIRCLE) {
              //currentButton = 'O';
              strom();
              strom();
              strom();
              //remoteState = previousRemoteState;
              //if(butStateCounter != 1){ butStateCounter--; }
              newButtonPress = 0;
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

        if (buttonHeld > 4) { // number of seconds/ 4-1       // Button Holds
          // Current Button Held

          
        }


        else {                                         // Button Single Presses
            // Current Button
            Serial.println(currentButton);

            if (currentButton == 'P') {
              EEPROM.update(5, 0);  // EEPROM Save 0 = BUTTON_POWER
              previousRemoteState = remoteState;
              remoteState = BUTTON_POWER;
              flashCount = 1;
              Serial.println("Off");
              turnoffLEDs();
              FastLED.show();
            }
            else if (currentButton == 'A') {
              EEPROM.update(5, 1);  // EEPROM Save 1 = BUTTON_A
              previousRemoteState = remoteState;
              remoteState = BUTTON_A;
              if (previousRemoteState == remoteState){ 
                if (butStateCounter < 4){     butStateCounter++;    }
                else {      butStateCounter = 1;     }
              }
              else {        butStateCounter = 1;     }
              Serial.print("butStateCounter:");
              Serial.println(butStateCounter);
              EEPROM.update(1, butStateCounter);
            }
            else if (currentButton == 'B') {
              EEPROM.update(5, 2);  // EEPROM Save 2 = BUTTON_B
              previousRemoteState = remoteState;
              remoteState = BUTTON_B;
              if (previousRemoteState == remoteState){ 
                if (butStateCounter < 4){     butStateCounter++;    }
                else {      butStateCounter = 1;     }
              }
              else {        butStateCounter = 1;     }
              Serial.print("butStateCounter:");
              Serial.println(butStateCounter);
              EEPROM.update(1, butStateCounter);
            }
            else if (currentButton == 'C') {
              EEPROM.update(5, 3);  // EEPROM Save 3 = BUTTON_C
              previousRemoteState = remoteState;
              remoteState = BUTTON_C;
              if (previousRemoteState == remoteState){ 
                if (butStateCounter < 4){     butStateCounter++;    }
                else {      butStateCounter = 1;     }
              }
              else {        butStateCounter = 1;     }
              Serial.print("butStateCounter:");
              Serial.println(butStateCounter);
              EEPROM.update(1, butStateCounter);
            }
            else if (currentButton == 'O') {
            //previousRemoteState = remoteState;
          
            //remoteState = previousRemoteState;
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

void reset()
{
  Serial.print("RESET!");
  fill_solid( leds, NUM_LEDS, CHSV(0, 0, 0));
  FastLED.show();
  delay(100);
  fill_solid( leds, NUM_LEDS, CHSV(255, 255, 200));
  FastLED.show();
  delay(1000);
  fill_solid( leds, NUM_LEDS, CHSV(0, 0, 0));
  FastLED.show();
  delay(300);
  fill_solid( leds, NUM_LEDS, CHSV(255, 255, 200));
  FastLED.show();
  delay(1000);
  fill_solid( leds, NUM_LEDS, CHSV(0, 0, 0));
  FastLED.show();
  newEpprom = 1;
  EEPROM.update(0, newEpprom);
  eepromSet();
      
}

