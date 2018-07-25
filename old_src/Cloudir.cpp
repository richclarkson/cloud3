#include <Arduino.h>
#include <IRremote.h> // Include the IRremote library
#define NUM_BUTTONS 9 // The remote has 9 buttons

int RECV_PIN = 4;       //

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
int locked;
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



void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    irrecv.enableIRIn(); // Start the receiver

}

void loop() {
  // put your main code here, to run repeatedly:
 remote();
}


void remote() {


  if (millis() - checking > 250)
  {


    if (irrecv.decode(&results))
    {

      resultCode = (results.value & 0xFFFF);

      Serial.print("Remote Code");
      Serial.println(resultCode);


      if (resultCode == 0xFFFF) {
        buttonHeld++;


        if (currentButton == 'L') {
          if (locked == 0) {
            upDownLeftRightRemoteHeld();
            remoteState = BUTTON_LEFT;
            colourVariable = 20;
          }
        }
        else if (currentButton == 'R') {
          if (locked == 0) {
            upDownLeftRightRemoteHeld();
            remoteState = BUTTON_RIGHT;
            colourVariable = 20;
          }
        }
        else if (currentButton == 'U') {
          if (locked == 0) {
            upDownLeftRightRemoteHeld();
            remoteState = BUTTON_UP;
          }
        }
        else if (currentButton == 'D') {
          if (locked == 0) {
            upDownLeftRightRemoteHeld();
            remoteState = BUTTON_DOWN;
          }
        }

        if (buttonHeld == 10) {

          if ((remoteState != BUTTON_LEFT) && (remoteState != BUTTON_RIGHT)) {
            //flashon (N_PIXELS, 0, 100, 100, 100);
            //runningFlashOFF(N_PIXELS, 0, 100, 100, 100);
            //flashon (N_PIXELS, 0, 100, 100, 100);
            //runningFlashOFF(N_PIXELS, 0, 100, 100, 100);
          }
        }

      }
      else {

        for (int i = 0; i < 9; i++) {              //compare against each of the button codes
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
              if (locked == 0) {
                upDownLeftRightRemote();
                remoteState = BUTTON_UP;
              }
            }
            else if (resultCode == BUTTON_DOWN) {
              currentButton = 'D';
              if (locked == 0) {
                upDownLeftRightRemote();
                remoteState = BUTTON_DOWN;
              }
            }
            else if (resultCode == BUTTON_LEFT) {
              currentButton = 'L';
              if (locked == 0) {
                upDownLeftRightRemote();
                remoteState = BUTTON_LEFT;
                colourVariable = 4;
              }
            }
            else if (resultCode == BUTTON_RIGHT) {
              currentButton = 'R';
              if (locked == 0) {
                upDownLeftRightRemote();
                remoteState = BUTTON_RIGHT;
                colourVariable = 4;
              }
            }
            else if (resultCode == BUTTON_CIRCLE) {
              currentButton = 'O';
            }

            newButtonPress = 1;

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


        if (buttonHeld > 9) { // number of seconds/ 4-1       // Button Holds
          // Current Button Held

          Serial.print("buttonHeld :    ");
          Serial.println(currentButton);

          if (locked == 0) { //IF NOT LOCKED

            if (currentButton == 'O') {
              remoteState = BUTTON_CIRCLE_HELD;
            }
            else if (currentButton == 'A') {
              previousRemoteState = remoteState;
              remoteState = BUTTON_A_HELD;
            }
            else if (currentButton == 'B') {
              previousRemoteState = remoteState;
              remoteState = BUTTON_B_HELD;
            }
          }

          if (currentButton == 'P') {
            remoteState = BUTTON_POWER_HELD;
          }

          else if (currentButton == 'C') {
            previousRemoteState = remoteState;
            remoteState = BUTTON_C_HELD;
          }


        }


        else {                                         // Button Single Presses


          if (locked == 0) { // IF NOT LOCKED

            // Current Button
            Serial.println(currentButton);




            if (currentButton == 'P') {
              remoteState = BUTTON_POWER;
            }
            else if (currentButton == 'A') {
              remoteState = BUTTON_A;
            }
            else if (currentButton == 'B') {
              remoteState = BUTTON_B;
            }
            else if (currentButton == 'C') {
              remoteState = BUTTON_C;
            }
          }


          if (currentButton == 'O') {
            previousRemoteState = remoteState;
            remoteState = BUTTON_CIRCLE;
          }
        }

        buttonHeld = 0;

      }
      //EEPROM.write(0, remoteState);             //fix this later
    }

    checking = millis();
  }

}




void upDownLeftRightReturn() {      //go back into prev mode

  variableState = 1;
  remoteState = previousRemoteState;
  variableMillis = millis();
  //flashoff (N_PIXELS, 0, 0, 0, 0);
}



void upDownLeftRightRemote() {

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

void upDownLeftRightRemoteHeld() {

  variableState = 0;
  variableMillis = millis();
  irrecv.resume(); // Receive the next value
  remote();
}


