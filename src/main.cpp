
  #include <FastLED.h>
  #include <EEPROM.h>
  
  
  
  FASTLED_USING_NAMESPACE
  
  #if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
  #warning "Requires FastLED 3.1 or later; check github for latest code."
  #endif
  
  #define DATA_PIN    4
  //#define CLK_PIN   4
  #define LED_TYPE    WS2812B
  #define COLOR_ORDER RGB
  #define NUM_LEDS    190  //(96 each section)
  CRGB leds[NUM_LEDS];
  
  #define BRIGHTNESS          255
  #define FRAMES_PER_SECOND  120   //might need to bump down to 60 as was redefined lower down by accident.

  int minLEDvalue[NUM_LEDS];
int goingUp[NUM_LEDS];
int currentValue[NUM_LEDS];

int currentValueFade = 15;
int goingUpFade = 1;
  
  const int lowerHalfend = 92;
  const int upperHalfstart = 93;
  
  int mode = 0;
  int clicked = 0;
  int firstrun = 1;
  int twinkle1 = 1;
  int twinkle2 = 1;
  int twinkle3 = 1;
  int twinkle4 = 1;
  int twinkle5 = 1;
  int target1 = random16(5,255);
  int target2 = random16(5,255);
  int target3 = random16(5,255);
  int target4 = random16(5,255);
  int target5 = random16(5,255);
  int pos = random16(0,NUM_LEDS);
  int pos2 = random16(0,NUM_LEDS);
  int pos3 = random16(0,NUM_LEDS);
  int pos4 = random16(1,lowerHalfend);
  int pos5 = random16(upperHalfstart,NUM_LEDS);
  int fadeCounter = 0;
  int dotCount = 0;
  long thunderStrike = 0;
  long thunderThreshold = 1000;
  long auroraCounter = 0;
  long auroraThreshold = 100;  

  
  const int pumpPin = 11;
  const int fanPin = 10;
  const int brightLEDPin = 9;
  const int mister1Pin = 14;
  const int mister2Pin = 15;
  const int extractionPin = 13;
  
  const int ButtonLEDpinR = 3;
  const int ButtonLEDpinG = 5;
  const int ButtonLEDpinB = 6;
  
  const int buttonPin1 = 7;     // the number of the pushbutton pin
  int buttonState1 = 0;         // variable for reading the pushbutton status
  const int buttonPin2 = 8;     // the number of the pushbutton pin
  int buttonState2 = 0;         // variable for reading the pushbutton status
  const int buttonPin3 = 17;     // the number of the pushbutton pin
  int buttonState3 = 0;         // variable for reading the pushbutton status
  const int buttonPin4 = 18;     // the number of the pushbutton pin
  int buttonState4 = 0;         // variable for reading the pushbutton status
  const int buttonPin5 = 19;     // the number of the pushbutton pin
  int buttonState5 = 0;         // variable for reading the pushbutton status
  const int buttonPin6 = 20;     // the number of the pushbutton pin
  int buttonState6 = 0;         // variable for reading the pushbutton status

  int newEpprom;
  
  long  timma = 700;
  int counterThreshold = 100;
  //#define FRAMES_PER_SECOND 60


  void eepromSet(); 
  void checkButtons();
  void RGB_color(int red_light_value, int green_light_value, int blue_light_value);
  void turnoffLEDs();
  void RGB_LEDS();
  void twinkle();
  void volcano();
  void aurora();

  
  
  void setup() {
    
    delay(500); // 3 second delay for recovery
    Serial.begin(19200);
    Serial1.begin(9600);
    Serial.println("Starting Up");
  
    pinMode(ButtonLEDpinR, OUTPUT);
    pinMode(ButtonLEDpinG, OUTPUT);
    pinMode(ButtonLEDpinB, OUTPUT);

    

    pinMode(pumpPin, OUTPUT);
    pinMode(fanPin, OUTPUT);
    pinMode(brightLEDPin, OUTPUT);
    pinMode(mister1Pin, OUTPUT);
    pinMode(mister2Pin, OUTPUT);
    pinMode(extractionPin, OUTPUT);
  
    pinMode(buttonPin1, INPUT_PULLUP);
    pinMode(buttonPin2, INPUT_PULLUP);
    pinMode(buttonPin3, INPUT_PULLUP); 
    pinMode(buttonPin4, INPUT_PULLUP); 
    pinMode(buttonPin5, INPUT_PULLUP); 
    pinMode(buttonPin6, INPUT_PULLUP); 

    
    
    // tell FastLED about the LED strip configuration
    FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS);
  
    // set master brightness control
    FastLED.setBrightness(BRIGHTNESS);

    Serial.println("here i am");
  
    turnoffLEDs();
    FastLED.show();

    RGB_color(0, 0, 0); // White      White

    //mode = 12;
    eepromSet();
    Serial.println("end of setup");
  }
  
  uint8_t gHue = 0; // rotating "base color" used by many of the patterns
    
  void loop()
  {
    Serial.print("loop");
    delay(1000);
    checkButtons();

   if (mode == 1){           // Sunlight
     if (firstrun == 1){
       for (int i = 0; i <= 255; i++) {     
           analogWrite(brightLEDPin,i);
           delay(5);
         }
       firstrun = 0;
     }
     //do nothing
   }

   else if (mode == 2){           // Twilight
     if (firstrun == 1){
       // do nothing
       firstrun = 0;
     }
     //twinkling white RGB LEDs
     twinkle();
     }


    if (mode == 3){           // Heavy Rain
     if (firstrun == 1){
       digitalWrite(pumpPin, HIGH);
       for (int i = 0; i <= 254; i++) {     
           analogWrite(brightLEDPin,i);
           delay(5);
         }

       //digitalWrite(mister1Pin, HIGH);
       //digitalWrite(mister2Pin, HIGH);
       //digitalWrite(fanPin, HIGH);
       for (int x = upperHalfstart; x < NUM_LEDS; x++) {     
         leds[x] = CHSV(170, 255, 150);   //prepare the data for the LED given the above in global color
       }
       FastLED.show();  
       firstrun = 0;
     }
     //do nothing
   }

   
   
   if (mode == 4){           // Light Rain
     if (firstrun == 1){
       //analogWrite(brightLEDPin,20);
       for (int x = upperHalfstart; x < NUM_LEDS; x++) {     
         leds[x] = CHSV(200, 255, 255);   //prepare the data for the LED given the above in global color
       }
       FastLED.show(); 
       firstrun = 0;
     }
     EVERY_N_SECONDS(60) {
        digitalWrite(pumpPin, HIGH);
        delay(500);
        digitalWrite(pumpPin, LOW);
     }
   }

   

   if (mode == 5){           // Partly Cloudy
     if (firstrun == 1){
       analogWrite(brightLEDPin,100);
       digitalWrite(mister1Pin, HIGH);
       digitalWrite(mister2Pin, HIGH);
       digitalWrite(fanPin, HIGH);
       for (int x = 0; x < lowerHalfend; x++) {     
         leds[x] = CHSV(200, 255, 200);   //prepare the data for the LED given the above in global color
       }
       FastLED.show(); 
       firstrun = 0;
     }
     //do nothing
   }
   
   if (mode == 6){           // Fog
     if (firstrun == 1){
       digitalWrite(mister1Pin, HIGH);
       digitalWrite(mister2Pin, HIGH);
       digitalWrite(fanPin, HIGH);
       firstrun = 0;
     }
     //do nothing
   }

   if (mode == 7){           // Aurora
     if (firstrun == 1){
       digitalWrite(mister1Pin, HIGH);
       digitalWrite(mister2Pin, HIGH);
       digitalWrite(fanPin, HIGH);
       //analogWrite(brightLEDPin, 100);
       firstrun = 0;
     }
     aurora();
   }

   if (mode == 8){           // Rainbow
     if (firstrun == 1){
       digitalWrite(mister1Pin, HIGH);
       digitalWrite(mister2Pin, HIGH);
       digitalWrite(fanPin, HIGH);
       firstrun = 0;
     }
     fill_rainbow( leds, NUM_LEDS, gHue, 1);
     for (int x = upperHalfstart; x < NUM_LEDS; x++) {     
         leds[x] = CHSV(200, 255, 0);   //prepare the data for the LED given the above in global color
       }
     FastLED.show();  
     // insert a delay to keep the framerate modest
     delay(10); 
     EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
   }

   if (mode == 9){           // Volcano Ocean
     if (firstrun == 1){
       digitalWrite(mister1Pin, HIGH);
       digitalWrite(mister2Pin, HIGH);
       digitalWrite(fanPin, HIGH);
       firstrun = 0;
//        for (int x = 0; x < lowerHalfend; x++) {     
//          leds[x] = CHSV(95, 255, 255);   //prepare the data for the LED given the above in global color
//        }
//        FastLED.show();
     }
    volcano();
   }

   if (mode == 10){           // Rainforest
     if (firstrun == 1){
       digitalWrite(mister1Pin, HIGH);
       digitalWrite(mister2Pin, HIGH);
       //digitalWrite(fanPin, HIGH);
       firstrun = 0;
       for (int x = 0; x < lowerHalfend; x++) {     
         leds[x] = CHSV(240, 255, 150);   //prepare the data for the LED given the above in global color
       }
       FastLED.show();
     }
     EVERY_N_SECONDS(20) {
        digitalWrite(pumpPin, HIGH);
        digitalWrite(fanPin, HIGH);
        delay(500);
        digitalWrite(pumpPin, LOW);
        delay(500);
        digitalWrite(fanPin, LOW);
     }
   }

   if (mode == 11){           // Thunder storm
     if (firstrun == 1){
       digitalWrite(pumpPin, HIGH);
       digitalWrite(mister1Pin, HIGH);
       digitalWrite(mister2Pin, HIGH);
       digitalWrite(fanPin, HIGH);
       firstrun = 0;
     }
     if (thunderStrike++ >= thunderThreshold){
       analogWrite(brightLEDPin,255);;
       delay(random(300));
       analogWrite(brightLEDPin,0);
       delay(random(300));
       analogWrite(brightLEDPin,255);
       delay(random(300));
       analogWrite(brightLEDPin,0);
       Serial.println("ThunderStrike");

       if (random(10) >= 2){
       analogWrite(brightLEDPin,255);
       delay(random(50));
       analogWrite(brightLEDPin,0);
       delay(random(50));
       analogWrite(brightLEDPin,255);
       delay(random(50));
       analogWrite(brightLEDPin,0);
       }

       if (random(10) >= 2){
         analogWrite(brightLEDPin,254);
         delay(100);
         for (int i = 254; i >= 0; i--) {     
           analogWrite(brightLEDPin,i);
           delay(5);
         }
       }
       
       thunderStrike = 0;
       thunderThreshold = random(300,3000);
     }
     delay(10);
   }

   if (mode == 12){           // Stand-by
     if (firstrun == 1){
       //do nothing
       digitalWrite(extractionPin, HIGH);
       firstrun = 0;
     }
     //do nothing
   }




  }
  







      // RGB LEDs

