
/*
  Cloud Xbee Remote 2.0
  2021 Richard Clarkson Studio
*/


#include <Arduino.h>
#include <IRremote.h>



//Global Varriables
int channel = 8;
int sensitivity;  // 0-8 where 8 = maximum sensitivity
int Bvariable;  // brightness

//xBee Varriables
String xbeeValue;
int modeValue = 0;
int counter1 = 1;

int relayState = 0;


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
const uint16_t BUTTON_9_HELD = 24;
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

uint16_t BUTTON_ARRAY2[10] = {BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4, BUTTON_5, BUTTON_6, BUTTON_7, BUTTON_8, BUTTON_9, BUTTON_1_HELD};


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

bool cflag = 1;

/* Initialize the irrecv part of the IRremote  library */
IRrecv irrecv(RECV_PIN);
decode_results results; // This will store our IR received codes
uint16_t lastCode = 0; // This keeps track of the last code RX'd



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
    255,   30,  29,  29,  70, 90, 110, 125, 150, 175, 200, 225, 250, 0
};

static const int wheelS[] = {
      0,  100, 200, 255, 255, 255, 255, 255, 255, 255, 255, 200, 255, 255
};
int wheelPosition;

int speedOfAnimation[] = {
  30, 20, 17, 12, 5, 1
};
int timeSpeed;

//Lamp Mode Variables
int rainbowCounter = 0;

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






void setup()
{ 
  Serial.begin(19200);
  Serial1.begin(9600); 
  irrecv.enableIRIn(); // Start the IR receiver
  //Serial.begin(9600);
}



void loop()
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
             // reset();
             Serial1.write("0");
            }
          }
      }
      else {
            newButtonPress = 1;

            if (resultCode == BUTTON_1) {
              currentButton = 'A';
            }
            else if (resultCode == BUTTON_4) {
              currentButton = 'D';
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
              Serial1.write("a");
              }
            else if (resultCode == BUTTON_ADOWN) {
              currentButton = 'P';
               Serial1.write("b");
              }

            else if (resultCode == BUTTON_BUP) {
              currentButton = 'K';
                Serial1.write("n");
              }
            else if (resultCode == BUTTON_BDOWN) {
              currentButton = 'Q';
                Serial1.write("r");
              }

            else if (resultCode == BUTTON_CUP) {
              currentButton = 'L';
                if (wheelPosition < 13){       
                  wheelPosition++;  
                }
                else {                         
                  wheelPosition = 0; 
                  }
              remoteState = BUTTON_CUP;
              }
            else if (resultCode == BUTTON_CDOWN) {
                currentButton = 'R';
                if (wheelPosition > 0){        
                  wheelPosition--;   
                  }
                else {                        
                  wheelPosition = 13; 
                  }
                remoteState = BUTTON_CDOWN;
              }

            else if (resultCode == BUTTON_7) {
              currentButton = 'G';
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
              Serial1.write("1");
            }
            else if (currentButton == 'D') {
              Serial1.write("2");
            }
            else if (currentButton == 'B') {
               Serial1.write("4");
            }
            else if (currentButton == 'C') {
               Serial1.write("3");
            }
            else if (currentButton == 'G') {
                Serial1.write("7");
            }
            else if (currentButton == 'H') {
               Serial1.write("6");
            }
            else if (currentButton == 'I') {
                Serial1.write("9");
            }
            else if (currentButton == 'E') {
                Serial1.write("5");
            }
            else if (currentButton == 'F') {
                Serial1.write("8");
            }
        }
        buttonHeld = 0;
      }
    }
    checking = millis();
  }
}



