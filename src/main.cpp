/*

 Cloud by Richard Clarkson Studio, LLC.

 Version 1 - 5 by Richard Clarkson 2013
 Version 6 by Oscar de la Hera Gomez 2015
 Version 7 by Richard Clarkson 2016

*/


//#define SUPPRESS_ERROR_MESSAGE_FOR_BEGIN

// include libraries
#include <Arduino.h>
#include <SD.h>
#include <Audio.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_WS2801.h>
#include <IRremote.h>
#include <EEPROM.h>



/* Setup constants for SparkFun's IR Remote: */
#define NUM_BUTTONS 9 // The remote has 9 buttons
/* Define the IR remote button codes. We're only using the
 least signinficant two bytes of these codes. Each one
 should actually have 0x10EF in front of it. Find these codes
 by running the IRrecvDump example sketch included with
 the IRremote library.*/

//*******************************       PIN CONSTANTS    ******************************************//

int RECV_PIN = 15;       //   

int motion_1 = 5;       // 

uint8_t dataPin  = 4;    // Green wire on Adafruit Pixels  
uint8_t clockPin = 3;    // Blue wire on Adafruit Pixels   


//*******************************       PIXEL CONSTANTS    ******************************************//


// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row

#define N_PIXELS 25    

Adafruit_WS2801 strip = Adafruit_WS2801(N_PIXELS, dataPin, clockPin);



//*******************************       BUTTON CONSTANTS    ******************************************//

const uint16_t BUTTON_POWER = 0xD827; // i.e. 0x10EFD827  //55335
const uint16_t BUTTON_POWER_HELD = 23;

const uint16_t BUTTON_A = 0xF807;
const uint16_t BUTTON_A_HELD = 20;

const uint16_t BUTTON_B = 0x7887;
const uint16_t BUTTON_B_HELD = 21;

const uint16_t BUTTON_C = 0x58A7;
const uint16_t BUTTON_C_HELD = 22;

const uint16_t BUTTON_AUP = 0xA05F;
const uint16_t BUTTON_ADOWN = 0x00FF;

const uint16_t BUTTON_BUP = 0xC03F;
const uint16_t BUTTON_BDOWN = 0x40BF;

const uint16_t BUTTON_CUP = 0x10EF;
const uint16_t BUTTON_CDOWN = 0x807F;

const uint16_t BUTTON_CIRCLE = 0x20DF;  // hex = 4815
const uint16_t BUTTON_CIRCLE_HELD = 24;

const uint16_t BUTTON_HELD = 0xFFFF;

uint16_t BUTTON_ARRAY[11] = {BUTTON_POWER, BUTTON_A, BUTTON_B, BUTTON_C, BUTTON_CIRCLE, BUTTON_AUP, BUTTON_ADOWN, BUTTON_BUP, BUTTON_BDOWN, BUTTON_CUP, BUTTON_CDOWN};
uint16_t BUTTON_ARRAY2[9] = {BUTTON_POWER, BUTTON_A, BUTTON_B, BUTTON_C, BUTTON_CIRCLE, BUTTON_CIRCLE_HELD, BUTTON_A_HELD, BUTTON_B_HELD, BUTTON_C_HELD,};


//*******************************       IR CONSTANTS    ******************************************//

long checking = 0;
int newButtonPress = 0;
char currentButton;

//*******************************       MUSIC CONSTANTS    ******************************************//

int shouldflashcolor = 1;
unsigned int sample;

//*******************************       GLOBAL VARIABLES   *******************************************//

int firstRun = 0;

int strikeCounter = 0;

int rvalueShift;    //255
int gvalueShift;    //241
int bvalueShift;    //224

int firststorm = 0;
int shiftColor;

int yellowFrequncy;

int randomstriketrigger;   //5
int oldRandomstrikeTrigger;

unsigned long intervalSensor = 1501;

unsigned long previousMillisSensor = 0;

int sensor_1 = 0;

int ledSingle1 = 0;
int ledSingle2;
int ledSingle3;


int upupup = 0;
int fadeval = 255;
int delaycount = 0;

//int minLEDvalue[N_PIXELS];
//int goingUp[N_PIXELS];
//int currentValue[N_PIXELS];


int currentValueFade = 15;
int goingUpFade = 1;

int fadeout;

int sensorReading = 0;

int randomiser;

int lampMode = 1;
int firstPassLamp = 1;
int shouldIflash = 1;

unsigned long doNothingInterval = 10;
unsigned long currentDoNothingMillis;
unsigned long previousDoNothingMillis;

unsigned long variableMillis = millis();

int variableState = 0;

//int locked;                           // WHEN CLOUD IS UNLOCKED; LOCKED = 0; WHEN LOCKED, LOCKED =1;

int speedOfDelay;
int oldSpeedOfDelay = 10;

int cStateVariable = 1;

int colourVariable = 20;

int stormCounter = 100;

int teensyMappingBassToLeds = 0;

int higherValue;

//int indicatorFlash = 75 - (2 * N_PIXELS);   //number ranges from 10 to 35

int bass;
int mid;
int high;
int prevBass = 0;
int prevMid = 0;
int prevHigh = 0;
int bassMid;
int allchannels;
int dropCounter = 0;

int flashCount = 1;
int flashCountA = 1;
int remotEeprom;


#define TOP       (N_PIXELS + 2) // Allow dot to go slightly off scale

byte
  peak      = 0,      // Used for falling dot
  dotCount  = 0,      // Frame counter for delaying dot-falling speed
  volCount  = 0,      // Frame counter for storing past volume data
  peakMid   = 0;




//*******************************       MODE VARIABLES   *******************************************//

int buttonACase = 0;
int buttonBCase = 0;
int buttonCCase = 0;
uint16_t currentMode;

//**************************       MIC VARIABLES & CONSTANTS    *************************************//


const int myInput = AUDIO_INPUT_MIC;


//*************************       REMOTE VARIABLES & CONSTANTS    ************************************//

uint16_t resultCode;
int buttonHeld = 0;
int remoteState;
int stateCounter = 0;

int previousRemoteState;


int mode;

int prevButtonHeld;

//*************************************      TEENSY AUDIO SHIELD   **********************************************//

AudioPlaySdWav           playWav1;
AudioOutputI2S           i2s1;

AudioInputI2S            audioInput;         // audio shield: mic or line-in
AudioAnalyzeFFT256       myFFT;

AudioConnection          patchCord1(playWav1, 0, i2s1, 0);
AudioConnection          patchCord2(playWav1, 1, i2s1, 1);

AudioConnection          patchCord3(audioInput, myFFT);


AudioControlSGTL5000     audioShield;




//*************************************      AUDIO SHIELD  CONSTANTS  **********************************************//

float level[127];
float fftLevels[6] = {0, 4, 5, 15, 16, 127};

// SENSITIVITY BANDS

int  noiseSensitivity; //= {0.25, 0, 0.2, 0, 0.1}; // B [0] R [0] G SENSITIVIT VALUES

int sensitivityState;
int oldNoiseSensitivity;
int prevNoiseSensitivity;
float thunderVolume = 0.6;
float oldThunderVolume;
int pStateVariable = 4;

int j;
int fftSize = sizeof(fftLevels) / 4;

int eqFactor = 1;


       float c0;
       float c1;
       float c2;
       float c3;
       float c4;
       
       float c5;
       float c6;
       float c7;
       float c8;
       
       float c9;
       float c10;
       float c11;
       float c12;

       int cn;

       int prevc1;
       int prevc4;
       int prevc7;
       int prevc9;
       int prevc12;



//****************************************************************************************************//
//****************************************************************************************************//
//****************************************************************************************************//
//*********************************      INITIALIZATIONS     *****************************************//
//****************************************************************************************************//
//****************************************************************************************************//
//****************************************************************************************************//

/* Initialize the irrecv part of the IRremote  library */
IRrecv irrecv(RECV_PIN);
decode_results results; // This will store our IR received codes
uint16_t lastCode = 0; // This keeps track of the last code RX'd



void turnoffLEDs();
void teensyMusicModeFour(); 
void teensyMusicModeTwo();
void teensyMusicMode();
void teensyMusicModeThree();
void teensyMusicChecker();
void masterReset();
void upDownLeftRightRemote();
void upDownLeftRightReturn();
void demoModeTwo();
void fadein();
void lampmode5();
void remote();
void changeVariableB();
void changeVariableA();
void donothing(int delayBetweenStorms);
void WheelShiftColor(byte WheelPos);
uint32_t Wheel(byte WheelPos);
uint32_t Color(byte r, byte g, byte b);
void Stormingusingotherfunctions(int r, int g, int b);
void SingleFlashSometimesTwin(int r, int g, int b);
void DoubleFlashSingle(int r, int g, int b);
void lampmode4 (uint8_t wait);
void rainbowCycle(uint8_t wait);
void rainbow(uint8_t wait);
void fairylights4(int SpeedOfdelay, int r, int g, int b);
void weather(int r, int g, int b);
void flashAllOff(int r, int g, int b);
void flashoff(int number, int offDelay, int r, int g, int b);
void flashon(int number, int onDelay, int r, int g, int b);
void runningFlashOFF(int number, int flashspeed, int r, int g, int b);
void runningflash(int number, int flashspeed, int r, int g, int b);












//****************************************************************************************************//
//****************************************************************************************************//
//****************************************************************************************************//
//**********************************      SETUP & LOOP      ******************************************//
//****************************************************************************************************//
//****************************************************************************************************//
//****************************************************************************************************//

void setup() {

 
  Serial.begin(9600);
 

  //analogReference(EXTERNAL);
  irrecv.enableIRIn(); // Start the receiver
  strip.begin();
  strip.show();
  pinMode (motion_1, INPUT);

    
  runningflash (N_PIXELS, 20, 255, 255, 255);
  runningFlashOFF(N_PIXELS, 10, 255, 255, 255);

  // AUDIO SHIELD SETUP

  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(30); // FOR SAFETY

  audioShield.enable();
  audioShield.volume(thunderVolume);
  audioShield.inputSelect(AUDIO_INPUT_MIC);
  //audioShield.micGain(10);      //17

  // SD CARD SETUP

  // SPI.setMOSI(7);
  // SPI.setSCK(14);
  // if (!(SD.begin(10))) {
  //   // stop here, but print a message repetitively
  //   while (1) {
  //     Serial.println("Unable to access the SD card");
  //     delay(500);
  //   }
  // }

  // FFT CONFIGURE


  // Configure the window algorithm to use
  myFFT.windowFunction(AudioWindowHanning256);
  //myFFT.windowFunction(NULL);

  
    // LOAD EEPROM

    
  Serial.println("Load EEPROM");
 
    // SAVE YELLOW FREQUENCY AT LOCATION '0'
    yellowFrequncy = (int)EEPROM.read(0);
    if (yellowFrequncy == 255)     {      yellowFrequncy = 4;    }
    Serial.print("yellowFrequncy : ");
    Serial.println(yellowFrequncy);
     

    // SAVE SHIFT COLOUR AT LOCATION '1'
    shiftColor = (int)EEPROM.read(1);
    if (shiftColor == 255)    {      shiftColor = 0;    }
    Serial.print("shiftColor : ");
    Serial.println(shiftColor);
     

    // SAVE NOISE AT LOCATION '2'
    
    noiseSensitivity = (int)EEPROM.read(2);         //turned noise from a float into an int
        if(noiseSensitivity == 255)    {
         noiseSensitivity = 10;
         sensitivityState = 4;
     }
    audioShield.micGain(noiseSensitivity);      //17
    Serial.print("noiseSensitivity : ");
    Serial.println(noiseSensitivity);
     

//    // SAVE LOCKED AT LOCATION '3'
//    //    locked = (int)EEPROM.read(3);
//    locked = 0;
//    Serial.print("locked : ");
//    Serial.println(locked);
//     

    // SAVE speedOfDelay AT LOCATION '4'
    
    speedOfDelay = (int)EEPROM.read(4);
    if (speedOfDelay == 255)    {      speedOfDelay = 10;    }
    Serial.print("speedOfDelay : ");
    Serial.println(speedOfDelay);
     


    // SAVE R AT LOCATION '5'
    rvalueShift = (int)EEPROM.read(5);
    Serial.print("rvalueShift : ");
    Serial.println(rvalueShift);
     


    // SAVE G AT LOCATION '6'
    gvalueShift = (int)EEPROM.read(6);
    Serial.print("gvalueShift : ");
    Serial.println(gvalueShift);
     

    // SAVE B AT LOCATION '7'
    
    bvalueShift = (int)EEPROM.read(7);
    Serial.print("bvalueShift : ");
    Serial.println(bvalueShift);
     


    // SAVE randomstriketrigger AT LOCATION '8'
    randomstriketrigger = (int)EEPROM.read(8);
    if (randomstriketrigger == 255)     {      randomstriketrigger = 5;    }
    Serial.print("randomstriketrigger : ");
    Serial.println(randomstriketrigger);
     


    // SAVE RemoteState AT LOCATION '9'
    
    remotEeprom = (int)EEPROM.read(9);
    if (remoteState == 255)    {        remoteState = 1000;    }
    else {remoteState = BUTTON_ARRAY2[remotEeprom];   }
    previousRemoteState = remoteState;
    Serial.print("remoteState : ");
    Serial.println(remoteState);
     


    // SAVE Volume AT LOCATION '10'
    
    thunderVolume = (int)EEPROM.read(10);
    thunderVolume = thunderVolume / 10;
    if (thunderVolume == 25.50)    {      thunderVolume = 0.6;    }
    Serial.print("thunderVolume : ");
    Serial.println(thunderVolume);
}