void aurora()
{  
  auroraCounter++;
  
  if (auroraCounter < auroraThreshold){
     
  if (twinkle1++ < target1){
  leds[pos] += CHSV( 95, 0, twinkle1);
  }
  else {
    pos = random16(upperHalfstart+1,NUM_LEDS);
    twinkle1 = 2; 
    target1 = random16(255);
  }
  if (twinkle2++ < target2){
  leds[pos2] += CHSV( 0, 255, twinkle2);
  }
  else {
    pos2 = random16(upperHalfstart+1,NUM_LEDS);
    twinkle2 = 2;
    target2 = random16(255); 
  }
    if (twinkle3++ < target3){
  leds[pos3] += CHSV( 50, 255, twinkle3);
  }
  else {
    pos3 = random16(upperHalfstart+1,NUM_LEDS);
    twinkle3 = 3;
    target3 = random16(255); 
  }
  if (twinkle4++ < target4){
  leds[pos4] += CHSV( 200, 255, twinkle3);
  }
  else {
    pos4 = random16(upperHalfstart+1,NUM_LEDS);
    twinkle4 = 3;
    target4 = random16(255); 
   }
   
  if (twinkle5++ < target5){
  leds[pos5] += CHSV( 140, 255, twinkle5);
  }
  else {
    pos5 = random16(upperHalfstart+1,NUM_LEDS);
    twinkle5 = 1;
    target5 = random16(5,255); 
   } 
   
  blur1d(leds, NUM_LEDS, 172);
  blur1d(leds, NUM_LEDS, 172); 
  }
  
  else if (auroraCounter > auroraThreshold){
  fadeToBlackBy( leds, NUM_LEDS, 1);
  digitalWrite(fanPin, LOW);
   //Serial.println("Limit"); 
  }
  if (auroraCounter > (auroraThreshold + 150)){
    Serial.println("Fade Out Aurora");
    auroraCounter = 0;
    auroraThreshold = random(10,200);
    digitalWrite(fanPin, HIGH);
  }
  FastLED.show();
  delay(35);
 }