void serialEvent1(){
  while (Serial1.available()) {
    xbeeValue = Serial1.read();
    Serial.println(xbeeValue);
    if (xbeeValue == '1'){           // 1 
              previousRemoteState = remoteState;
              remoteState = BUTTON_1;
    }
    else if (xbeeValue == '2'){           // 2 
              previousRemoteState = remoteState;
              remoteState = BUTTON_4;
    }
    else if (xbeeValue == '4'){      //was 3 should be 4          // 3
              previousRemoteState = remoteState;
              remoteState = BUTTON_2;
    }
    else if (xbeeValue == '9'){     //was 4 but is broken
              previousRemoteState = remoteState;
              remoteState = BUTTON_3;
    }
    else if (xbeeValue == '3'){     //was 5 should be 3
              previousRemoteState = remoteState;
              remoteState = BUTTON_7;
    }
    else if (xbeeValue == '6'){
      previousRemoteState = remoteState;
              remoteState = BUTTON_8;
    }
    else if (xbeeValue == '7'){
      previousRemoteState = remoteState;
              remoteState = BUTTON_9;
    }
    else if (xbeeValue == '5'){     //was 8 should be 5
      previousRemoteState = remoteState;
              remoteState = BUTTON_5;
    }
    else if (xbeeValue == '8'){       //was 9 should be 8
      previousRemoteState = remoteState;
              remoteState = BUTTON_6;
    }


    else if (xbeeValue == '0'){      //reset
      //reset();
    }
    



    else if (xbeeValue == "a"){      // BULB on / off
      //digitalWrite(relayPin, HIGH);
      relayState = HIGH;

    }
    else if (xbeeValue == "b"){
      //digitalWrite(relayPin, LOW);
      relayState = LOW;

    }


    else if (xbeeValue == "c"){        // settings
      Bvariable = 0;
    }
    else if (xbeeValue == "d"){
      Bvariable = 1;
    }
    else if (xbeeValue == "e"){
      Bvariable = 2;

    }
    else if (xbeeValue == "f"){
      Bvariable = 3;

    }
    else if (xbeeValue == "g"){
      Bvariable = 4;

    }
    else if (xbeeValue == "h"){
      Bvariable = 5;

    }
    else if (xbeeValue == "i"){
      Bvariable = 6;

    }
    else if (xbeeValue == "j"){
      Bvariable = 7;

    }
    else if (xbeeValue == "k"){
      Bvariable = 8;

    }


    else if (xbeeValue == "l"){
      sensitivity = 0;
    }
    else if (xbeeValue == "m"){
      sensitivity = 1;

    }
    else if (xbeeValue == "n"){
      sensitivity = 2;

    }
    else if (xbeeValue == "o"){
      sensitivity = 3;

    }
    else if (xbeeValue == "p"){
      sensitivity = 4;

    }
    else if (xbeeValue == "q"){
      sensitivity = 5;

    }
    else if (xbeeValue == "r"){
      sensitivity = 6;

    }
    else if (xbeeValue == "s"){
      sensitivity = 7;

    }
    else if (xbeeValue == "t"){
      sensitivity = 8;

    }


    else if (xbeeValue == "A"){
      wheelPosition = 0;  

    }
    else if (xbeeValue == "B"){
      wheelPosition = 1;  

    }
    else if (xbeeValue == "C"){
      wheelPosition = 2;  

    }
    else if (xbeeValue == "D"){
      wheelPosition = 3;  

    }
    else if (xbeeValue == "E"){
      wheelPosition = 4;  

    }
    else if (xbeeValue == "F"){
      wheelPosition = 5;  

    }
    else if (xbeeValue == "G"){
      wheelPosition = 6;  

    }
    else if (xbeeValue == "H"){
      wheelPosition = 7;  

    }
    else if (xbeeValue == "I"){
      wheelPosition = 8;  

    }
    else if (xbeeValue == "J"){
      wheelPosition = 9;  

    }
    else if (xbeeValue == "K"){
      wheelPosition = 10;  

    }
    else if (xbeeValue == "L"){
      wheelPosition = 11;  

    }
    else if (xbeeValue == "M"){
      wheelPosition = 12;  

    }
    else if (xbeeValue == "N"){
      wheelPosition = 13;  

    }

  

  }
}