void loop() {

  remote();

  //******************************    POWER BUTTON   ******************************************//

 if (remoteState == BUTTON_POWER) {
    Serial.println("POWER!");
 
    flashon (N_PIXELS, 0, 255, 255, 255);
    runningFlashOFF(N_PIXELS, 10, 255, 255, 255);
    
   // stateCounter++;
    strikeCounter = 0;
    stateCounter = 0;

    currentMode = BUTTON_POWER;
    remoteState = 1000;
  }
  
  if (remoteState == BUTTON_POWER_HELD) {
    Serial.println("POWER HELD!");
//   flashon (N_PIXELS, 10, 255, 0, 0);
//   masterReset();
//   runningFlashOFF(N_PIXELS, 10, 0, 255, 0);
//   remoteState = 1000;
  }


  //********************************    BUTTON A  ********************************************//
  
  if (remoteState == BUTTON_A_HELD){
     if (flashCountA == 1){ 
      currentMode = BUTTON_C;
       Serial.println("Breathing");
      stateCounter = 0;
      flashCountA = 0;
      delay(1000);
     }
     lampmode4(speedOfDelay);
  }
  
  
  if (remoteState == BUTTON_A)
  {
    if (flashCountA == 1){ 
      currentMode = BUTTON_C;
      flashon (strip.numPixels(), 0, 220,50,80);
      runningFlashOFF(strip.numPixels(), 10, 220,50,80);
        Serial.println("Music Strike");
      flashCountA = 0;
     }
     teensyMusicModeFour();
  }
    

  //*********************************  BUTTON B  ********************************************//
  
  if (remoteState == BUTTON_B_HELD) {
     
      if (flashCount == 1){ 
       currentMode = BUTTON_B;
       flashon (strip.numPixels(), 0, 200, 50, 120);
       runningFlashOFF(strip.numPixels(), 10, 200, 50, 120);
        Serial.println("Music Color");
       flashCount = 0;
       delay(1000);
      }
      teensyMusicMode();
  }

  
  if (remoteState == BUTTON_B) {
     if (flashCount == 1){ 
        currentMode = BUTTON_B;
        flashon (strip.numPixels(), 0, 0, 189, 66);
        runningFlashOFF(strip.numPixels(), 10, 0, 189, 66);
        lampMode = 1;
        firstPassLamp = 1;
        shouldIflash = 1;
         Serial.println("Music Fade");
        flashCount = 0;
      }
      teensyMusicModeThree();
  }


  //*********************************   C BUTTONS  ********************************************//
  
  if (remoteState == BUTTON_C_HELD){

    if (flashCount == 1){ 
    currentMode = BUTTON_C;
     Serial.println("Neon");
    flashCount = 0;
    delay(1000);
   }
    rainbowCycle(speedOfDelay);
  }


  
  if (remoteState == BUTTON_C)
  {
    if (flashCount == 1){ 
     currentMode = BUTTON_C;
     stateCounter = 0;
      Serial.println("Fairy");
     fadein();
     //flashon (N_PIXELS, 0, rvalueShift, rvalueShift, rvalueShift);
     
     remote();
     flashCount = 0;
    }
    fairylights4(speedOfDelay, rvalueShift, gvalueShift, bvalueShift);
    stateCounter++;
  }

  

  //******************************* CIRCLE BUTTONS  ******************************************//

  
  if (remoteState == BUTTON_CIRCLE_HELD) {
    if (flashCount == 1){ 
    stateCounter = 0;
    stormCounter = 100;
    Serial.println("Motion Storm");
    flashCount = 0;
    previousRemoteState = remoteState;
    currentMode = BUTTON_A;
    }
    if (stateCounter == 0){
      flashon (strip.numPixels(), 0, 0, 80, 60);
      runningFlashOFF(strip.numPixels(), 10, 0, 80, 60);
      delay(1000);
    }
    weather(rvalueShift, gvalueShift, bvalueShift);
    stateCounter++;
    mode = 1;
  }
  
  else if (remoteState == BUTTON_CIRCLE) {
    if (flashCount == 1){ 
      Serial.println("Timed Storm");
      flashCount = 0;
      previousRemoteState = remoteState;
      currentMode = BUTTON_A;
    }
    demoModeTwo();
  }

  //******************************* RIGHT BUTTON  ******************************************//

  if (remoteState == BUTTON_CDOWN) {
    
    //remote();
    
    
    // MAKE REMOTE STATE PREVIOUS REMOTE STATE
    
    if(millis() - variableMillis > 1000)
    {
      upDownLeftRightReturn();
    }
    
    
    // THE PURPOSE OF THIS FUNCTION IS TO INCTREMENT THE WHEEL
    
    if(variableState == 0)
    {
    shiftColor = shiftColor + colourVariable;
    
    if(shiftColor > 255){
     shiftColor = 0; 
    }

     Serial.println(shiftColor);
    
    flashon(N_PIXELS,0,rvalueShift,gvalueShift,bvalueShift);
    WheelShiftColor(shiftColor);
    
    for (int j=0; j < N_PIXELS; j++) {
      //remote(); 
      strip.setPixelColor(j,rvalueShift,gvalueShift,bvalueShift);
      strip.show();
    } 
    
    //stateCounter = 0;

     EEPROM.update(1, shiftColor);
    
    variableState = 1;
    
    remote();
    
    }

  }

  //******************************* LEFT BUTTON  ******************************************//
  
  if (remoteState == BUTTON_CUP) {
    
    //remote();
    
    if(millis() - variableMillis > 1000)
    {
     upDownLeftRightReturn();
    
    }

    
    
    if(variableState == 0)
    {
    shiftColor = shiftColor - colourVariable;
    
    
    if(shiftColor < 0){
     shiftColor = 255; 
    }
     Serial.println(shiftColor);
    flashon(N_PIXELS,0,rvalueShift,gvalueShift,bvalueShift);
    WheelShiftColor(shiftColor);
    
    for (int j=0; j < N_PIXELS; j++) {
      //remote(); 
      strip.setPixelColor(j,rvalueShift,gvalueShift,bvalueShift);
      strip.show();
    } 
    
    //stateCounter = 0;  
  
     EEPROM.update(1, shiftColor);
    
    remote();
    
    variableState = 1;

    
    }
  }

  //******************************* A UP BUTTON  ******************************************//

  if (remoteState == BUTTON_AUP) {
      
   if(millis() - variableMillis > 1500){
    upDownLeftRightReturn();
    }
    
    if(variableState == 0){
      
      if(currentMode == BUTTON_A){                                   //A  Lightening Mode
          if (yellowFrequncy != 6) {
            yellowFrequncy++;
             EEPROM.update(0, yellowFrequncy);
          }
      variableState = 1;
      changeVariableA();
    }


      else if(currentMode == BUTTON_C){                             //C
        
        // SPEED OF THE EFFECT
         if (cStateVariable != 6) {
          cStateVariable++;
        }   
      variableState = 1;
      changeVariableA();
    }
   }
  }

  //******************************* A  DOWN BUTTON  ***************************************//

  if (remoteState == BUTTON_ADOWN) {
    
  if(millis() - variableMillis > 1500){
        upDownLeftRightReturn();
     }     
      
      if(variableState == 0){
      
      if(currentMode == BUTTON_A){                              // A   Lightening Mode
        if (yellowFrequncy != 1) {
          yellowFrequncy--;
           EEPROM.update(0, yellowFrequncy);
        }
      variableState = 1;
      changeVariableA();
    }
      
      else if(currentMode == BUTTON_C){                         //C

        if (cStateVariable != 1) {
          cStateVariable--;
        }   
      variableState = 1;
      changeVariableA();
    }
    }
  }


 //******************************* B UP BUTTON  ******************************************//

  if (remoteState == BUTTON_BUP) {
      
   if(millis() - variableMillis > 1500){
    upDownLeftRightReturn();
     if (currentMode == BUTTON_C){ flashCount = 0;  }
    }
    
    if(variableState == 0){
      
      if(currentMode == BUTTON_B){                       //B
          
          if (sensitivityState != 6){
          sensitivityState++;
         }
      variableState = 1;
      changeVariableB();
    }

      else if(currentMode == BUTTON_A){                             //Lightening Volume
        
        // SPEED OF THE EFFECT
         if (pStateVariable != 6) {
          pStateVariable++;
        }   
      variableState = 1;
      changeVariableB();
    }
   }
  }

  //*******************************  B DOWN BUTTON  ***************************************//

  if (remoteState == BUTTON_BDOWN) {
    
  if(millis() - variableMillis > 1500){
        upDownLeftRightReturn();
        if (currentMode == BUTTON_C){ flashCount = 0;  }
     }     
      
      if(variableState == 0){
      
      if(currentMode == BUTTON_B){                         //B
          
          if (sensitivityState != 1){
          sensitivityState--;
         }
      variableState = 1;
      changeVariableB();   
      }
      
      else if(currentMode == BUTTON_A){                         //Lightening Volume

        if (pStateVariable != 1) {
          pStateVariable--;
        }
      variableState = 1;
      changeVariableB();     
      }
    }
  }

  
}    //********************************    END LOOP   ********************************************//



  void changeVariableA(){
        
        if(currentMode == BUTTON_A)                                 //A
          {
            if (yellowFrequncy == 6) {
              flashon (N_PIXELS, 0, 255, 255, 0);
              randomstriketrigger = 1;
            }
            else if (yellowFrequncy == 5) {
              flashon (N_PIXELS, 0, 200, 200, 0);
              randomstriketrigger = 3;
            }
            else if (yellowFrequncy == 4) {
              flashon (N_PIXELS, 0, 100, 100, 0);
              randomstriketrigger = 5;
            }
            else if (yellowFrequncy == 3) {
              flashon (N_PIXELS, 0, 50, 50, 0);
              randomstriketrigger = 10;
            }
            else if (yellowFrequncy == 2) {
              flashon (N_PIXELS, 0, 20, 20, 0);
              randomstriketrigger = 25;
            }
            else if (yellowFrequncy == 1) {
              flashon (N_PIXELS, 0, 1, 1, 0);
              randomstriketrigger = 50;
            }
            if (oldRandomstrikeTrigger != randomstriketrigger){
             EEPROM.update(8, randomstriketrigger);
            
              Serial.print("randomstriketrigger =  ");
              Serial.println(randomstriketrigger);
             
            oldRandomstrikeTrigger = randomstriketrigger;
            }
          }


          else if (currentMode == BUTTON_C) {                           //C
            
            if (cStateVariable == 6) {
              flashon (N_PIXELS, 0, 0, 0, 255); 
              speedOfDelay = 0;
            }
            else if (cStateVariable == 5) {
              flashon (N_PIXELS, 0, 0, 0, 200);
              speedOfDelay = 3;
            }
            else if (cStateVariable == 4) {
              flashon (N_PIXELS, 0, 0, 0, 50);
              speedOfDelay = 10;
            }
            else if (cStateVariable == 3) {
              flashon (N_PIXELS, 0, 0, 0, 20);
              speedOfDelay = 12;
            }
            else if (cStateVariable == 2) {
              flashon (N_PIXELS, 0, 0, 0, 10);
              speedOfDelay = 18;
            }
             else if (cStateVariable == 1) {
              flashon (N_PIXELS, 0, 0, 0, 10);
              speedOfDelay = 20;
            } 
            if (oldSpeedOfDelay != speedOfDelay){
             EEPROM.update(4, speedOfDelay);
            
              Serial.print("speedOfDelay =  ");
              Serial.println(speedOfDelay);
             
            oldSpeedOfDelay = speedOfDelay;
            }
          }         
  }


  void changeVariableB(){
        
        if (currentMode == BUTTON_B) {                      //B  >   0 min 0.81 max
            
            if (sensitivityState == 6) {
              flashon (N_PIXELS, 0, 0, 180, 90);
              for(int i = 0; i<5; i++){
              noiseSensitivity = 20;
             }
            }
            else if (sensitivityState == 5) {
              flashon (N_PIXELS, 0, 0, 140, 70);
              for(int i = 0; i<5; i++){
              noiseSensitivity = 15;
             }
            }
            else if (sensitivityState == 4) {
              flashon (N_PIXELS, 0, 0, 80, 40);
              for(int i = 0; i<5; i++){
              noiseSensitivity = 10;
             }
            }
            else if (sensitivityState == 3) {
               flashon (N_PIXELS, 0, 0, 80, 40);
               for(int i = 0; i<5; i++){
              noiseSensitivity = 8;
             }
            }
            else if (sensitivityState == 2) {
              flashon (N_PIXELS, 0, 0, 20, 10);
              for(int i = 0; i<5; i++){
              noiseSensitivity = 3;
             }
            }
            else if (sensitivityState == 1) {
              flashon (N_PIXELS, 0, 0, 1, 1);
              for(int i = 0; i<5; i++){
              noiseSensitivity = 0;
             }
            }

            
              // SAVE TO EEPROM            

            if (oldNoiseSensitivity != noiseSensitivity){
             EEPROM.update(2, noiseSensitivity);
            
              Serial.print("noiseSensitivity =  ");
              Serial.println(noiseSensitivity);
             
            audioShield.micGain(noiseSensitivity);      //17
            oldNoiseSensitivity = noiseSensitivity;
            }   
          }


                else if (currentMode == BUTTON_A) {                           //thunder modes
            
            if (pStateVariable == 6) {
              flashon (N_PIXELS, 0, 255, 0,0); 
              thunderVolume = 0.8;
            }
            else if (pStateVariable == 5) {
              flashon (N_PIXELS, 0, 200, 0, 0);
              thunderVolume = 0.7;
            }
            else if (pStateVariable == 4) {
              flashon (N_PIXELS, 0, 100, 0, 0);
              thunderVolume = 0.6;
            }
            else if (pStateVariable == 3) {
              flashon (N_PIXELS, 0, 20, 0, 0);
              thunderVolume = 0.3;
            }
            else if (pStateVariable == 2) {
              flashon (N_PIXELS, 0, 10, 0, 0);
              thunderVolume = 0.1;
            }
             else if (pStateVariable == 1) {
              flashon (N_PIXELS, 0, 5, 0, 0);
              thunderVolume = 0.0;
            } 
            if (oldThunderVolume != thunderVolume){
             EEPROM.update(10, (thunderVolume*10));
            
              Serial.print("thunderVolume =  ");
              Serial.println(thunderVolume);
             
            oldThunderVolume = thunderVolume;
            audioShield.volume(thunderVolume);
            }
       }
  }