void volcano()
{
  // random colored speckles that blink in and fade smoothly
  
  if (twinkle1++ < target1){
  leds[pos] += CHSV( 95, 255, twinkle1);
  }
  else {
    pos = random16(NUM_LEDS);
    twinkle1 = 2; 
    target1 = random16(255);
  }
  if (twinkle2++ < target2){
  leds[pos2] += CHSV( 97, 255, twinkle2);
  }
  else {
    pos2 = random16(NUM_LEDS);
    twinkle2 = 2;
    target2 = random16(255); 
  }
    if (twinkle3++ < target3){
  leds[pos3] += CHSV( 90, 255, twinkle3);
  }
  else {
    pos3 = random16(upperHalfstart,NUM_LEDS);
    twinkle3 = 3;
    target3 = random16(255); 
  }
  if (twinkle4++ < target4){
  leds[pos4] += CHSV( 170, 255, twinkle3);
  leds[pos4+1] += CHSV( 170, 255, twinkle3);
  leds[pos4-1] += CHSV( 170, 255, twinkle3);  
  }
  else {
    pos4 = random16(1,lowerHalfend);
    twinkle4 = 3;
    target4 = random16(255); 
  }
  
  FastLED.show();
  delay(25);

  //if (fadeCounter++ > 5){
  blur1d(leds, NUM_LEDS, 172);  
  /* "blurs" the blocks of colour = fading it out to left and right.  
      0 = no spread at all
      64 = moderate spreading
      172 = maximum smooth, even spreading
      173..255 = wider spreading, but increasing flicker
      NOTE: it spreads the lit block wider, smearing it into nearby unl
   */ 
  //}
}


void twinkle() 
{
  // random colored speckles that blink in and fade smoothly
  
  if (twinkle1++ < target1){
  leds[pos] += CHSV( 100, 0, twinkle1);
  }
  else {
    pos = random16(NUM_LEDS);
    twinkle1 = 1; 
    target1 = random16(255);
  }
  if (twinkle2++ < target2){
  leds[pos2] += CHSV( 100, 0, twinkle2);
  }
  else {
    pos2 = random16(NUM_LEDS);
    twinkle2 = 1;
    target2 = random16(255); 
  }
    if (twinkle3++ < target3){
  leds[pos3] += CHSV( 100, 0, twinkle3);
  }
  else {
    pos3 = random16(upperHalfstart,NUM_LEDS);
    twinkle3 = 1;
    target3 = random16(255); 
  }
  FastLED.show();
  delay(15);

  if (fadeCounter++ > 15){
  fadeToBlackBy( leds, NUM_LEDS, 1); 
  }
}




      
  void RGB_LEDS(){                                                           
      // Call the current pattern function once, updating the 'leds' array
    fill_rainbow( leds, lowerHalfend, gHue, 1);
    
    for (int x = upperHalfstart; x < NUM_LEDS; x++) {     
      leds[x] = CHSV(100, 200, 255);   //prepare the data for the LED given the above in global color
    }
  
    // send the 'leds' array out to the actual LED strip
    FastLED.show();  
    // insert a delay to keep the framerate modest
    delay(10); 
  
    // do some periodic updates
    EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  
  //  RGB_color(0, 255, 255); // Red      Volcano
  //  delay(timma);
  //  RGB_color(0, 210, 255); // Orange  Twilight
  //  delay(timma);
  //  RGB_color(0, 0, 255); // Yellow    Sunlight
  //  delay(timma);
  //  RGB_color(255, 225, 255); // Dark Green    Rainforest
  //  delay(timma);
  //  RGB_color(240, 0, 240); // Light Green  Aurora
  //  delay(timma);
  //  RGB_color(255, 50, 200); // teal      Light Rain
  //  delay(timma);
  //  RGB_color(255, 0, 0); // Aqua      Partly Cloudy
  //  delay(timma);
  //  RGB_color(255, 255, 225); // Navy     Fog
  //  delay(timma);
  //  RGB_color(255, 240, 20); // Mid Blue  Heavy Rain
  //  delay(timma);
  //  RGB_color(0, 255, 0); // Purple   Thunder Storm
  //  delay(timma);
  //  RGB_color(0, 200, 128); // Fuchsia           Rainbow
  //  delay(timma);
  //  RGB_color(0, 0, 0); // White      White
  //  delay(timma);
  }
  
  
  
  void turnoffLEDs()
  {
      for (int led = 0; led < NUM_LEDS; led++)        
    { //turn off LEDs
      leds[led] = CHSV( 100, 0, 0);
    }
    digitalWrite(pumpPin, LOW);   
    digitalWrite(fanPin, LOW);   
    analogWrite(brightLEDPin,0);
    digitalWrite(mister1Pin, LOW);   
    digitalWrite(mister2Pin, LOW); 
    digitalWrite(extractionPin, LOW); 
    firstrun = 1; 
  }
  
  
  void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
   {
    analogWrite(ButtonLEDpinR, red_light_value);
    analogWrite(ButtonLEDpinG, green_light_value);
    analogWrite(ButtonLEDpinB, blue_light_value);
  }










     // Buttons

  void checkButtons(){
    Serial.print("checking buttons");
    buttonState1 = digitalRead(buttonPin1);
    buttonState2 = digitalRead(buttonPin2);
    buttonState3 = digitalRead(buttonPin3);
    buttonState4 = digitalRead(buttonPin4);
    buttonState5 = digitalRead(buttonPin5);
    buttonState6 = digitalRead(buttonPin6);
     
  
  
      // check if the pushbutton is pressed.
    // if it is, the buttonState is HIGH:
    if (buttonState1 == LOW) {     
      Serial.println("Button 1 Pressed");
      RGB_color(0, 0, 255); // Yellow    Sunlight
      delay(timma);
      turnoffLEDs();       //turn off LEDs
      FastLED.show(); 
      Serial1.write("1");
      mode = 1;
      EEPROM.update(1, mode);
      buttonState1 = digitalRead(buttonPin1);
  
      if (buttonState1 == LOW){             // check to see if the button is still being held
        Serial.println("Button 1 HELD");
      RGB_color(0, 210, 255); // Orange  Twilight
      Serial1.write("2");
      delay(timma);
      mode = 2;
      EEPROM.update(1, mode);
      }
    }
     
    else if (buttonState2 == LOW) {     
      Serial.println("Button 2 Pressed");
      RGB_color(255, 240, 20); // Mid Blue  Heavy Rain
      delay(timma); 
      turnoffLEDs();       //turn off LEDs
      FastLED.show();
      Serial1.write("3");
      mode = 3;
      EEPROM.update(1, mode);
      buttonState2 = digitalRead(buttonPin2);
      
      if (buttonState2 == LOW){             // check to see if the button is still being held
        Serial.println("Button 2 HELD");
      RGB_color(255, 50, 200); // teal      Light Rain
      delay(timma);
      Serial1.write("4");
      mode = 4;
      EEPROM.update(1, mode);
      }
    }

    else if (buttonState3 == LOW) {     
      Serial.println("Button 3 Pressed");
      RGB_color(255, 0, 0); // Aqua      Partly Cloudy
      delay(timma); 
      turnoffLEDs();       //turn off LEDs
      FastLED.show();
      Serial1.write("5");
      mode = 5;
      EEPROM.update(1, mode);
      buttonState3 = digitalRead(buttonPin3);
      
      if (buttonState3 == LOW){             // check to see if the button is still being held
        Serial.println("Button 3 HELD");
      RGB_color(255, 255, 225); // Navy     Fog
      delay(timma);
      Serial1.write("6");
      mode = 6;
      EEPROM.update(1, mode);
      }
    }

    else if (buttonState4 == LOW) {     
      Serial.println("Button 4 Pressed");
      RGB_color(240, 0, 240); // Light Green  Aurora
      delay(timma); 
      turnoffLEDs();       //turn off LEDs
      FastLED.show();
      Serial1.write("7");
      mode = 7;
      EEPROM.update(1, mode);
      buttonState4 = digitalRead(buttonPin4);
      
      if (buttonState4 == LOW){             // check to see if the button is still being held
        Serial.println("Button 4 HELD");
      RGB_color(0, 200, 128); // Fuchsia           Rainbow
      delay(timma);
      Serial1.write("8");
      mode = 8;
      EEPROM.update(1, mode);
      }
    }

    else if (buttonState5 == LOW) {     
      Serial.println("Button 5 Pressed");
      RGB_color(0, 255, 255); // Red      Volcano
      delay(timma);
      turnoffLEDs();       //turn off LEDs
      FastLED.show(); 
      Serial1.write("9");
      mode = 9;
      EEPROM.update(1, mode);
      buttonState5 = digitalRead(buttonPin5);
      
      if (buttonState5 == LOW){             // check to see if the button is still being held
        Serial.println("Button 5 HELD");
      RGB_color(255, 225, 255); // Dark Green    Rainforest
      delay(timma);
      Serial1.write("a");
      mode = 10;
      EEPROM.update(1, mode);
      }
    }

    else if (buttonState6 == LOW) {     
      Serial.println("Button 6 Pressed");
      RGB_color(0, 255, 0); // Purple   Thunder Storm
      delay(timma); 
      turnoffLEDs();       //turn off LEDs
      FastLED.show();
      Serial1.write("b");
      mode = 11;
      EEPROM.update(1, mode);
      buttonState6 = digitalRead(buttonPin6);
      
      if (buttonState6 == LOW){             // check to see if the button is still being held
        Serial.println("Button 6 HELD");
      RGB_color(0, 0, 0); // White      White
      delay(timma);
      Serial1.write("c");
      mode = 12;
      EEPROM.update(1, mode);
      }
    }
  }






  