//****************************************************************************************************//
//****************************************************************************************************//
//****************************************************************************************************//
//**********************************       REMOTE FUNCTION     ***************************************//
//****************************************************************************************************//
//****************************************************************************************************//
//****************************************************************************************************//



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

        if (buttonHeld >= 4) {                                   // Button Holds
             Serial.print("buttonHeld :    ");
             Serial.println(currentButton);

            if (currentButton == 'O') {
              previousRemoteState = remoteState;
              remoteState = BUTTON_CIRCLE_HELD;
              EEPROM.update(9, 5);  // EEPROM Save 5 = BUTTON_CURCLE_HELD
              turnoffLEDs();
              flashCount = 1;
            }
            else if (currentButton == 'A') {
              previousRemoteState = remoteState;
              remoteState = BUTTON_A_HELD;
              EEPROM.update(9, 6);  // EEPROM Save 6 = BUTTON_A_HELD
              turnoffLEDs();
              flashCountA = 1;
            }
            else if (currentButton == 'B') {
              previousRemoteState = remoteState;
              remoteState = BUTTON_B_HELD;
              EEPROM.update(9, 7);  // EEPROM Save 7 = BUTTON_B_HELD
              turnoffLEDs();
              flashCount = 1;
            }
            else if (currentButton == 'P') {
              //remoteState = BUTTON_POWER_HELD;
              masterReset();
            }
            else if (currentButton == 'C') {
              previousRemoteState = remoteState;
              remoteState = BUTTON_C_HELD;
              EEPROM.update(9, 8);  // EEPROM Save 8 = BUTTON_C_HELD
              turnoffLEDs();
              flashCount = 1;
            }
          }
        
      }
      else {
        for (int i = 0; i < 11; i++) 
        {              //compare against each of the button codes
          if (resultCode == BUTTON_ARRAY[i]) {

            // ACTUAL BUTTON
            newButtonPress = 1;

            if (resultCode == BUTTON_POWER) {
              currentButton = 'P';
            }
            else if (resultCode == BUTTON_A) {
              currentButton = 'A';
              flashCountA = 1;
            }
            else if (resultCode == BUTTON_B) {
              currentButton = 'B';
            }
            else if (resultCode == BUTTON_C) {
              currentButton = 'C';
              //flashCount = 1;
            }
            else if (resultCode == BUTTON_AUP) {
              currentButton = 'U';
              if (currentMode == BUTTON_C || BUTTON_A) {
                upDownLeftRightRemote();
                remoteState = BUTTON_AUP;
               }
              }
            else if (resultCode == BUTTON_ADOWN) {
              currentButton = 'D';
                if (currentMode == BUTTON_C || BUTTON_A) {
                  upDownLeftRightRemote();
                  remoteState = BUTTON_ADOWN;
                }
              }
              else if (resultCode == BUTTON_BUP) {
              currentButton = 'I';
                if (currentMode == BUTTON_B || BUTTON_A) {
                  upDownLeftRightRemote();
                  remoteState = BUTTON_BUP;
                }
              }
            else if (resultCode == BUTTON_BDOWN) {
              currentButton = 'Y';
              if (currentMode == BUTTON_B || BUTTON_A) {
                upDownLeftRightRemote();
                remoteState = BUTTON_BDOWN;
               }
              }
            else if (resultCode == BUTTON_CUP) {
              currentButton = 'L';
//                if (wheelPosition < 12){       
//                  wheelPosition++;  
//                  EEPROM.update(6, wheelPosition);  // EEPROM Save 
//                }
//                else {                         
//                  wheelPosition = 0; 
//                  EEPROM.update(6, wheelPosition);  // EEPROM Save
//                  }
              upDownLeftRightRemote();
              remoteState = BUTTON_CUP;
              }
            else if (resultCode == BUTTON_CDOWN) {
                currentButton = 'R';
//                if (wheelPosition > 0){        
//                  wheelPosition--;   
//                  EEPROM.update(6, wheelPosition);  // EEPROM Save
//                  }
//                else {                        
//                  wheelPosition = 12; 
//                  EEPROM.update(6, wheelPosition);  // EEPROM Save
//                  }
                upDownLeftRightRemote();
                remoteState = BUTTON_CDOWN;
                
              }
            else if (resultCode == BUTTON_CIRCLE) {
              currentButton = 'O';
              // strom();
              // strom();
              // strom();
              //remoteState = BUTTON_CIRCLE;
              //if(butStateCounter != 1){ butStateCounter--; }
              //newButtonPress = 0;
            }

            
            //Serial.println("Comparing Button Code");

          }
          else {

            // NOT A REAL BUTTON
            //       
            //       if (newButtonPress == 0){
            //         Serial.print("NOT A REAL BUTTON    ");
            //         Serial.println(currentButton);
            //         }
            //        
          }

        }
        buttonHeld = 0;
      }
        irrecv.resume(); // Receive the next value
    }
    else {

      // THIS IS WHERE BUTTONS ARE SET GIVEN A NEW BUTTON PRESS

      if (newButtonPress == 1) {
        playWav1.stop();
        newButtonPress = 0;

        if (buttonHeld > 4) { // number of seconds/ 4-1       // Button Holds
          // Current Button Held



          
        }


        else {                                         // Button Single Presses
            // Current Button
            Serial.println(currentButton);

            if (currentButton == 'P') {
              EEPROM.update(9, 0);  // EEPROM Save 0 = BUTTON_POWER
              previousRemoteState = remoteState;
              remoteState = BUTTON_POWER;
              flashCount = 1;
              Serial.println("Off");
              turnoffLEDs();
             // FastLED.show();
            }
            else if (currentButton == 'A') {
              EEPROM.update(9, 1);  // EEPROM Save 1 = BUTTON_A
              previousRemoteState = remoteState;
              remoteState = BUTTON_A;
              turnoffLEDs();
              flashCountA = 1;
            }
            else if (currentButton == 'B') {
              EEPROM.update(9, 2);  // EEPROM Save 2 = BUTTON_B
              previousRemoteState = remoteState;
              remoteState = BUTTON_B;
              turnoffLEDs();
              flashCount = 1;
            }
            else if (currentButton == 'C') {
              EEPROM.update(9, 3);  // EEPROM Save 3 = BUTTON_C
              previousRemoteState = remoteState;
              remoteState = BUTTON_C;
              turnoffLEDs();
              flashCount = 1;
            }
            else if (currentButton == 'O') {
              EEPROM.update(9, 4);  // EEPROM Save 4 = BUTTON_CURCLE
              previousRemoteState = remoteState;
              remoteState = BUTTON_CIRCLE;
              flashCount = 1;
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


//****************************************************************************************************//
//****************************************************************************************************//
//****************************************************************************************************//
//********************************       LIGHTING FUNCTIONS    ***************************************//
//****************************************************************************************************//
//****************************************************************************************************//
//****************************************************************************************************//

//void fairy(){
//
////int goingUp[N_PIXELS];
////int currentValue[N_PIXELS];
//
//  // turn all leds on
//  // randomly choose 1 led to fade out
//  // randomly choose a second led to fade out
//  // fade in first led
//  // randomly choose a different led to fade out
//  // randomly choose a different led to fade in
//
//    /* Create an array of the LED "status"  currentValue[N_PIXELS]
//     * 
//     * 
//     * 
//     */
//
//    
//    //for (int x = 0; x <= N_PIXELS; x++) 
//   
//
//     //ledSingle1 = random (25)  put this into set up of function.
//     //ledSingle2 = random (25)  put this into set up of function.
//
//     for (int x = 0; x <= N_PIXELS; x++) {
//       if ((x != ledSingle1 || ledSingle2)){
//       strip.setPixelColor(x, (255 * r) / 255, (255 * g) / 255, (255 * b) / 255);        
//       }
//     }
//
//
//        
//      if (goingUp[ledSingle1] == 1){               // brighten
//      currentValue[ledSingle1]++;
//        if (currentValue[ledSingle1] >= 255) { 
//          //goingUp[random (25)] = 0;
//        }
//      }
//      else{                               //  dim
//        currentValue[ledSingle1]--;
//        if (currentValue[ledSingle1] <= minLEDvalue[x]) {
//          goingUp[ledSingle1] = 1;
//        }
//      }
//   
//}



void runningflash(int number, int flashspeed, int r, int g, int b) {
  for (int i = 0; i <= number; i++) {
    strip.setPixelColor(i, (255 * r) / 255, (255 * g) / 255, (255 * b) / 255);
    delay(flashspeed);
    strip.show();
  }
}

void runningFlashOFF(int number, int flashspeed, int r, int g, int b) {
  for (int i = 0; i <= number; i++) {
    strip.setPixelColor(i, (0 * r) / 255, (0 * g) / 255, (0 * b) / 255);
    delay(flashspeed);
    strip.show();
  }
}

void flashon(int number, int onDelay, int r, int g, int b) {
  for (int i = 0; i <= number; i++) {
    strip.setPixelColor(i, (255 * r) / 255, (255 * g) / 255, (255 * b) / 255);
    strip.show();
  }
  delay (onDelay);
}

void flashoff(int number, int offDelay, int r, int g, int b) {
  for (int i = 0; i <= number; i++) {
    strip.setPixelColor(i, 0 * r, 0 * g, 0 * b);
    strip.show();
  }
  delay (offDelay);
}

void flashAllOff(int r, int g, int b) {
  for (int i = 0; i <= 25; i++) {
    strip.setPixelColor(i, (0 * r) / 255, (0 * g) / 255, (0 * b) / 255);
    strip.show();
  }
}

void weather(int r, int g, int b) {
  unsigned long currentMillis = millis();
  intervalSensor = (1001);
  if (currentMillis - previousMillisSensor > intervalSensor) {
    previousMillisSensor = currentMillis;
    sensor_1 = digitalRead(motion_1);

    if (sensor_1 == HIGH) {
 
      Serial.println("Motion Detected");
 
      flashAllOff(r, g, b);
      DoubleFlashSingle(r, g, b);
      SingleFlashSometimesTwin(r, g, b);
      flashAllOff(r, g, b);
      strikeCounter ++;
 
      Serial.print("Strike number:   ");
      Serial.println(strikeCounter);
 
    }

    if (strikeCounter >= randomstriketrigger) {
      firststorm = 0;
      Stormingusingotherfunctions(rvalueShift, gvalueShift, bvalueShift);
 
      Serial.println("STORM!");
 
      strikeCounter = 0;
    }
  }
}

void fairylights4(int SpeedOfdelay, int r, int g, int b) {

//  if (stateCounter == 0) {
//    fadein();
//    remote();
//    
//  }
  remote();   // added this 2019

  while (remoteState == BUTTON_C)  {    //TODO?
    ledSingle2 = (ledSingle1 + 2);
    ledSingle3 = random (25);

    for (int i = 255; i > 0; i--)
    {
      fadeout = 255 - i;
      strip.setPixelColor(ledSingle2, (i * r) / 255, (i * g) / 255, (i * b) / 255);
      strip.setPixelColor(ledSingle1, (fadeout * r) / 255, (fadeout * g) / 255, (fadeout * b) / 255);
      strip.show();
      //Serial.println("1st!");
        remote();
      if (remoteState != BUTTON_C){
        break;
      }
      if (buttonHeld > 4){
        break;
      }

      delay (SpeedOfdelay);
    }

    ledSingle1 = ledSingle1 +1;
    if (ledSingle1 > 24) {
      ledSingle1 = 1;
    }
    
    if ((ledSingle3 != ledSingle2) && (ledSingle3 != ledSingle1)) {
      for (int k = 255; k > 0; k--)
      {
        strip.setPixelColor(ledSingle3, (k * r) / 255, (k * g) / 255, (k * b) / 255);
        strip.show();
        //Serial.println("2nd!");
           remote();
        if (remoteState != BUTTON_C) {
          break;
        }
        if (buttonHeld > 4) {
          break;
        }
        delay (SpeedOfdelay);
      }
    }



    for (int j = 0; j < 255; j++)
    {
      fadeout = 255 - j;
      strip.setPixelColor(ledSingle2, (j * r) / 255, (j * g) / 255, (j * b) / 255);
      strip.setPixelColor(ledSingle1, (fadeout * r) / 255, (fadeout * g) / 255, (fadeout * b) / 255);
      strip.show();
      //Serial.println("3rd!");
      remote();
      if (remoteState != BUTTON_C) {
        break;
      }
      if (buttonHeld > 4) {
        break;
      }
      delay (SpeedOfdelay);
    }

    if ((ledSingle3 != ledSingle2) && (ledSingle3 != ledSingle1)) {
      for (int x = 0; x < 255; x++)
      {
        
        strip.setPixelColor(ledSingle3, (x * r) / 255, (x * g) / 255, (x * b) / 255);
        strip.show();
       // Serial.println("4th!");
        remote();
        if (remoteState != BUTTON_C) {
          break;
        }
        if (buttonHeld > 4) {
          break;
        }
        delay (SpeedOfdelay);
      }
    }
  }
}



void rainbow(uint8_t wait) {
  int i, j;

  for (j = 0; j < 256; j++) {   // 3 cycles of all 256 colors in the wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 255));
    }
    strip.show();   // write all the pixels out
    remote();
    if (remoteState != previousRemoteState){                      //changed
      break;
    }
    if (buttonHeld > 30){
      break;
    }
    delay(wait);
  }
}

// Slightly different, this one makes the rainbow wheel equally distributed
// along the chain
void rainbowCycle(uint8_t wait) {
  int i, j;

  for (j = 0; j < 256 * 5; j++) {   // 5 cycles of all 25 colors in the wheel
    for (i = 0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 256 / strip.numPixels()) + j) % 256) );
    }
    remote();
    if (remoteState != previousRemoteState){                  //changed
      break;
    }
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

void lampmode4 (uint8_t wait) {
  int i, j;

  for (j = 20; j < 255; j++) {   //
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i,j,j,j);
    }
    strip.show();   // write all the pixels out
    remote();
    
    if (remoteState != previousRemoteState){                      //changed
      break;
    }
    if (buttonHeld > 30){
      break;
    }
    delay(wait);
  }

  for (j = 255; j > 20; j--) {   //
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i,j,j,j);
    }
    strip.show();   // write all the pixels out
    remote();
    
    if (remoteState != previousRemoteState){                      //changed
      break;
    }
    if (buttonHeld > 30){
      break;
    }
    delay(wait);
  }
}


void lampmode5 () {

    delaycount++;
    if (delaycount > speedOfDelay * 2){

      if (upupup == 1){               // brighten
      fadeval++;
        if (fadeval >= 255) { 
          upupup = 0;
          //Serial.println("upupup");
        }
      }
      else{                               //  dim
        fadeval--;
        if (fadeval <= 20) {
          upupup = 1;
          //Serial.println("downdowndown");
        }
      }

     //Serial.println(fadeval);
     for (int i = 0; i < 25; i++) {
       strip.setPixelColor(i,fadeval,fadeval,fadeval);
      }
      strip.show();   // write all the pixels out
      //delay(10);
      delaycount = 0;
     }
     //delay(1);
}



void DoubleFlashSingle(int r, int g, int b) {
  ledSingle1 = random (25);
  strip.setPixelColor(ledSingle1, (255 * r) / 255, (255 * g) / 255, (255 * b) / 255);
  strip.show();
  delay(random (10, 100));
  strip.setPixelColor(ledSingle1, (0 * r) / 255, (0 * g) / 255, (0 * b) / 255);
  strip.show();
  strip.setPixelColor(ledSingle1, (255 * r) / 255, (255 * g) / 255, (255 * b) / 255);
  strip.show();
  delay(random (40, 100));
  strip.setPixelColor(ledSingle1, (0 * r) / 255, (0 * g) / 255, (0 * b) / 255);
  strip.show();
}