//void strom(int bright)
//{
//   runningflash = random(1,8);
//   runningflashfucntion(bright);
//
//    for (int led = 0; led < NUM_LEDS; led++) {          leds[led] = CHSV( 100, 0, 0);      }    //turn off all LEDS
//      FastLED.show();
//    
//    if (random(1, 3) == 2) { 
//       runningflashfucntion(bright);
//
//       for (int led = 0; led < NUM_LEDS; led++) {          leds[led] = CHSV( 100, 0, 0);      }    //turn off all LEDS
//       FastLED.show();
//    }
//
//    else { 
//      runningflashfucntion(bright);
//
//      for (int led = 0; led < NUM_LEDS; led++) {          leds[led] = CHSV( 100, 0, 0);      }    //turn off all LEDS
//        FastLED.show();
//     
//      delay(500);
//      runningflash = random(1,8);
//      runningflashfucntion(bright);
//    }
//
//  for (int led = 0; led < NUM_LEDS; led++) {          leds[led] = CHSV( 100, 0, 0);      }    //turn off all LEDS
//      FastLED.show();
//
//  for (int led = 0; led < NUM_LEDS; led++) {          leds[led] = CHSV( 100, 0, 0);      }  //turn off all LEDS
//      FastLED.show();
//      delay(random(10, 100));                                                               //random delay between 10-100
//
//   if (random(100) <= 10) {                                  //1/10 times big flash! 
//      for (int i = 255; i > 0; i--) {
//          for (int led = 0; led < int(NUM_LEDS); led++) {         
//           leds[led] = CHSV( 100, 0, i);
//          }
//       FastLED.show();
//    }  
//  }
//  turnoffLEDs();
//  FastLED.show();
//} 
//
//
//
//
//void pops(int bright)
//{
//
//   EVERY_N_SECONDS(1) {
//   randomPop = random(20);   
//   if (randomPop == 1){
//    
//    for (int led = 0; led < NUM_LEDS; led++) {          leds[led] = CHSV( 100, 0, 0);      }    //turn off all LEDS
//      FastLED.show();
//
//  ledSingle1 = random(NUM_LEDS);                                                  //choose a random LED
//  leds[ledSingle1] = CHSV( 100, 0, bright);                                          //turn it on for a random time between 10-100 then turn it off. Do this twice.
//  FastLED.show();
//  delay(random (10, 100));
//  leds[ledSingle1] = CHSV( 100, 0, 0);
//  FastLED.show();
//  leds[ledSingle1] = CHSV( 100, 0, bright);
//  FastLED.show();
//  delay(random (40, 100));
//  leds[ledSingle1] = CHSV( 100, 0, 0);
//  FastLED.show();
//
//  if (random(1, 3) == 2) {                                                        //every 1 out of 3 times flash another LED
//    ledSingle1 = random (NUM_LEDS);
//    ledSingle2 = random (50);
//    leds[ledSingle1] = CHSV( 100, 0, bright);
//    if(ledSingle2 < NUM_LEDS){   leds[ledSingle2] = CHSV( 100, 0, bright);  }
//    FastLED.show();
//  }
//  else {
//    ledSingle1 = random (NUM_LEDS);                                               //every 2 out of 3 times flash a small group of LEDs on and off
//    ledSingle2 = ledSingle1 + 1;
//    ledSingle3 = ledSingle2 + 2;
//    leds[ledSingle1] = CHSV( 100, 0, bright);
//    FastLED.show();
//    delay(random (15));
//    if(ledSingle2 < NUM_LEDS){   leds[ledSingle2] = CHSV( 100, 0, bright); }
//    FastLED.show();
//    delay(random (12));
//    if(ledSingle3 < NUM_LEDS){   leds[ledSingle3] = CHSV( 100, 0, bright); }
//    FastLED.show();
//  }
//
//  for (int led = 0; led < NUM_LEDS; led++) {          leds[led] = CHSV( 100, 0, 0);      }    //turn off all LEDS
//      FastLED.show();
//   } 
//  }
//}
//
//
//
//
//
//
//
//
//void runningflashfucntion(int bright){
//  //runningflash = random(1,8);
//  
//  //if (runningflash++ >= 9){ runningflash = 1; }
//  Serial.println(runningflash);
//  
//  //runningflash = 8;
//
//  if (runningflash == 1){
//     for (int led = 0; led < 3; led++) {          //turn on a random chain of LEDs white
//        leds[led] = CHSV( 100, 0, bright);
//        FastLED.show();
//        delay(runningdelay);
//      }
//  }
//
//  else if (runningflash == 2){
//    for (int led = 12; led < 17; led++) {          //turn on a random chain of LEDs white
//        leds[led] = CHSV( 100, 0, bright-20);
//        FastLED.show();
//        delay(runningdelay);
//    }
//  }
//
//  else if (runningflash == 3){
//    for (int led = 18; led < 21; led++) {          //turn on a random chain of LEDs white
//        leds[led] = CHSV( 100, 0, bright);
//        FastLED.show();
//        delay(runningdelay);
//    }
//  }
//
//  else if (runningflash == 4){
//    for (int led = 5; led < 9; led++) {          //turn on a random chain of LEDs white
//        leds[led] = CHSV( 100, 0, bright);
//        FastLED.show();
//        delay(runningdelay);
//    }
//  }
//
//  else if (runningflash == 5){
//    for (int led = 4; led >= 1; led--) {          //turn on a random chain of LEDs white
//        leds[led] = CHSV( 100, 0, bright);
//        FastLED.show();
//        delay(runningdelay);
//    }
//  }
//
//  else if (runningflash == 6){
//    for (int led = 14; led >= 10; led--) {          //turn on a random chain of LEDs white
//        leds[led] = CHSV( 100, 0, bright-30);
//        FastLED.show();
//        delay(runningdelay);
//    }
//  }
//
//  else if (runningflash == 7){
//    for (int led = 22; led >= 19; led--) {          //turn on a random chain of LEDs white
//        leds[led] = CHSV( 100, 0, bright);
//        FastLED.show();
//        delay(runningdelay);
//    }
//  }
//  
//  else {
//  for (int led = 0; led < int(random(NUM_LEDS)); led++) {          //turn on a random chain of LEDs white
//        leds[led] = CHSV( 100, 0, bright-50);
//        FastLED.show();
//        delay(runningdelay);
//      }
//  }
//}




void eepromSet() 
{ //newEpprom = 1;          // first run eprom data save
  newEpprom = EEPROM.read(0);          // first run eprom data save
  if (newEpprom != 73) {
    
    Serial.println("New EPROM!");
    // eeprom values:
    newEpprom = 73;
    EEPROM.update(0, newEpprom);
    mode = 0;
    EEPROM.update(1, mode);
  }

  else { 
    Serial.println("Old EPROM!");                           //not new eeprom
    mode =        (int)EEPROM.read(1);

    Serial.print("mode :   ");
    Serial.println(mode);

    if (mode < 0 || mode > 12){    // safety in case bad eprom reading
      mode = 0;

    }

      // check if the pushbutton is pressed.
    // if it is, the buttonState is HIGH:
    if (mode == 1){
      RGB_color(0, 0, 255); // Yellow    Sunlight
      Serial1.write("1");
      }

     else if (mode == 2){
      RGB_color(0, 210, 255); // Orange  Twilight
      Serial1.write("2");
      }
     
     else if (mode == 3){
      RGB_color(255, 240, 20); // Mid Blue  Heavy Rain
      Serial1.write("3");
      }
      
     else if (mode == 4){
      RGB_color(255, 50, 200); // teal      Light Rain
      Serial1.write("4");
      }

     else if (mode == 5){
      RGB_color(255, 0, 0); // Aqua      Partly Cloudy
      Serial1.write("5");
      }

     else if (mode == 6){
      RGB_color(255, 255, 225); // Navy     Fog
      Serial1.write("6");
      }

     else if (mode == 7){
      RGB_color(240, 0, 240); // Light Green  Aurora
      Serial1.write("7");
      }

     else if (mode == 8){
      RGB_color(0, 200, 128); // Fuchsia           Rainbow
      Serial1.write("8");
      }

     else if (mode == 9){
      RGB_color(0, 255, 255); // Red      Volcano
      Serial1.write("9");
      }

     else if (mode == 10){
      RGB_color(255, 225, 255); // Dark Green    Rainforest
      Serial1.write("a");
      }

     else if (mode == 11){
      RGB_color(0, 255, 0); // Purple   Thunder Storm
      Serial1.write("b");
      }

     else if (mode == 12){
      RGB_color(0, 0, 0); // White      White
      Serial1.write("c");
      }
     }
    }