void SingleFlashSometimesTwin(int r, int g, int b) {
  randomiser = random (1, 3);
  if (randomiser == 2) {
    ledSingle1 = random (25);
    ledSingle2 = random (50);
    strip.setPixelColor(ledSingle1, (255 * r) / 255, (255 * g) / 255, (255 * b) / 255);
    strip.setPixelColor(ledSingle2, (255 * r) / 255, (255 * g) / 255, (255 * b) / 255);
    strip.show();
  }
  else {
    ledSingle1 = random (25);
    ledSingle2 = ledSingle1 ++;
    ledSingle3 = ledSingle2 + 2;
    strip.setPixelColor(ledSingle1, (255 * r) / 255, (255 * g) / 255, (255 * b) / 255);
    strip.show();
    delay (20);
    strip.setPixelColor(ledSingle2, (255 * r) / 255, (255 * g) / 255, (255 * b) / 255);
    strip.setPixelColor(ledSingle1, (0 * r) / 255, (0 * g) / 255, (0 * b) / 255);
    strip.show();
    delay (20);
    strip.setPixelColor(ledSingle2, (0 * r) / 255, (0 * g) / 255, (0 * b) / 255);
    strip.setPixelColor(ledSingle3, (255 * r) / 255, (255 * g) / 255, (255 * b) / 255);
    strip.show();
  }
}

void fadein() {
  for (int i = 0; i <= 255; i++) {
    for (int j = 0; j <= strip.numPixels(); j++) {
      strip.setPixelColor(j, (i * rvalueShift) / 255, (i * gvalueShift) / 255, (i * bvalueShift) / 255);
    }
    //strip.setPixelColor(0, 0, 0, 0);
    strip.show();
   // delay (20);
    remote();
    if (remoteState != BUTTON_C) {
      break;
    }
    if (buttonHeld > 10) {
      break;
    }
  }
}


//****************************************************************************************************//
//****************************************************************************************************//
//****************************************************************************************************//
//*********************************       OTHER FUNCTIONS    *****************************************//
//****************************************************************************************************//
//****************************************************************************************************//
//****************************************************************************************************//


void Stormingusingotherfunctions(int r, int g, int b) {
  
    int randomChoose = int (random (1, 9));
  
  firststorm = 0;

  if (randomChoose == 1) {
 
    Serial.println("01");
 

    playWav1.play("1.WAV");

    // A brief delay for the library read WAV info
    delay(5);
  }

  if (randomChoose == 2) {
 
    Serial.println("02");
 

    playWav1.play("2.WAV");
    delay(5);
  }
  if (randomChoose == 3) {
 
    Serial.println("03");
 

    playWav1.play("3.WAV");
    delay(5);
  }
  if (randomChoose == 4) {
 
    Serial.println("04");
 
    playWav1.play("4.WAV");
    delay(5);
  }
  if (randomChoose == 5) {
 
    Serial.println("05");
    delay(5);
 
    playWav1.play("5.WAV");
    delay(5);
  }
  if (randomChoose == 6) {
 
    Serial.println("06");
 

    playWav1.play("6.WAV");
  }
  if (randomChoose == 7) {
 
    Serial.println("07");
 

    playWav1.play("7.WAV");
    delay(5);
  }
  if (randomChoose == 8) {
 
    Serial.println("08");
 
    playWav1.play("8.WAV");
    delay(5);
  }

 
  Serial.println(F("Started playing"));
 



  while (playWav1.isPlaying()) {
    
    if( firststorm == 0){

      runningflash(random (25), 3, r, g, b);
      for (int i = 0; i <= (int)random(3); i++) {
        flashon (random(25), random(10, 40), r, g, b);
        flashoff (random(25), random(10, 200), r, g, b);
      }

      flashAllOff(r, g, b);
      for (int i = 0; i <= (int)random(4); i++) {
        flashon (random(25), random(10, 50), r, g, b);
        flashoff (random(25), random(10, 100), r, g, b);
      }
      //remote();

      flashAllOff(r, g, b);
      DoubleFlashSingle(r, g, b);
      SingleFlashSometimesTwin(r, g, b);
      flashAllOff(r, g, b);
      SingleFlashSometimesTwin(r, g, b);
      flashAllOff(r, g, b);
      
      firststorm =1;
    }
    else{
     
      //DO NOTHING
     
      
     remote();


//       if (remoteState == BUTTON_CIRCLE) {
//
//        remoteState = previousRemoteState;
//       }
      
    }
    



  }
}


/* Helper functions */

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85) {
    return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
//   EEPROM.update(5, rvalueShift);
//   EEPROM.update(6, gvalueShift);
//   EEPROM.update(7, bvalueShift);
}

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
void WheelShiftColor(byte WheelPos)
{
  if (WheelPos < 85) {
    rvalueShift = WheelPos * 3;
    gvalueShift = 255 - WheelPos *3;
    bvalueShift = 0;  
    
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    rvalueShift = 255 - WheelPos * 3;
    gvalueShift = 0;
    bvalueShift = WheelPos *3; 
    
  } else  if (WheelPos < 200) {
    WheelPos -= 170;
    rvalueShift = 0;
    gvalueShift = WheelPos * 3;
    bvalueShift = 255 - WheelPos * 3;
    
  } else  if (WheelPos < 240) {
    WheelPos -= 200;
    rvalueShift = 255;
    gvalueShift = 241;
    bvalueShift = 224; 
    
  } else {
    WheelPos -= 240;
    rvalueShift = 255;
    gvalueShift = 255;
    bvalueShift = 255; 
  }
  
  
  EEPROM.update(5, rvalueShift);
  EEPROM.update(6, gvalueShift);
  EEPROM.update(7, bvalueShift);
}




//****************************************************************************************************//
//****************************************************************************************************//
//****************************************************************************************************//
//***********************************       DEMO FUNCTION    ****************************************//
//****************************************************************************************************//
//****************************************************************************************************//
//****************************************************************************************************//




void demoModeTwo(){
    
  while (remoteState == BUTTON_CIRCLE){
    remote();
    
    
  Serial.println("DEMO IN 2");
     

    Stormingusingotherfunctions(rvalueShift,gvalueShift,bvalueShift);
       remote();
    if (remoteState != BUTTON_CIRCLE){
      break;
    }
    donothing(random(1,14));
       remote();
    if (remoteState != BUTTON_CIRCLE){
      break;
    }
    Stormingusingotherfunctions(rvalueShift,gvalueShift,bvalueShift);
    donothing(random(10,26));
       remote();
    if (remoteState != BUTTON_CIRCLE){
      break;
    }
    Stormingusingotherfunctions(rvalueShift,gvalueShift,bvalueShift);
    remote();
    if (remoteState != BUTTON_CIRCLE){
      break;
    }
    Stormingusingotherfunctions(rvalueShift,gvalueShift,bvalueShift);
    donothing(random(9,33));
       remote();
    if (remoteState != BUTTON_CIRCLE){
      break;
    }
    
  }
  
}





void donothing(int delayBetweenStorms){   //e.g 5
  
  doNothingInterval = (unsigned long) 1000*delayBetweenStorms;  
 
  for (int i=0; i <= 1; i++){
  remote();
   if (remoteState != previousRemoteState){                                     
       break;
      }
     
      currentDoNothingMillis = millis();
  
      if(currentDoNothingMillis - previousDoNothingMillis > doNothingInterval) { 
        previousDoNothingMillis = currentDoNothingMillis; 

        
        Serial.println("Tick!");
           

       }
       else {
       i = 0;
    }
  }
}



//****************************************************************************************************//
//****************************************************************************************************//
//****************************************************************************************************//
//*************************************       DRY FUNCTIONS    **************************************//
//****************************************************************************************************//
//****************************************************************************************************//
//****************************************************************************************************//


void upDownLeftRightReturn(){
 
    variableState = 1;
    flashCount = 1;
    remoteState = previousRemoteState;   

  variableMillis = millis();

}

void upDownLeftRightRemote(){
  
  variableState = 0;
  stateCounter = 0;
  
           
           if((remoteState != BUTTON_CDOWN) && (remoteState != BUTTON_CUP) && (remoteState != BUTTON_AUP) && (remoteState != BUTTON_ADOWN) && (remoteState != BUTTON_BUP) && (remoteState != BUTTON_BDOWN))
           {
             if(remoteState == 1000)
             {
               previousRemoteState = BUTTON_POWER;
             }
             else{
             previousRemoteState = remoteState;
             }
             
           }
           
           variableMillis = millis();
  
}

//void upDownLeftRightRemoteHeld(){
//  
//          variableState = 0;
//          variableMillis = millis();
//          irrecv.resume(); // Receive the next value
//          remote();
//          
//}


//****************************************************************************************************//
//****************************************************************************************************//
//****************************************************************************************************//
//******************************       MASTER RESET FUNCTION    **************************************//
//****************************************************************************************************//
//****************************************************************************************************//
//****************************************************************************************************//


// MASTER RESET FUNCTION

void masterReset(){
 
 // RESET EEPROM -- SET EVERYTHING TO 255

     flashon (N_PIXELS, 10, 255, 0, 0);
  runningFlashOFF(strip.numPixels(), 10, 0, 0, 255);
     flashon (N_PIXELS, 10, 255, 0, 0);
  runningFlashOFF(strip.numPixels(), 10, 0, 0, 255);
  //runningFlashOFF(N_PIXELS, 10, 0, 255, 0);
 
 for (int i = 0; i<11;i++){
  
    EEPROM.update(i, 255);
   
 }
    
    firstRun = 0;
    
    remoteState = 1000;
    rvalueShift = 255;    //255
    gvalueShift = 255;    //241 
    bvalueShift = 255;    //224

    randomstriketrigger = 5;      ////////////////////Make SURE all these are the same as at the top!
    yellowFrequncy = 4;
    speedOfDelay = 10;
    shiftColor = 0;
    audioShield.volume(0.6);
    pStateVariable = 4;
    noiseSensitivity = 10;
    sensitivityState = 4;
    audioShield.micGain(noiseSensitivity);      //17
}

//****************************************************************************************************//
//****************************************************************************************************//
//****************************************************************************************************//
//***********************************      MUSIC FUNCTION     ****************************************//
//****************************************************************************************************//
//****************************************************************************************************//
//****************************************************************************************************//

void teensyMusicChecker(){   // Music/Mic Checker Function    float fftLevels[6] = {0, 4, 5, 15, 16, 127};  bass = 0 to 4, mid = 5 to 15, high = 16 to 127


    //int eq[]={7,7,6,3,2,1,0,0,0,0,0,0,0};
    
  if (myFFT.available()) {
    
//    for (int i = 0; i < (fftSize - 1); i++)
//    {
//      j = i + 1;
//      level[i] = myFFT.read(fftLevels[i], fftLevels[j]);
//    }
//     bass = 0;
//     mid = 0;
//     high = 0;
//
//     
//     for (int i = 1; i < (3); i++){
//     int bassChannels = ((myFFT.read(i))*16) - eq[i];
//     bassChannels = constrain(bassChannels, 0, 9);
//     bass = bass + bassChannels;
//     }
//   
//
//
//     for (int i = 3; i < (7); i++){
//     int midChannels = ((myFFT.read(i))*16) - eq[i];
//     midChannels = constrain(midChannels, 0, 9);
//     mid = mid + midChannels;
//     }
//
//     for (int i = 7; i < (12); i++){
//     int highChannels = ((myFFT.read(i))*16) - eq[i];
//     highChannels = constrain(highChannels, 0, 9);
//     high = high + highChannels;
//     }
//     
//     
//     bass = bass * 42;
//     mid  = mid  * 42;
//     high = high * 42;


       //float c0  = ((myFFT.read(0 ))*16) - 7;
//       c1  = constrain(((myFFT.read(1 ))*16) / 8, 0, 9);
//       c2  = constrain(((myFFT.read(2 ))*16) / 8, 0, 9);
//       c3  = constrain(((myFFT.read(3 ))*16) / 7, 0, 9);
//       c4  = constrain(((myFFT.read(4 ))*16) / 7, 0, 9);
//       
//       c5  = constrain(((myFFT.read(5 ))*16) / 4, 0, 9);
//       c6  = constrain(((myFFT.read(6 ))*16) / 3, 0, 9);
//       c7  = constrain(((myFFT.read(7 ))*16) / 2, 0, 9);
//       c8  = constrain(((myFFT.read(8 ))*16) / 2, 0, 9);
//       
//       c9  = constrain(((myFFT.read(9 ))*16) / 1, 0, 9);
//       c10 = constrain(((myFFT.read(10))*16) / 1, 0, 9);
//       c11 = constrain(((myFFT.read(11))*16) / 1, 0, 9);
//       c12 = constrain(((myFFT.read(12))*16) / 1, 0, 9);


       //int eq[]={  198,     216,     71,    81,     53,     40,     41,     28,     22,     22,     19,     14  };


       
     //cn  = constrain(((myFFT.read(c ))*precsale) - noise, 0, 255);
       c0  = constrain(((myFFT.read(0 ))*400) - (20*eqFactor), 0, 255);
       c1  = constrain(((myFFT.read(1 ))*400) - (20*eqFactor), 0, 255);
       c2  = constrain(((myFFT.read(2 ))*500) - (12*eqFactor), 0, 255);
       c3  = constrain(((myFFT.read(3 ))*500) - (6*eqFactor) , 0, 255);
       c4  = constrain(((myFFT.read(4 ))*500) - (6*eqFactor) , 0, 255);
      
       c5  = constrain(((myFFT.read(5 ))*500) - (3*eqFactor) , 0, 255);
       c6  = constrain(((myFFT.read(6 ))*500) - (3*eqFactor) , 0, 255);
       c7  = constrain(((myFFT.read(7 ))*500) - (3*eqFactor) , 0, 255);
       c8  = constrain(((myFFT.read(8 ))*500) - (3*eqFactor) , 0, 255);
       
       c9  = constrain(((myFFT.read(9 ))*500) - (2*eqFactor) , 0, 255);
       c10 = constrain(((myFFT.read(10))*500) - (2*eqFactor) , 0, 255);
       c11 = constrain(((myFFT.read(11))*500) - (2*eqFactor) , 0, 255);
       c12 = constrain(((myFFT.read(12))*500) - (2*eqFactor) , 0, 255);

     //cn  = cn  * (1-(eq/255));

       cn = c2;
       c0  = c0  * (1-(220/255)); 
       c1  = c1  * (1-(220/255));       
       c2  = c2  * (1-(210/255));
       c3  = c3  * (1-(100/255));
       c4  = c4  * (1-(80/255));
       c5  = c5  * (1-(50/255));
       c6  = c6  * (1-(40/255));
       c7  = c7  * (1-(40/255));
       c8  = c8  * (1-(30/255));
       c9  = c9  * (1-(20/255));
       c10 = c10 * (1-(20/255));
       c11 = c11 * (1-(20/255));
       c12 = c12 * (1-(15/255));

       



       bass = ((c0 + c1 + c2  + c3 )/2);
       mid  = ((c4 + c5 + c6  + c7 )*1.5);
       high = ((c8 + c9 + c10 + c11 + c12)*2);


//       Serial.print(cn);
//       Serial.print("     ");

//       Serial.print((int)c1 );
//       Serial.print("     ");
//       Serial.print((int)c2 );
//       Serial.print("     ");
//       Serial.print((int)c3 );
//       Serial.print("     ");
//       Serial.print((int)c4 );
//       Serial.print("     ");
//       Serial.print((int)c5 );
//       Serial.print("     ");
//       Serial.print((int)c6 );
//       Serial.print("     ");
//       Serial.print((int)c7 );
//       Serial.print("     ");
//       Serial.print((int)c8 );
//       Serial.print("     ");
//       Serial.print((int)c9 );
//       Serial.print("     ");
//       Serial.print((int)c10);
//       Serial.print("     ");
//       Serial.print((int)c11);
//       Serial.print("     ");
//       Serial.print((int)c12);
//       Serial.print("     ");
//       Serial.println();




//     for (int i = 1; i < (11); i++){
//      
//     float channel = (myFFT.read(i))*100;  //*16=9 
//
//     //channel = map(channel,eq[i],9,0,9);
//     //channel = channel - eq[i];
//     
//     channel = constrain(channel, 0, 255);
//     Serial.print((int)channel);
//     Serial.print("  ");
//
//
//     }
////     Serial.println();




////    bass = map (level[0]*100,90,180,0,255);     //values 0 - 1.8
    bass = constrain(bass, 0, 255);
////    mid = map (level[2]*100,15,180,0,255);     //values 0 - 0.6
   mid = constrain(mid, 0, 255);
////    high = map (level[4]*100,25,180,0,255);     //values 0 - 0.5
    high = constrain(high, 0, 255);
  
    
    remote();

    //  SENSITIVTY:
   
//    for (int i = 0; i < 5; i++){ 
//      
////      if(level[i] < 0.5)
////      {
////       level[i] = 0; 
////      }
//      
//      if (level[i] < 0){    //clip out nothing values
//        level[i] = 0;
//      }
//    }

//    bass = level[0]*100;
//    mid = level[2]*100;
//    high = level[4]*100;
//
//       
//  // if (bass > 0 || mid > 0 || high > 0 ){
//    Serial.print("FFT:  ");
////    for (int i = 0; i < (fftSize - 1); i++) {
////
////      Serial.print(level[i]);
////      Serial.print("  ");
////      i++;
////    }
//     Serial.print(high);
//     Serial.print("  ");
////    Serial.print((level[2])*100);
////     Serial.print("  ");
////   Serial.print( (level[4])*100);
////     Serial.print("  ");
//    Serial.println();
//   //}
// 

    //level[0] rests at around 20-30 peaks at around 180
    //level[2] rests at around 13-15  peaks at around 140
    //level[4] rests at around 21-25  peaks at around 180


//    bass = map (level[0]*100,90,180,0,255);     //values 0 - 1.8
//    bass = constrain(bass, 0, 255);
//    mid = map (level[2]*100,15,180,0,255);     //values 0 - 0.6
//    mid = constrain(mid, 0, 255);
//    high = map (level[4]*100,25,180,0,255);     //values 0 - 0.5
//    high = constrain(high, 0, 255);

    //if (mid == 0 && high == 0) bass = 0;

    
    
//   
//   if (bass > 0 || mid > 0 || high > 0 ){
////    Serial.print("FFT:  ");
////    for (int i = 0; i < (fftSize - 1); i++) {
////
////      Serial.print(level[i]);
////      Serial.print("  ");
////      i++;
////    }
//     Serial.print(bass);
//     Serial.print("  ");
//    Serial.print(mid);
//     Serial.print("  ");
//    Serial.print(high);
//     Serial.print("  ");
//    Serial.println();
//   }
// 
 }
}

void teensyMusicModeThree() {    //mid responsive colourchange through left/right buttons
  
  teensyMusicChecker();

     eqFactor = 4;
     bass = constrain((bass + mid + high)/3, 0, 255);

    
    if (bass >= prevBass){                   //upbeat ||  bass 21 prevbass 19
    for (int i=prevBass; i <= bass; i=i+10){
        for (int j=0; j <=N_PIXELS; j++){
          strip.setPixelColor(j,(i*rvalueShift)/255, (i*gvalueShift)/255, (i*bvalueShift)/255);
        }   
    }
    strip.show();
    prevBass = bass;
    }

    else{                                   //fading  ||  bass 17 prevbass 19
        for (int i=prevBass; i >= bass; i=i-1){
        for (int j=0; j <=N_PIXELS; j++){
          strip.setPixelColor(j,(i*rvalueShift)/255, (i*gvalueShift)/255, (i*bvalueShift)/255);
        }   
      strip.show();
      delay (1);
      teensyMusicChecker();
      if (bass > prevBass){
      break;
        }
    }
    prevBass = bass;
    }
      
}





void teensyMusicMode() {
     eqFactor = 3;

     teensyMusicChecker();

    if (high < prevHigh){
      high = prevHigh - 10;
    }

    if (mid < prevMid){
      mid = prevMid - 10;
    }

    if (bass < prevBass){
      bass = prevBass - 18;
    }

    bass = constrain(bass, 0, 255);
    mid = constrain(mid, 0, 255);
    high = constrain(high, 0, 255);
    
      
    for (int i = 0; i <= N_PIXELS; i++) {
     // strip.setPixelColor(i, ((int)(level[4]-1) * 150), (int)(level[2] * 200), (int)(level[0] * 150));  //  4=high, 2=mid, 0=bass
        strip.setPixelColor(i, high,mid,bass);
        //strip.show();
    }
    strip.show();
    prevHigh = high;
    prevMid = mid;
    prevBass = bass;
}



void teensyMusicModeTwo() {        //bass responsive in white
    eqFactor = 4;
    teensyMusicChecker();

    int height = map(bass,100,255,0,N_PIXELS);
    height = constrain(height, 0, N_PIXELS);
    
    for(int i=0; i<N_PIXELS; i++) {
    bass = bass - (255-(i*10));
    if(i >= height)               strip.setPixelColor(i,   0,   0, 0);
    else strip.setPixelColor(i,bass,bass,bass);
    }
    strip.show(); // Update strip
 }


void teensyMusicModeFour() {        //
    eqFactor = 5;

    teensyMusicChecker();

    c1 = (c0+c1)/2;
    c4 = (c4+c5);
    c7 = (c7+c8);
    c9 = (c9+c10);
    c12= (c11+c12);
    
    
    if (c1 < prevc1){
      c1 = prevc1 - 15;
    }

    if (c4 < prevc4){
      c4 = prevc4 - 15;
    }

    if (c7 < prevc7){
      c7 = prevc7 - 15;
    }

        if (c9 < prevc9){
      c9 = prevc9 - 15;
    }

        if (c12 < prevc12){
      c12 = prevc12 - 15;
    }
    
    //int multiplyier = 2;
     
     c1 = constrain(c1, 0, 255);
     c4 = constrain(c4, 0, 255);
     c7 = constrain(c7, 0, 255);
     c9 = constrain(c9, 0, 255);
     c12 = constrain(c12, 0, 255);
    
    
                           //r  g   b
     strip.setPixelColor(0  ,c1     ,c1     ,c1 );
     strip.setPixelColor(1  ,c1     ,c1     ,c1 );
     strip.setPixelColor(2  ,c1     ,c1     ,c1 );
     strip.setPixelColor(3  ,c1     ,c1     ,c1 );
     strip.setPixelColor(4  ,c1     ,c1     ,c1 );
     
     strip.setPixelColor(5  ,c4/2     ,c4/2  ,c4 /2);
     strip.setPixelColor(6  ,c4/2     ,c4/2  ,c4 /2);
     strip.setPixelColor(7  ,c4/2     ,c4/2  ,c4 /2);
     strip.setPixelColor(8  ,c4/2     ,c4/2  ,c4 /2);
     strip.setPixelColor(9  ,c4/2     ,c4/2  ,c4 /2);
     
     strip.setPixelColor(10 ,c7/2 ,c7/2   ,c7/2 );
     strip.setPixelColor(11 ,c7/2 ,c7/2   ,c7/2 );
     strip.setPixelColor(12 ,c7/2 ,c7/2   ,c7/2 );
     strip.setPixelColor(13 ,c7/2 ,c7/2   ,c7/2 );
     strip.setPixelColor(14 ,c7/2 ,c7/2   ,c7/2 );
     
     strip.setPixelColor(15 ,c9    ,c9    ,c9);
     strip.setPixelColor(16 ,c9    ,c9    ,c9);
     strip.setPixelColor(17 ,c9    ,c9    ,c9);
     strip.setPixelColor(18 ,c9    ,c9    ,c9);
     strip.setPixelColor(19 ,c9    ,c9    ,c9);
     
     strip.setPixelColor(20 ,c12   ,c12      ,c12);
     strip.setPixelColor(21 ,c12   ,c12      ,c12);
     strip.setPixelColor(22 ,c12   ,c12      ,c12);
     strip.setPixelColor(23 ,c12   ,c12      ,c12);
     strip.setPixelColor(24 ,c12   ,c12      ,c12);

    strip.show(); // Update strip



   prevc1 = c1;
   prevc4 = c4;
   prevc7 = c7;
   prevc9 = c9;
   prevc12 = c12;
    
 }



void turnoffLEDs()
{
    for (int i = 0; i <= N_PIXELS; i++) {
        strip.setPixelColor(i, 0,0,0);
    }
    strip.show();
}
















// /*
//   Cloud 3.2 WS2081 LEDS
//   2020 Richard Clarkson Studio

//   TODO fix LED flickering: For some reason any LED quantity over 25 results in LED flickering, symptoms also result when the Speed of animation setting is active.
//    - temporary workaround limit LEDs to 25 and remove Speed of Animation setting.

//   TODO add one more white step in between blue yellow change over.
// */



// #include <Arduino.h>
// #include <FastLED.h>
// #include <Audio.h>   
// #include <EEPROM.h>
// #include <IRremote.h>

// FASTLED_USING_NAMESPACE
// #define NUM_LEDS 50        //     T=10 S=17 M=25 L=50* L= 2 sets of 25

// int LED_ADJUSTED = 25;     

// const int shunt1Pin = 5;     // We will use shunts to change quantity of LEDs on the fly
// const int shunt2Pin = 6;
// int shunt1;
// int shunt2;


// //Global Varriables
// int channel = 8;
// int sensitivity;  // 0-8 where 8 = maximum sensitivity
// int Bvariable;  // brightness

// //Sound Variables
// int soundLevel;          // this is the output of the FFT after being EQ
// int dot = 100;           // this is a slowly falling value based on the peaks of soundLevel, used by musicMode1 and musicMode4
// int dotCount = 1;

// int Bass;
// int Mid;
// int High;
// int prevBass;
// int prevMid;
// int prevHigh;

// const int numReadings = 60;

// int readings[numReadings];      // the readings from the analog input
// int readIndex = 0;              // the index of the current reading
// int total = 0;                  // the running total
// int average = 0;                // the average

// int readingsM[numReadings];      // the readings from the analog input
// int readIndexM = 0;              // the index of the current reading
// int totalM = 0;                  // the running total
// int averageM = 0;                // the average

// int readingsH[numReadings];      // the readings from the analog input
// int readIndexH = 0;              // the index of the current reading
// int totalH = 0;                  // the running total
// int averageH = 0;                // the average


// //IR Varriables
// #define NUM_BUTTONS 18 // The remote has 9 buttons

// int RECV_PIN = 15;       //

// // Prototype Functions:
// void remote();
// void upDownLeftRightReturn();
// void upDownLeftRightRemote();
// void upDownLeftRightRemoteHeld();


// //*******************************       BUTTON CONSTANTS    ******************************************//

// const uint16_t BUTTON_1 = 0xD827; // i.e. 0x10EFD827  //55335  - was Button Power
// const uint16_t BUTTON_1_HELD = 23;

// const uint16_t BUTTON_2 = 0x7887;  // was button B
// const uint16_t BUTTON_3 = 0x58A7;  // was button C
// const uint16_t BUTTON_3_HELD = 24;
// const uint16_t BUTTON_4 = 0xF807;  // was button A
// const uint16_t BUTTON_5 = 0x609F;   // was button B held
// const uint16_t BUTTON_6 = 0xE01F;   // was button C held
// const uint16_t BUTTON_7 = 0x20DF; // hex = 4815   was button circle
// const uint16_t BUTTON_8 = 0x906F;   // was button circle held  
// const uint16_t BUTTON_9 = 0x08F7;   // was button A HELD

// const uint16_t BUTTON_AUP = 0xA05F;
// const uint16_t BUTTON_ADOWN = 0x00FF;   
// //const uint16_t BUTTON_ARESET = 0x28D7;  //na 

// const uint16_t BUTTON_BUP = 0xC03F;
// const uint16_t BUTTON_BDOWN = 0x40BF;
// //const uint16_t BUTTON_BRESET = 0xA857; //na

// const uint16_t BUTTON_CUP = 0x10EF;     
// const uint16_t BUTTON_CDOWN = 0x807F;   
// //const uint16_t BUTTON_CRESET = 0x6897;  //na 

// const uint16_t BUTTON_HELD = 0xFFFF;

// uint16_t BUTTON_ARRAY[15] = {BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4, BUTTON_5, BUTTON_6, BUTTON_7, BUTTON_8, BUTTON_9, BUTTON_AUP, BUTTON_ADOWN, BUTTON_BUP, BUTTON_BDOWN, BUTTON_CUP, BUTTON_CDOWN};

// uint16_t BUTTON_ARRAY2[10] = {BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4, BUTTON_5, BUTTON_6, BUTTON_7, BUTTON_8, BUTTON_9, BUTTON_1_HELD};


// //*******************************       IR CONSTANTS    ******************************************//

// int colourVariable = 20;
// int variableState = 0;
// unsigned long previousMillisSensor = 0;
// unsigned long variableMillis = millis();


// long checking = 0;
// int newButtonPress = 0;
// char currentButton;

// uint16_t resultCode;
// int buttonHeld = 0;
// int remoteState;
// int stateCounter = 0;
// int remotEeprom;

// int previousRemoteState;

// int butStateCounter = 1;
// int demoCounter = 1;

// int mode;

// int prevButtonHeld;

// bool cflag = 1;

// /* Initialize the irrecv part of the IRremote  library */
// IRrecv irrecv(RECV_PIN);
// decode_results results; // This will store our IR received codes
// uint16_t lastCode = 0; // This keeps track of the last code RX'd








// // GUItool: begin automatically generated code
// AudioInputI2S            i2s1;           //xy=139,91
// AudioAnalyzeFFT1024      fft1024;        //xy=467,147
// AudioAnalyzeRMS          rms1;           //xy=411,381
// AudioConnection          patchCord1(i2s1, 0, fft1024, 0);
// AudioConnection          patchCord2(i2s1, 0, rms1, 0);
// // GUItool: end automatically generated code

// // An array to hold the 8 frequency bands
// float reading[8];
// int fftArray[8];
// int fftSingle;

// // This is low-level noise that's subtracted from each frequency band:
// static const int noise[8] = {   // numbers generated using serial plotter at room tone at x10000 factor
//   20, 05, 04, 06, 22, 64, 68, 89            // these are converted x 0.0001 later on before subtraction
// };
// static const int binStart[8] = {   //
//   0, 2, 3, 5, 9, 27, 99, 227            //
// };
// static const int binEnd[8] = {   //
//   1, 2, 4, 8, 26, 98, 226, 511            //
// };
// static const int scale[9] = {
//   //300, 1000, 3000, 6000, 10000, 20000, 30000, 50000, 100000   // sensitivity setting (mulitplication factor)
//   50 ,   90,  200,  500,  1000,  3000,  6000, 12000, 30000      // lower sensitivity for Speaker Cloud
// };
// float eq[8] = {
//   1.0, 1.0, 0.7, 0.5, 1.0, 1.0, 1.5, 2.0   // individual channel scaling factor
// };

// static const int preview[] = {
//   10, 4, 10, 12, 50, 60, 25, 3, 2, 5, 5, 100, 60, 10, 5, 6, 7, 12, 5, 6, 9, 8
// };
// int previewCounter = 0;

// static const int wheelH[] = {
//     255,   30,  29,  29,  70, 90, 110, 125, 150, 175, 200, 225, 250, 0
// };

// static const int wheelS[] = {
//       0,  100, 200, 255, 255, 255, 255, 255, 255, 255, 255, 200, 255, 255
// };
// int wheelPosition;

// int speedOfAnimation[] = {
//   30, 20, 17, 12, 5, 1
// };
// int timeSpeed;

// //Lamp Mode Variables
// int rainbowCounter = 0;

// uint8_t gHue = 180;           // rotating "base color" used by many of the patterns

// //LED Variables
// #define DATA_PIN 4 //MOSI  // Green
// #define CLK_PIN 3  //SCK  // Blue
// #define LED_TYPE WS2801 //APA102
// #define COLOR_ORDER RGB
// CRGB leds[NUM_LEDS];

// int minLEDvalue[NUM_LEDS];
// int goingUp[NUM_LEDS];
// int currentValue[NUM_LEDS];

// int currentValueFade = 15;
// int goingUpFade = 1;

// int ledSingle1;
// int ledSingle2;
// int ledSingle3;

// int modeColor;
// int indcatorDots;
// uint8_t ledCimber = 8;
// int variableCounter = 9;  //global
// uint8_t pushAndHold = 0;
// uint8_t dotBrightness = 250;
// uint8_t dotBrightnessDirection = 1;
// int newEpprom;
// uint8_t automatedIndicator;
// int numberLoops = 50;
// int capReading;
// int aveCapReading;
// bool flag = 0;
// int flashCount = 1;
// int flashCountA = 1;
// int calm = 10;


// // Prototype Functions:
// void musicmode1();
// void musicmode2();
// void musicmode3();
// void musicmode4();
// void musicmode5();
// void lampMode1();
// void lampMode2();
// void lampMode3();
// void lampMode4();
// void rainbow(int startPos, int number, float deltaHue);
// void turnoffLEDs();
// void analyzeLevel();
// void analyzeFFT();
// void analyzeFFTall();
// void FFTreading(int FFTchannel);
// void eepromSet();
// void fetchSoundData();
// void timedLightening(int gap);
// void demo();
// void soundLightening();
// void strom();
// void flash(int hue, int saturation);
// void reset();


// void setup()
// { 
//   irrecv.enableIRIn(); // Start the IR receiver
//   AudioMemory(12);
//   Serial.println("Cloud v3.0");

//   pinMode(shunt1Pin, INPUT_PULLUP);    
//   pinMode(shunt2Pin, INPUT_PULLUP);  

//   shunt1 = digitalRead(shunt1Pin);  
//   shunt2 = digitalRead(shunt2Pin); 

//   if (shunt1 == LOW && shunt2 == HIGH)
//   { 
//     LED_ADJUSTED = 50; //used for RGBH Clouds that have all 50 LEDs
//     Serial.println("Pin 5");
//     }

//   else if (shunt1 == HIGH && shunt2 == LOW)
//   { 
//     LED_ADJUSTED = 17; //used for RGBM Clouds that only have 17 LEDs
//     Serial.println("Pin 6");
//     }

//   else if (shunt1 == LOW && shunt2 == LOW)
//   { 
//     LED_ADJUSTED = 10; //used for RGBS Clouds that have only 10 LEDs
//     Serial.println("BOTH");
//     }


//   FastLED.addLeds<LED_TYPE, DATA_PIN, CLK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
//   FastLED.setBrightness(255);
//   turnoffLEDs();
//   FastLED.show();
//   turnoffLEDs();
//   FastLED.show();
//   Serial.begin(9600);
//   delay(1000);  // Sanity Delay
//   turnoffLEDs();
//   FastLED.show();
//   for (int i = 0; i < LED_ADJUSTED; i++) {    
//     minLEDvalue[i] = random(1,150);       //fill up the minimum LED value array for Fairy Light Mode
//     currentValue[i] = random(1,254);      //fill up the current value array for Fairy Light Mode
//     goingUp[i] = random(0,1);             //fill up the going up value array for Fairy Light Mode
//   }
//   for (int i = 0; i < 1000; i++) {
//     for (int x = 0; x < LED_ADJUSTED; x++) {
//       if(goingUp[x] == 1){
//         currentValue[x]++;
//         if (currentValue[x] >= 255) {goingUp[x] = 0;}
//       }
//       else{
//         currentValue[x]--;
//         if (currentValue[x] <= minLEDvalue[x]) {goingUp[x] = 1;}
//       }
//     }
//   }
//   flash(100, 0);
//   turnoffLEDs();
//   FastLED.show();
  
//   //EEPROM.update(0, 1);       // uncomment to load default EPROM values
//   eepromSet();
//     // butStateCounter = 1;                  //
//     // timeSpeed = 2;                          //
//     // Bvariable = 8;                          //
//     // sensitivity = 4;                        //
//     // remoteState = BUTTON_2;
// }



// void loop()
// { 
//   remote();

//   //   shunt1 = digitalRead(shunt1Pin);  
//   // if (shunt1 == LOW)
//   // { 
//   //   //LED_ADJUSTED = 10; //used for RGBS Clouds that only have 10 LEDs
//   //   Serial.println("Pin 5");
//   //   }

//   //   shunt2 = digitalRead(shunt2Pin);  
//   // if (shunt2 == LOW)
//   // { 
//   //   //LED_ADJUSTED = 17; //used for RGBM Clouds that only have 17 LEDs
//   //   Serial.println("Pin 6");
//   //   }

//   if      (remoteState == BUTTON_1){
//      if (flashCount == 1){  
//        flash(100,0); 
//        Serial.print("flash white");
//        turnoffLEDs();
//        FastLED.show(); 
//        flashCount = 0; 
//        }
//   }
//   else if (remoteState == BUTTON_1_HELD){
//       //reset();
//   }
//   else if (remoteState == BUTTON_8){
//     analyzeFFTall(); 
//     musicmode3(); 
//   }
//   else if (remoteState == BUTTON_9){
//     lampMode4();  
//   }
//   else if (remoteState == BUTTON_2){
//     fetchSoundData();
//     musicmode4();  
//   }
//   else if (remoteState == BUTTON_5){
//     analyzeFFTall(); 
//     musicmode5();  
//   }
//   else if (remoteState == BUTTON_3){
//     lampMode2(); 
//   }
//   else if (remoteState == BUTTON_3_HELD){
//       //solid white mode
//       if (cflag == 1){
//         fill_solid( leds, LED_ADJUSTED, CHSV( wheelH[wheelPosition], wheelS[wheelPosition], 255));
//         FastLED.show();
//         delay(100);
//         fill_solid( leds, LED_ADJUSTED, CHSV( wheelH[wheelPosition], wheelS[wheelPosition], 255));
//         FastLED.show();
//         cflag = 0;
//     }
//   }

//   else if (remoteState == BUTTON_6){
//     lampMode3();
//   }
//   else if (remoteState == BUTTON_4){    
//     if (flashCount == 1){  
//        strom();
//        flashCount = 0;  
//       }
//     timedLightening(30);
//   }

//   else if (remoteState == BUTTON_7){
//     fetchSoundData();
//     soundLightening();
//   }


//   else if (remoteState == BUTTON_AUP)            // Global brightness
//   {
//     if(millis() - variableMillis > 1000){
//         upDownLeftRightReturn();
//      }
//       if(variableState == 0){             
//           if (Bvariable < 8){        Bvariable++;   }
//           FastLED.setBrightness(map(Bvariable,0,8,20,255));
//           fill_solid( leds, LED_ADJUSTED, CHSV(60,150,(map(Bvariable,0,8,20,250))));
//           FastLED.show();
//           Serial.print("Bvariable = ");
//           Serial.println(Bvariable);
//           EEPROM.update(3, Bvariable);
//         variableState = 1;
//     } 
//   }

//   else if (remoteState == BUTTON_BUP)            // sensitivity
//   {
//     if(millis() - variableMillis > 1000){
//         upDownLeftRightReturn();
//      }
//       if(variableState == 0){
//           if (sensitivity < 8){     sensitivity++;  }
//           fill_solid( leds, LED_ADJUSTED, CHSV(180,150,(map(sensitivity,0,8,20,250))));
//           FastLED.show();
//           Serial.print("sensitivity = ");
//           Serial.println(sensitivity);
//           EEPROM.update(4, sensitivity);
//         variableState = 1;
//     }    
//   }

//   // else if (remoteState == BUTTON_CUP)           // Speed of Animation
//   // {
//   //   if(millis() - variableMillis > 1000){
//   //       upDownLeftRightReturn();
//   //    }
//   //     if(variableState == 0){
        
//   //         if (timeSpeed < 5){          timeSpeed++;     }
//   //         fill_solid( leds, LED_ADJUSTED, CHSV(110,150,(map(timeSpeed,0,5,20,250))));
//   //         FastLED.show();
//   //         Serial.print("timeSpeed = ");
//   //         Serial.println(speedOfAnimation[timeSpeed]);
//   //         EEPROM.update(2, timeSpeed);
//   //       variableState = 1;
//   //   }    
//   // }

//   else if (remoteState == BUTTON_ADOWN)              // Global brightness
//   {
//     if(millis() - variableMillis > 1000){
//         upDownLeftRightReturn();
//      }
//       if(variableState == 0){
//           if (Bvariable > 0){     Bvariable--;   }
//           FastLED.setBrightness(map(Bvariable,0,8,20,255));
//           fill_solid( leds, LED_ADJUSTED, CHSV(60,150,(map(Bvariable,0,8,20,250))));
//           FastLED.show();
//           Serial.print("Bvariable = ");
//           Serial.println(Bvariable);
//           EEPROM.update(3, Bvariable);
//         variableState = 1;
//     }    
//   }

//   else if (remoteState == BUTTON_BDOWN)               // sensitivity
//   {
//     if(millis() - variableMillis > 1000){
//         upDownLeftRightReturn();
//      }
//       if(variableState == 0){
//           if (sensitivity > 0){     sensitivity--;  }
//           fill_solid( leds, LED_ADJUSTED, CHSV(180,150,(map(sensitivity,0,8,20,250))));
//           FastLED.show();
//           Serial.print("sensitivity = ");
//           Serial.println(sensitivity);
//           EEPROM.update(4, sensitivity);
//         variableState = 1;
//     }
//   }

//   // else if (remoteState == BUTTON_CDOWN)             // Speed of Animation
//   // {
//   //   if(millis() - variableMillis > 1000){
//   //       upDownLeftRightReturn();
//   //    }
//   //     if(variableState == 0){
//   //         if (timeSpeed > 0){          timeSpeed--;     }
//   //         fill_solid( leds, LED_ADJUSTED, CHSV(110,150,(map(timeSpeed,0,5,20,250))));
//   //         FastLED.show();
//   //         Serial.print("timeSpeed = ");
//   //         Serial.println(speedOfAnimation[timeSpeed]);
//   //         EEPROM.update(2, timeSpeed);
//   //       variableState = 1;
//   //   }  
//   // }

//   else if ((remoteState == BUTTON_CDOWN) || (remoteState == BUTTON_CUP))
//   {
//     for (int x = 0; x < LED_ADJUSTED; x++) {
//       leds[x] = CHSV(wheelH[wheelPosition], wheelS[wheelPosition], 200);
//      }
//     FastLED.show();

//     if(millis() - variableMillis > 1000){
//         upDownLeftRightReturn();
//      }
//   }
// }


//   // prepareModes();               // load in startup values for each mode
//   // runMode();                      // run the loop using selected mode
//   // soundLevel = map(aveCapReading,0,3000,0,115);   // visual debugging backup
//   // soundLevel = constrain(soundLevel,0,115);       // visual debugging backup
//   // musicmode1();                                   // visual debugging backup

// //*******************************       Cloud Modes    ******************************************//

// void timedLightening(int gap)
// {
//   EVERY_N_SECONDS(calm){
//     strom();
//     calm = random(gap);
//   }
// }

// void soundLightening()
// { 
//   //  if (dot != soundLevel){
//   //  Serial.println(soundLevel);
//   //  dot = soundLevel;
//   //  }
//   if (soundLevel > 180){    //was 250
//   strom();
//  }
// }

// void demo()
// {
//   if(demoCounter < 3000) { demoCounter++; }
//   else {demoCounter = 0 ; }

//   if(demoCounter > 0 && demoCounter < 15000){
//     lampMode1();
//   }
//   else if(demoCounter >= 1500 && demoCounter < 29990){
//     lampMode3();
//   }
//   else if(demoCounter >= 29990 && demoCounter < 30000){
//     timedLightening(30);
//   }
// }

// //*******************************       Music Modes    ******************************************//

// void fetchSoundData()    // fetch readings
// {
//   if (channel < 8){
//     analyzeFFT();
//   }
//   else if (channel == 8){
//     analyzeLevel();
//   }
// }

// void analyzeLevel() 
// {
//   if (rms1.available()) {
//     soundLevel = (rms1.read() - 0.0006) * scale[sensitivity];     // remove noise and scale
//     soundLevel = constrain(soundLevel, 0, 255);              // limit
//   }
// }

// void analyzeFFT() 
// {
//   if (fft1024.available()) {
//     FFTreading(channel);
//     soundLevel = fftArray[channel];
//   }
// }

// void analyzeFFTall() 
// {
//   if (fft1024.available()) {
//     for (int i = 0; i < 8; i++) {
//       FFTreading(i);
//     }
//   }
// }

// void FFTreading(int FFTchannel) 
// {
//   reading[FFTchannel] =  fft1024.read(binStart[FFTchannel], binEnd[FFTchannel]);
//   reading[FFTchannel] = reading[FFTchannel] - (noise[FFTchannel] * 0.0001);           // remove noise
//   fftArray[FFTchannel] = (reading[FFTchannel] * scale[sensitivity]) * eq[FFTchannel]; // scale
//   fftArray[FFTchannel] = constrain(fftArray[FFTchannel], 0, 255);                // limit
// }

// void musicmode1()   // Falling Dot
// { 
//   //fadeToBlackBy( leds, LED_ADJUSTED, 3);
//   turnoffLEDs();

//   soundLevel = (map(soundLevel,0,150,0,LED_ADJUSTED+1))-1;    //250
//   if (soundLevel > dot)  dot = soundLevel; // Keep dot on top of soundLevel
//   if (dot > LED_ADJUSTED)    dot = LED_ADJUSTED; // Keep dot from going out of frame

//   //turnoffLEDs();
//   if (soundLevel > 1){
//   for (int led = 0; led < dot; led++)
//   { // Start by Filling LEDS up to the soundLevel with dim white
//     leds[led] = CHSV( 100, 0, 255);
//    } 
//   }
//   //leds[dot] = CHSV( 100, 150, 255);
  
//   // for (int led = dot; led < LED_ADJUSTED; led++)
//   // { //make everything above the dot black
//   //   leds[led].setRGB(0, 0, 0);
//   // }
//   FastLED.show(); // send data to LEDs to display

//   if (++dotCount >= 40) {                   // make the dot fall slowly
//     dotCount = 0;
//     if (dot > 0) {
//       dot--;
//     }
//   }
// }

// void musicmode2()   // Middle Out 
// { 
//   soundLevel = map(soundLevel, 0, 255,0, LED_ADJUSTED);              // limit
//   turnoffLEDs();
//   for (int led = (LED_ADJUSTED - soundLevel) / 2; led < (soundLevel / 2) + (LED_ADJUSTED / 2); led++)
//   {
//     leds[led].setRGB(50, 50, 50);
//   }
//   if (soundLevel <= 0)  // NO SOUND
//   {                                    // If no sound (dot = 0)
//     leds[LED_ADJUSTED / 2].setRGB(80, 80, 80); // keep center dot illuminated
//   }
//   FastLED.show(); // send data to LEDs to display
//   delay(10);
// }

// void musicmode3()    // Ripple
// { 
//   for (int y = 0; y < 8; y++) // create 8 different LED sections of the Cloud each based on the 8 FFT channels
//   {
//     int bottomOfRipple = ((LED_ADJUSTED / 8) * y);                        //break the LEDs into groups of 8
//     if (bottomOfRipple <= 0){      bottomOfRipple = 0;    }           //lower limit

//     int topOfRipple = ((LED_ADJUSTED / 8) * y) + (LED_ADJUSTED/8);            //break the LEDs into groups of 8
//     if (topOfRipple >= LED_ADJUSTED){      topOfRipple = LED_ADJUSTED;  }     //upper limit

//     int rippleBrightness = fftArray[y] * 5;                           //scale the brightness up
//     rippleBrightness = constrain( rippleBrightness, 0, 255 );         //limit to floor and celling
//     for (int led = bottomOfRipple; led < topOfRipple; led++)
//     {
//       leds[led] = CHSV(wheelH[wheelPosition], wheelS[wheelPosition], rippleBrightness); // fill in LEDs according to the top and bottom of each section deffined above
//     }
//   }
//   FastLED.show();      //send data to LEDs
// }

// void musicmode4()   // Fade
// { 
//   if (soundLevel * 3 > dot){    dot = soundLevel * 3;  } // scale up by 3
//   if (dot > 255){    
//     dot = 255;
//     Serial.println("Mic Working!");  
//     }                       // upper limit

//   for (int led = 0; led < LED_ADJUSTED; led++)
//   {
//     leds[led] = CHSV( wheelH[wheelPosition], wheelS[wheelPosition], dot);    // prepare data for LEDs
//   }
//   FastLED.show();                                        // send data to LEDS
//   if (++dotCount >= 5) {                                 // make the brightness fall 1 level each five frames
//     dotCount = 0;
//     if (dot > 0) {
//       dot--;
//     }
//   }
// }

// void musicmode5()     // Colorful Music Mode
// {   
//   total = total - readings[readIndex];                 // subtract the last reading
//   readings[readIndex] = constrain((fftArray[0] + fftArray[1]),0,255);   // read data from the mic for BASS channels
//   total = total + readings[readIndex];                 // add the reading to the total
//   readIndex = readIndex + 1;                           // advance to the next position in the array
//   if (readIndex >= numReadings) { readIndex = 0; }     // if we're at the end of the array wrap around to the beginning
//   average = total / numReadings;                       // calculate the average

//   totalM = totalM - readingsM[readIndexM];             // subtract the last reading
//   readingsM[readIndexM] = constrain((fftArray[2] + fftArray[3]),0,255);   // read data from the mic for MID channels
//   totalM = totalM + readingsM[readIndexM];             // add the reading to the total
//   readIndexM = readIndexM + 1;                         // advance to the next position in the array
//   if (readIndexM >= numReadings) { readIndexM = 0; }   // if we're at the end of the array wrap around to the beginning
//   averageM = totalM / numReadings;                     // calculate the average

//   totalH = totalH - readingsH[readIndexH];             // subtract the last reading
//   readingsH[readIndexH] = constrain((fftArray[4] + fftArray[5] + fftArray[6] + fftArray[7]),0,255);   // read data from the mic for HIGH channels
//   totalH = totalH + readingsH[readIndexH];             // add the reading to the total
//   readIndexH = readIndexH + 1;                         // advance to the next position in the array
//   if (readIndexH >= numReadings) { readIndexH = 0; }   // if we're at the end of the array wrap around to the beginning
//   averageH = totalH / numReadings;                     // calculate the average

//     for (int x = 0; x < LED_ADJUSTED; x++) {
//     leds[x].setRGB(averageH, averageM, average);       //prepare data to send to LEDs
//   }
//   FastLED.show();                                      //send data to LEDs
// }

// //*******************************      Lamp Modes    ******************************************//

// void turnoffLEDs()
// {
//     for (int led = 0; led < NUM_LEDS; led++)        // changed to NUM_LEDS just in case it misses any extra LEDS
//   { //turn off LEDs
//     leds[led] = CHSV( 100, 0, 0);
//   }
// }

// void flash(int hue, int saturation)
// {
//      for (int led = 0; led < LED_ADJUSTED; led++) {
//         leds[led] = CHSV( hue, saturation, 200);
//       }
//       FastLED.show();
//       delay(200);
//       turnoffLEDs();
//       FastLED.show();
//       flashCount = 0;
// }

// void lampMode1()  // Neon
// {
//   rainbow(0, LED_ADJUSTED, 0.1);
//   FastLED.show();
// }

// void lampMode2()  // Fairy Light
// {
//     EVERY_N_MILLISECONDS_I(thistimer,10) {                //delay to slow down the animation to a calm breathing speed
//     thistimer.setPeriod(speedOfAnimation[timeSpeed]);     //setting to adjust the speed of the delay
//     for (int x = 0; x < LED_ADJUSTED; x++) {                  //this mode controls each LED individually where each LED has a dynamic setting about its fade level, fade direction and minimum fade value
//     if(goingUp[x] == 1){
//       currentValue[x]++;                                  //if the fade direction for this LED is up then add one brightness to the LED
//       if (currentValue[x] >= 255) {goingUp[x] = 0;}       //if the fade direction for this LED is reaches the maximum (255) change fade direction
//     }
//     else{
//       currentValue[x]--;                                  //if the fade direction for this LED is down then remove one brightness from the LED
//       if (currentValue[x] <= minLEDvalue[x]) {goingUp[x] = 1;} //if the fade direction for this LED is reaches the minimum change fade direction
//     }
//     leds[x] = CHSV( wheelH[wheelPosition], wheelS[wheelPosition], currentValue[x]);   //prepare the data for the LED given the above in global color
//   }
//   rainbowCounter = 0;                                     //indicator flag
//  }
//  FastLED.show();                                          //send data to LEDs
// }

// void lampMode3()  // Ombre
// {
//   rainbow(0, LED_ADJUSTED, 5);     //color fuction fill LEDs strip with color so that 50% of the colorwheel is visible     
//   FastLED.show();              //send data to LEDs
// }

// void lampMode4()  // Breathing Light
// {
//   EVERY_N_MILLISECONDS_I(thistimer,10) {                  //delay to slow down the animation to a calm breathing speed
//     thistimer.setPeriod(speedOfAnimation[timeSpeed]);     //setting to adjust the speed of the delay
//     if(goingUpFade == 1){                                 
//       currentValueFade++;                                 //if the animation is going up add one more to the brightness
//       if (currentValueFade >= 255) {goingUpFade = 0;}     //if the animation reaches the top start fading out
//     }
//     else{
//       currentValueFade--;                                 //if the animation is still fading out remove one from the brightness
//       if (currentValueFade <= 10) {goingUpFade = 1;}      //if the animation reaches the bottom (10) start fading in
//     }
//     for (int x = 0; x < LED_ADJUSTED; x++) {
//       leds[x] = CHSV(wheelH[wheelPosition], wheelS[wheelPosition], currentValueFade);    //HSV data to send to LEDs
//     }
//     rainbowCounter = 0;                                   //indicator flag
//   }
//   FastLED.show();                                         //send data to LEDs
// }

// void rainbow(int startPos, int number, float deltaHue)             //FASTLED function
// {
//     if (++rainbowCounter >= speedOfAnimation[timeSpeed] * 3) {     //rotate color wheel depending on speed setting variable
//       gHue++;
//       rainbowCounter = 0;
//     } 
//     fill_rainbow( &(leds[startPos]), number, gHue, deltaHue);     //send data to LEDs
// }

// void strom()
// {
//   for (int led = 0; led < int(random(LED_ADJUSTED)); led++) {          //turn on a random chain of LEDs white
//         leds[led] = CHSV( 100, 0, 255);
//         FastLED.show();
//       }

//   for (int led = 0; led < int(random(LED_ADJUSTED)); led++) {  leds[led] = CHSV( 100, 0, 255);    }   //turn on another random chain of LEDs white
//       FastLED.show();
//       delay(random(10, 40));                                                                      //random delay between 10-40
//   for (int led = 0; led < int(random(LED_ADJUSTED)); led++) {  leds[led] = CHSV( 100, 0, 0);      }   //turn off a random chain of LEDs
//       FastLED.show();
//       delay(random(10, 200));                                                                     //random delay between 10-200
//   for (int led = 0; led < LED_ADJUSTED; led++) {          leds[led] = CHSV( 100, 0, 0);      }        //turn off all LEDs
//       FastLED.show();
  
//   for (int led = 0; led < int(random(LED_ADJUSTED)); led++) {  leds[led] = CHSV( 100, 0, 255);    }   //turn on another random chain of LEDs white
//       FastLED.show();
//       delay(random(10, 50));                                                                      //random delay between 10-50
//   for (int led = 0; led < int(random(LED_ADJUSTED)); led++) {  leds[led] = CHSV( 100, 0, 0);      }   //turn off a random chain of LEDs
//       FastLED.show();
//       delay(random(10, 100));                                                                     //random delay between 10-100

//   for (int led = 0; led < LED_ADJUSTED; led++) {          leds[led] = CHSV( 100, 0, 0);      }        //turn off all LEDs
//       FastLED.show();

//   ledSingle1 = random(LED_ADJUSTED);                                                  //choose a random LED
//   leds[ledSingle1] = CHSV( 100, 0, 255);                                          //turn it on for a random time between 10-100 then turn it off. Do this twice.
//   FastLED.show();
//   delay(random (10, 100));
//   leds[ledSingle1] = CHSV( 100, 0, 0);
//   FastLED.show();
//   leds[ledSingle1] = CHSV( 100, 0, 255);
//   FastLED.show();
//   delay(random (40, 100));
//   leds[ledSingle1] = CHSV( 100, 0, 0);
//   FastLED.show();

//   if (random(1, 3) == 2) {                                                        //every 1 out of 3 times flash another LED
//     ledSingle1 = random (LED_ADJUSTED);
//     ledSingle2 = random (50);
//     leds[ledSingle1] = CHSV( 100, 0, 255);
//     if(ledSingle2 < LED_ADJUSTED){   leds[ledSingle2] = CHSV( 100, 0, 255);  }
//     FastLED.show();
//   }
//   else {
//     ledSingle1 = random (LED_ADJUSTED);                                               //every 2 out of 3 times flash a small group of LEDs on and off
//     ledSingle2 = ledSingle1 + 1;
//     ledSingle3 = ledSingle2 + 2;
//     leds[ledSingle1] = CHSV( 100, 0, 255);
//     FastLED.show();
//     delay(random (20));
//     if(ledSingle2 < LED_ADJUSTED){   leds[ledSingle2] = CHSV( 100, 0, 255); }
//     FastLED.show();
//     delay(random (20));
//     if(ledSingle3 < LED_ADJUSTED){   leds[ledSingle3] = CHSV( 100, 0, 255); }
//     FastLED.show();
//   }

//   for (int led = 0; led < LED_ADJUSTED; led++) {          leds[led] = CHSV( 100, 0, 0);      }    //turn off all LEDS
//       FastLED.show();

//   if (random(1, 3) == 2) {                                                        //repeat previous small flashes
//     ledSingle1 = random (LED_ADJUSTED);
//     ledSingle2 = random (50);
//     leds[ledSingle1] = CHSV( 100, 0, 255);
//     if(ledSingle2 < LED_ADJUSTED){   leds[ledSingle2] = CHSV( 100, 0, 255);  }
//     FastLED.show();
//   }
//   else {
//     ledSingle1 = random (LED_ADJUSTED);
//     ledSingle2 = ledSingle1 + 1;
//     ledSingle3 = ledSingle2 + 2;
//     leds[ledSingle1] = CHSV( 100, 0, 255);
//     FastLED.show();
//     delay(random (20));
//     if(ledSingle2 < LED_ADJUSTED){   leds[ledSingle2] = CHSV( 100, 0, 255); }
//     FastLED.show();
//     delay(random (20));
//     if(ledSingle3 < LED_ADJUSTED){   leds[ledSingle3] = CHSV( 100, 0, 255); }
//     FastLED.show();
//   }

//   for (int led = 0; led < LED_ADJUSTED; led++) {          leds[led] = CHSV( 100, 0, 0);      }  //turn off all LEDS
//       FastLED.show();
//       delay(random(10, 200));                                                               //random delay between 10-100
// } 


// //*******************************      State Modes    ******************************************//

// void eepromSet() 
// { //newEpprom = 1;          // first run eprom data save
//   newEpprom = EEPROM.read(0);          // first run eprom data save
//   if (newEpprom != 73) {
    
//     Serial.println("New EPROM!");
//     // eeprom values:
//     newEpprom = 73;
//     remoteState = 1000;
//     remotEeprom = 0;
//     previousRemoteState = remoteState;
//     butStateCounter = 1;                  //
//     timeSpeed = 2;                            //
//     Bvariable = 8;                          //
//     FastLED.setBrightness(map(Bvariable,0,8,50,255)); // set master brightness control
//     sensitivity = 3;                        //
//     wheelPosition = 0;

//     EEPROM.update(0, newEpprom);
//     EEPROM.update(1, butStateCounter);
//     EEPROM.update(2, timeSpeed);
//     EEPROM.update(3, Bvariable);
//     EEPROM.update(4, sensitivity);
//     EEPROM.update(5, remotEeprom);
//     EEPROM.update(6, wheelPosition);

//   }

//   else { 
//     Serial.println("Old EPROM!");                           //not new eeprom
//     butStateCounter =    (int)EEPROM.read(1);
//     timeSpeed =          (int)EEPROM.read(2);
//     Bvariable =          (int)EEPROM.read(3);
//     sensitivity =        (int)EEPROM.read(4);
//     remotEeprom =        (int)EEPROM.read(5);
//     remoteState = BUTTON_ARRAY2[remotEeprom];  // 0 = BUTTON_1, 1 = BUTTON_4, 2 = BUTTON_2, 3 = BUTTON_3
//     wheelPosition =      (int)EEPROM.read(6);

//     Serial.print("remotEeprom :   ");
//     Serial.println(remotEeprom);
//     Serial.print("remoteState :   ");
//     Serial.println(remoteState);
//     Serial.print("butStateCounter :   ");
//     Serial.println(butStateCounter);
//     Serial.print("timeSpeed :   ");
//     Serial.println(timeSpeed);
//     Serial.print("sensitivity :   ");
//     Serial.println(sensitivity);
//     Serial.print("Bvariable :   ");
//     Serial.println(Bvariable);
//     Serial.print("wheelPosition :   ");
//     Serial.println(wheelPosition);

//     if (butStateCounter < 1 || butStateCounter > 4){    // safety in case bad eprom reading
//       butStateCounter = 1;
//     }
//     if (sensitivity < 0 || sensitivity > 9){    // safety in case bad eprom reading
//       sensitivity = 3;
//     }
//     if (Bvariable < 0 || Bvariable > 9){    // safety in case bad eprom reading
//       Bvariable = 3;
//     }
//     if (remotEeprom < 0 || remotEeprom > 10){    // safety in case bad eprom reading
//       remotEeprom = 0;
//     }
    
//     if (remotEeprom == 9){
//       fill_solid( leds, LED_ADJUSTED, CHSV( wheelH[wheelPosition], wheelS[wheelPosition], 255));
//       FastLED.show();
//       delay(100);
//       fill_solid( leds, LED_ADJUSTED, CHSV( wheelH[wheelPosition], wheelS[wheelPosition], 255));
//       FastLED.show();
//     }

//     if (wheelPosition < 0 || wheelPosition > 13){    // safety in case bad eprom reading
//       wheelPosition = 0;
//     }
//     FastLED.setBrightness(map(Bvariable,0,8,50,255)); // set master brightness control
//   }
// }

// //*******************************      IR Functions    ******************************************//


// void remote() 
// {


//   if (millis() - checking > 250)
//   {

//     if (irrecv.decode(&results))     //check remote
//     {

//       resultCode = (results.value & 0xFFFF);

//       Serial.print("Remote Code");
//       Serial.println(resultCode);


//       if (resultCode == 0xFFFF) {     //button held check
//         buttonHeld++;

//         if (buttonHeld >= 8) {                                   // Button Holds
//              Serial.print("buttonHeld :    ");
//              Serial.println(currentButton);
            
//             if (currentButton == 'A') {
//               //remoteState = BUTTON_1_HELD;
//               reset();
//             }
//             if (currentButton == 'C') {
//               //remoteState = BUTTON_C_HELD;
//               cflag = 1;
//               EEPROM.update(5, 9);  // EEPROM Save 3 = BUTTON_3
//               previousRemoteState = remoteState;
//               remoteState = BUTTON_3_HELD;
//             }
//           }
        
//       }
//       else {
//         for (int i = 0; i < 15; i++) 
//         {              //compare against each of the button codes
//           if (resultCode == BUTTON_ARRAY[i]) {

//             // ACTUAL BUTTON
//             newButtonPress = 1;

//             if (resultCode == BUTTON_1) {
//               currentButton = 'A';
//             }
//             else if (resultCode == BUTTON_4) {
//               currentButton = 'D';
//               flashCountA = 1;
//             }
//             else if (resultCode == BUTTON_2) {
//               currentButton = 'B';
//             }
//             else if (resultCode == BUTTON_3) {
//               currentButton = 'C';
//             }
//             else if (resultCode == BUTTON_5) {
//               currentButton = 'E';
//             }
//             else if (resultCode == BUTTON_6) {
//               currentButton = 'F';
//             }
//             else if (resultCode == BUTTON_8) {
//               currentButton = 'H';
//             }
//             else if (resultCode == BUTTON_9) {
//               currentButton = 'I';
//             }
//             else if (resultCode == BUTTON_AUP) {
//               currentButton = 'J';
//                 upDownLeftRightRemote();
//                 remoteState = BUTTON_AUP;
//               }
//             else if (resultCode == BUTTON_ADOWN) {
//               currentButton = 'P';
//                 upDownLeftRightRemote();
//                 remoteState = BUTTON_ADOWN;
//               }

//             else if (resultCode == BUTTON_BUP) {
//               currentButton = 'K';
//                 upDownLeftRightRemote();
//                 remoteState = BUTTON_BUP;
//               }
//             else if (resultCode == BUTTON_BDOWN) {
//               currentButton = 'Q';
//                 upDownLeftRightRemote();
//                 remoteState = BUTTON_BDOWN;
//               }

//             else if (resultCode == BUTTON_CUP) {
//               currentButton = 'L';
//                 if (wheelPosition < 13){       
//                   wheelPosition++;  
//                   EEPROM.update(6, wheelPosition);  // EEPROM Save 
//                 }
//                 else {                         
//                   wheelPosition = 0; 
//                   EEPROM.update(6, wheelPosition);  // EEPROM Save
//                   }
//               upDownLeftRightRemote();
//               remoteState = BUTTON_CUP;
//               }
//             else if (resultCode == BUTTON_CDOWN) {
//                 currentButton = 'R';
//                 if (wheelPosition > 0){        
//                   wheelPosition--;   
//                   EEPROM.update(6, wheelPosition);  // EEPROM Save
//                   }
//                 else {                        
//                   wheelPosition = 13; 
//                   EEPROM.update(6, wheelPosition);  // EEPROM Save
//                   }
//                 upDownLeftRightRemote();
//                 remoteState = BUTTON_CDOWN;
//               }

//             else if (resultCode == BUTTON_7) {
//               currentButton = 'G';
//               flashCountA = 1;
//             }

            
//             //Serial.println("Comparing Button Code");

//           }
//           else {

//             // NOT A REAL BUTTON
//             //       #if defined(DEVMODE)
//             //       if (newButtonPress == 0){
//             //         Serial.print("NOT A REAL BUTTON    ");
//             //         Serial.println(currentButton);
//             //         }
//             //       #endif
//           }

//         }
//         buttonHeld = 0;
//       }
//         irrecv.resume(); // Receive the next value
//     }
//     else {

//       // THIS IS WHERE BUTTONS ARE SET GIVEN A NEW BUTTON PRESS

//       if (newButtonPress == 1) {
//         newButtonPress = 0;

//         if (buttonHeld > 8) { // number of seconds/ 4-1       // Button Holds
//           // Current Button Held
          
//         }


//         else {                                         // Button Single Presses
//             // Current Button
//             Serial.println(currentButton);

//             if (currentButton == 'A') {
//               EEPROM.update(5, 0);  // EEPROM Save 0 = BUTTON_1
//               previousRemoteState = remoteState;
//               remoteState = BUTTON_1;
//               flashCount = 1;
//               Serial.println("Off");
//               turnoffLEDs();
//               FastLED.show();
//             }
//             else if (currentButton == 'D') {
//               EEPROM.update(5, 3);  // EEPROM Save 1 = BUTTON_4
//               previousRemoteState = remoteState;
//               remoteState = BUTTON_4;
//               flashCount = 1;
//               turnoffLEDs();
//             }
//             else if (currentButton == 'B') {
//               EEPROM.update(5, 1);  // EEPROM Save 2 = BUTTON_2
//               previousRemoteState = remoteState;
//               remoteState = BUTTON_2;
//               flash(100,0); 
//               turnoffLEDs();
//               FastLED.show(); 
//             }
//             else if (currentButton == 'C') {
//               EEPROM.update(5, 2);  // EEPROM Save 3 = BUTTON_3
//               previousRemoteState = remoteState;
//               remoteState = BUTTON_3;
//               turnoffLEDs();
//             }
//             else if (currentButton == 'G') {
//               EEPROM.update(5, 6);  // EEPROM Save 4 = BUTTON_7
//               previousRemoteState = remoteState;
//               remoteState = BUTTON_7;
//               flash(100,0); 
//               turnoffLEDs();
//               FastLED.show(); 
//             }
//             else if (currentButton == 'H') {
//               previousRemoteState = remoteState;
//               remoteState = BUTTON_8;
//               EEPROM.update(5, 7);  // EEPROM Save 5 = BUTTON_8
//               flash(100,0); 
//               turnoffLEDs();
//               FastLED.show(); 
//             }
//             else if (currentButton == 'I') {
//               previousRemoteState = remoteState;
//               remoteState = BUTTON_9;
//               EEPROM.update(5, 8);  // EEPROM Save 6 = BUTTON_9
//               turnoffLEDs();
//             }
//             else if (currentButton == 'E') {
//               previousRemoteState = remoteState;
//               remoteState = BUTTON_5;
//               EEPROM.update(5, 4);  // EEPROM Save 7 = BUTTON_5
//               flash(100,0); 
//               turnoffLEDs();
//               FastLED.show(); 
//             }
//             else if (currentButton == 'F') {
//               previousRemoteState = remoteState;
//               remoteState = BUTTON_6;
//               EEPROM.update(5, 5);  // EEPROM Save 8 = BUTTON_6
//               turnoffLEDs();
//             }
//         }
//         //Serial.println("Preparing");
//         //prepareModes();               // load in startup values for each mode and run preview
//         //Serial.println("Prepaired");
//         buttonHeld = 0;

//       }

//       //find the relevant remoteState value within BUTTON_ARRAY - save to eeprom as remotEeprom
//       // for (int x = 0; x < 3; x++){
//       //     if(remoteState == BUTTON_ARRAY[x]){
//       //       remotEeprom = x;
//       //  }
//       //  if (remotEeprom != prevRemotEeprom){
//       //     EEPROM.update(5, remotEeprom);
//       //     prevRemotEeprom = remotEeprom;
//       //  }
//       // }
      

//     }
//     checking = millis();
//   }
// }


// void upDownLeftRightReturn() 
// {      //go back into prev mode

//   variableState = 1;
//   cflag = 1;
//   remoteState = previousRemoteState;
//   variableMillis = millis();
//   turnoffLEDs();
//   FastLED.show();
// }

// void upDownLeftRightRemote() 
// {

//   variableState = 0;

//   if ((remoteState != BUTTON_CDOWN) && (remoteState != BUTTON_CUP) && (remoteState != BUTTON_AUP) && (remoteState != BUTTON_ADOWN) && (remoteState != BUTTON_BUP) && (remoteState != BUTTON_BDOWN))
//   {
//     if (remoteState == 1000)
//     {
//       previousRemoteState = BUTTON_1;
//     }
//     else {
//       previousRemoteState = remoteState;
//       cflag = 1;
//     }

//   }

//   variableMillis = millis();
//   irrecv.resume(); // Receive the next value
//   remote();
// }

// void upDownLeftRightRemoteHeld() 
// {

//   variableState = 0;
//   variableMillis = millis();
//   irrecv.resume(); // Receive the next value
//   remote();
// }

// void reset()
// {
//   Serial.print("RESET!");
//   fill_solid( leds, LED_ADJUSTED, CHSV(0, 0, 0));
//   FastLED.show();
//   delay(100);
//   fill_solid( leds, LED_ADJUSTED, CHSV(255, 255, 200));
//   FastLED.show();
//   delay(1000);
//   fill_solid( leds, LED_ADJUSTED, CHSV(0, 0, 0));
//   FastLED.show();
//   delay(300);
//   fill_solid( leds, LED_ADJUSTED, CHSV(255, 255, 200));
//   FastLED.show();
//   delay(1000);
//   fill_solid( leds, LED_ADJUSTED, CHSV(0, 0, 0));
//   FastLED.show();
//   newEpprom = 1;
//   EEPROM.update(0, newEpprom);
//   eepromSet();
      
// }

