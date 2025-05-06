
  #include <FastLED.h>
  #include <EEPROM.h>

  // Upload note - sometimes upload code fails - 
  // if that happends disconnect USB cord, turn Tempescope off, plug in USB cord upload code then should be sucessfull and can then on upload code suffessfully

  // Background extraction fans can be switched on/off by holding button 1 when unit powers up (red flash indicates now off, green now on)

  /*  TODO: 
      Go through modes 1 by one and make final tweeks
  */


  /*  Mode Key: 
 
      B1 Mode 1:  Sunlight         Yellow
      B1 Mode 2:  Twilight         Purple   
      B2 Mode 3:  Sunshower        Mid Blue
      B2 Mode 4:  Thunderstorm     Teal
      B3 Mode 5:  Midnight Rain    Dark Green
      B3 Mode 6:  Ocean Volcano    Red
      B4 Mode 7:  Partly Cloudy    Aqua
      B4 Mode 8:  Fog              Navy
      B5 Mode 9:  Rainbow          Fuchsia
      B5 Mode 10: Aurora           Lime
      B6 Mode 11: Sunrise          Orange
      B6 Mode 12: Stand-by         White
 

      RGB_color(0, 255, 255); // Red  
      RGB_color(0, 210, 255); // Orange      
      RGB_color(0, 0, 255); // Yellow        
      RGB_color(255, 225, 255); // Dark Green 
      RGB_color(240, 0, 240); // Lime        
      RGB_color(255, 50, 200); // teal       
      RGB_color(255, 0, 0); // Aqua          
      RGB_color(255, 255, 225); // Navy      
      RGB_color(255, 240, 20); // Mid Blue    
      RGB_color(0, 255, 0); // Purple        
      RGB_color(0, 200, 128); // Fuchsia      
      RGB_color(0, 0, 0); // White            
  */
  
  
  
  FASTLED_USING_NAMESPACE
  
  #if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
  #warning "Requires FastLED 3.1 or later; check github for latest code."
  #endif
  
  #define DATA_PIN    4
  //#define CLK_PIN   4
  #define LED_TYPE    WS2812B
  #define COLOR_ORDER GRB  // should be GRB
  #define NUM_LEDS    190  //(96 each section)
  CRGB leds[NUM_LEDS];
  
  #define BRIGHTNESS          255
  #define FRAMES_PER_SECOND  120   //might need to bump down to 60 as was redefined lower down by accident.

int minLEDvalue[NUM_LEDS];
int goingUp[NUM_LEDS];
int currentValue[NUM_LEDS];

int currentValueFade = 0;
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
  int heatIndex = 0;

    
  int blendDelay = 50;
  int palletCounter = 1;
  int palletDelay = 0;
  int extractionCounter = 0;
  int extrationOn = 1;

  
  const int pumpPin = 11;
  const int fanPin = 10;
  int fanPulseCounter = 0;
  const int fanSpeed = 150;
  const int brightLEDPin = 9;
  int brightLEDfade = 0;
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
  void fourseasons();
  void sunrise();


  
  
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
    FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  
    // set master brightness control
    FastLED.setBrightness(BRIGHTNESS);

    //Serial.println("here i am");
  
    turnoffLEDs();
    FastLED.show();

    RGB_color(0, 0, 0); // White      White

    //mode = 12;
    eepromSet();

    buttonState1 = digitalRead(buttonPin1);    // When unit powers up check button 1 to see if it is being pressed if so toggle on/off background extraction fans
      if (buttonState1 == LOW) {
        Serial.println("Button 1 Pressed during startup");
        if (extrationOn == 1){
          extrationOn = 0;
          EEPROM.update(2, extrationOn);
          Serial.println("Extration Turned Off");
          fill_solid(leds, NUM_LEDS, 0xF70505);
          FastLED.show();
          delay(timma);

          turnoffLEDs();       //turn off LEDs
          FastLED.show();
          delay(timma); 

          fill_solid(leds, NUM_LEDS, 0xF70505);
          FastLED.show(); 
          delay(timma);

          turnoffLEDs();       //turn off LEDs
          FastLED.show();
          delay(timma); 
        }

        else if (extrationOn == 0){
          extrationOn = 1;
          EEPROM.update(2, extrationOn);
          Serial.println("Extration Turned On");
          fill_solid(leds, NUM_LEDS, 0x33C213);
          FastLED.show();
          delay(timma);

          turnoffLEDs();       //turn off LEDs
          FastLED.show();
          delay(timma); 

          fill_solid(leds, NUM_LEDS, 0x33C213);
          FastLED.show(); 
          delay(timma);

          turnoffLEDs();       //turn off LEDs
          FastLED.show();
          delay(timma);                   
        }
      }

    Serial.println("end of setup");
  }
  
  uint8_t gHue = 0; // rotating "base color" used by many of the patterns
    








  
  void loop()
  {
    //Serial.print("loop");
    //delay(1000);

    if (extrationOn == 1){
    EVERY_N_SECONDS(60){        
      extractionCounter++;
      if (extractionCounter == 9) {     // Every 10 mins turn extractor fans on for 1 min (10-9) then back off
        digitalWrite(extractionPin, HIGH);
        Serial.println("extraction fans ON");  //TODO remote serial note
      }
      if (extractionCounter >= 11) {     
        digitalWrite(extractionPin, LOW);
        Serial.println("extraction fans OFF");  //TODO remote serial note
        extractionCounter = 0;
      }
    }
  }

   checkButtons();

   if (mode == 1){           // Sunlight
     if (firstrun == 1){
      digitalWrite(mister1Pin, HIGH);
      digitalWrite(mister2Pin, HIGH);
      //digitalWrite(fanPin, HIGH);                             
      analogWrite(fanPin, fanSpeed);
      for (int i = 0; i <= 255; i++) {     
           analogWrite(brightLEDPin,i);
           delay(5);
         }
       firstrun = 0;
     }
     //do nothing
   }

   if (mode == 2){           // Twilight
     if (firstrun == 1){
      digitalWrite(mister1Pin, HIGH);
      digitalWrite(mister2Pin, HIGH);
      //digitalWrite(fanPin, HIGH);                             
      analogWrite(fanPin, fanSpeed);
      firstrun = 0;
     }
     //twinkling white RGB LEDs
     twinkle();
     }


    if (mode == 3){           // Sunshower
     if (firstrun == 1){
       digitalWrite(pumpPin, HIGH);
       for (int i = 0; i <= 255; i++) {     
           analogWrite(brightLEDPin,i);
           delay(5);
         }
       digitalWrite(mister1Pin, HIGH);
       digitalWrite(mister2Pin, HIGH);
       //digitalWrite(fanPin, HIGH);                             
      analogWrite(fanPin, fanSpeed);
       for (int x = upperHalfstart; x < NUM_LEDS; x++) {     
         leds[x] = CHSV(170, 255, 150);   //prepare the data for the LED given the above in global color
       }
       FastLED.show();  
       firstrun = 0;
     }
     //do nothing
   }

  
  if (mode == 4){           // Thunderstorm
    if (firstrun == 1){
      digitalWrite(pumpPin, HIGH);
      digitalWrite(mister1Pin, HIGH);
      digitalWrite(mister2Pin, HIGH);
      //digitalWrite(fanPin, HIGH);                             
      analogWrite(fanPin, 220);
      firstrun = 0;
      thunderStrike = 600;
      thunderThreshold = 1000;
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

      if (random(10) >= 8){          //intermitent flashes of the upper RGB LEDs.
        for (int x = upperHalfstart; x < NUM_LEDS; x++) {     
          leds[x] = CHSV(200, 255, 255);   //prepare the data for the LED given the above in global color
          }
        FastLED.show();
        delay(15);
        for (int x = upperHalfstart; x < NUM_LEDS; x++) {     
          leds[x] = CHSV(0, 0, 0);   //prepare the data for the LED given the above in global color
          }
        FastLED.show();
      }
      
      thunderStrike = 0;
      thunderThreshold = random(300,3000);
    }
    delay(10);
  }


  if (mode == 5){           // Midnight Rain
    if (firstrun == 1){
      digitalWrite(pumpPin, HIGH);
      digitalWrite(mister1Pin, HIGH);
      digitalWrite(mister2Pin, HIGH);
      //digitalWrite(fanPin, HIGH);
      firstrun = 0;
      // for (int x = 0; x < lowerHalfend; x++) {     
      //   leds[x] = CHSV(240, 255, 150);   //prepare the data for the LED given the above in global color
      // }
      // FastLED.show();
    }
    EVERY_N_MILLISECONDS(10) {
      ++fanPulseCounter;
      if (fanPulseCounter == 600){
        analogWrite(fanPin, 255);
        ++fanPulseCounter;
      }
      if (fanPulseCounter == 1000){
        analogWrite(fanPin, 0);
        fanPulseCounter = 0;
      }
    }
  }
   

  if (mode == 6){           // Volcano Ocean
    if (firstrun == 1){
      //digitalWrite(pumpPin, HIGH); 
      digitalWrite(mister1Pin, HIGH);
      digitalWrite(mister2Pin, HIGH);
      //digitalWrite(fanPin, HIGH);
      analogWrite(fanPin, 255);
      firstrun = 0;
//        for (int x = 0; x < lowerHalfend; x++) {     
//          leds[x] = CHSV(95, 255, 255);   //prepare the data for the LED given the above in global color
//        }
//        FastLED.show();
    }
    volcano();
     EVERY_N_SECONDS(60) {
       digitalWrite(pumpPin, HIGH);
       delay(500);
       digitalWrite(pumpPin, LOW);
    }
  }
   
   

   if (mode == 7){           // Partly Cloudy
    if (firstrun == 1){
      analogWrite(brightLEDPin,255);
      digitalWrite(mister1Pin, HIGH);
      digitalWrite(mister2Pin, HIGH);
      //digitalWrite(fanPin, HIGH);
      analogWrite(fanPin, 255);
      for (int x = 0; x < lowerHalfend; x++) {     
        leds[x] = CHSV(0, 0, 255);   //white
      }
      for (int x = upperHalfstart; x < NUM_LEDS; x++) {     
       leds[x] = CHSV(60, 150, 255);   //yellow bright
     }
      FastLED.show(); 
      firstrun = 0;
    }
    EVERY_N_MILLISECONDS(10) {
      ++fanPulseCounter;
      if (fanPulseCounter == 600){
        analogWrite(fanPin, 255);
        ++fanPulseCounter;
      }
      if (fanPulseCounter == 1000){
        analogWrite(fanPin, 0);
        fanPulseCounter = 0;
      }
    }
  }

  if (mode == 8){           // Fog
    if (firstrun == 1){
      digitalWrite(mister1Pin, HIGH);
      digitalWrite(mister2Pin, HIGH);
      //digitalWrite(fanPin, HIGH);
      analogWrite(fanPin, 255);
      for (int x = 0; x < lowerHalfend; x++) {     
       leds[x] = CHSV(0, 0, 255);   //white
      }
      for (int x = upperHalfstart; x < NUM_LEDS; x++) {     
       leds[x] = CHSV(0, 0, 255);   //white
      }
     FastLED.show(); 
      firstrun = 0;
    }
    EVERY_N_MILLISECONDS(10) {
      ++fanPulseCounter;
      if (fanPulseCounter == 600){
        analogWrite(fanPin, 255);
        ++fanPulseCounter;
      }
      if (fanPulseCounter == 1000){
        analogWrite(fanPin, 0);
        fanPulseCounter = 0;
      }
    }
  }

   if (mode == 9){           // Rainbow
     if (firstrun == 1){
       digitalWrite(mister1Pin, HIGH);
       digitalWrite(mister2Pin, HIGH);
       //digitalWrite(fanPin, HIGH);                             
      analogWrite(fanPin, fanSpeed);
       firstrun = 0;
     }
     fill_rainbow( leds, NUM_LEDS, gHue, 2);
     fill_rainbow( leds, upperHalfstart, gHue, 1);
    //  for (int x = upperHalfstart; x < NUM_LEDS; x++) {     
    //      leds[x] = CHSV(200, 255, 0);   //prepare the data for the LED given the above in global color
    //    }
     FastLED.show();  
     // insert a delay to keep the framerate modest
     delay(10); 
     EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
   }

   

   if (mode == 10){           // Aurora 
    if (firstrun == 1){
      digitalWrite(mister1Pin, HIGH);
      digitalWrite(mister2Pin, HIGH);
      //digitalWrite(fanPin, HIGH);        //has a pulsing fan built into aurora LED function.
      analogWrite(fanPin, 255);
      //analogWrite(brightLEDPin, 100);
      firstrun = 0;
    }
    aurora();
  }

   if (mode == 11){           // Sunrise
     if (firstrun == 1){
      palletCounter = 0;
      firstrun = 0;
      palletDelay = 0;
      brightLEDfade = 0;
      //digitalWrite(extractionPin, HIGH);
      //extractionCounter = 1;
     }
     sunrise();
    

      // EVERY_N_MILLISECONDS(100){                   // Troubleshooting each top component
      //   palletDelay++;
      // }
      //   if (palletDelay == 100) {     
      //     analogWrite(brightLEDPin,255);
      //     Serial.println("Bright LED on");
      //     palletDelay++;
      //   }
      //   if (palletDelay == 200) {     
      //     analogWrite(brightLEDPin,0);
      //     Serial.println("Bright LED off");
      //     palletDelay++;
      //   }
      //   if (palletDelay == 300) {     
      //     digitalWrite(mister1Pin, HIGH);
      //     digitalWrite(mister2Pin, HIGH);
      //     Serial.println("Misters on");
      //     palletDelay++;
      //   }
      //   if (palletDelay == 400) {     
      //     digitalWrite(mister1Pin, LOW);
      //     digitalWrite(mister2Pin, LOW);
      //     Serial.println("Misters off");
      //     palletDelay++;
      //   }
      //   if (palletDelay == 500) {     
      //     digitalWrite(fanPin, HIGH);
      //     Serial.println("lower fan on");
      //     palletDelay++;
      //   }
      //   if (palletDelay == 600) {     
      //     analogWrite(fanPin, 0);
      //     Serial.println("lower fan off");
      //     palletDelay++;
      //   }
      //   if (palletDelay == 700) {     
      //     palletDelay = 0;
      //   }
   }


   if (mode == 12){           // Stand-by
     if (firstrun == 1){

      palletCounter = 0;
      firstrun = 0;
      palletDelay = 0;

      // for (int i = 0; i <= 255; i++) {     
      //   analogWrite(brightLEDPin,i);
      //   delay(5);
      // }
      

      //  for (int x = 0; x < NUM_LEDS; x++) {     
      //    leds[x] = CHSV( 18, 255, 255); 
      //  }
      //  FastLED.show();
      
       firstrun = 0;
     }
     //fourseasons();
   }
  }
  











void fourseasons()             // slowly change colors of all RGB LEDS from orange to blue to pink to green
{
CRGBPalette256 palette = CRGBPalette256(    
  CRGB::OrangeRed, CRGB::RoyalBlue
);
CRGBPalette256 palette2 = CRGBPalette256(    
  CRGB::RoyalBlue, CRGB::DeepPink
);
CRGBPalette256 palette3 = CRGBPalette256(    
  CRGB::DeepPink, CRGB::DarkGreen
);
CRGBPalette256 palette4 = CRGBPalette256(    
  CRGB::DarkGreen, CRGB::OrangeRed
);
CRGBPalette256 palette0 = CRGBPalette256(    
  CRGB::Black, CRGB::OrangeRed
);

EVERY_N_MILLISECONDS(20){        // adjust this to slow everything down
  palletDelay++;
  if (palletDelay > 3000) {     // tweek this to adjust amount of time staying in target color vs changing colors 
    palletDelay = 0;
    blendDelay = 0;
    palletCounter++;
    Serial.print("Season: ");
    Serial.println(palletCounter);
    //extractionCounter++;            // counter to turn on and off extraction fans
    // Serial.print("Counter: ");
    // Serial.println(extractionCounter);
    // if (extractionCounter == 1){
    //   digitalWrite(extractionPin, HIGH);
    //   Serial.println("extraction fans on");
    // }
    // if (extractionCounter == 2){             // adjust this to change how long fans are ON for
    //   digitalWrite(extractionPin, LOW);
    //   Serial.println("extraction fans off");
    // }
    // if (extractionCounter > 3){           // adjust this to change how long fans are OFF for
    //   extractionCounter = 0;
    // }
    if (palletCounter > 4){
      palletCounter = 1;
    }
    // if (palletCounter == 1){
    //   digitalWrite(extractionPin, HIGH);
    //   Serial.println("extraction fans ON");
    // }
    // if (palletCounter == 2){
    //   digitalWrite(extractionPin, LOW);
    //   Serial.println("extraction fans OFF");
    // }

  }

  if (palletDelay < 255){
    if (palletCounter == 0){
      CRGB currentColor = ColorFromPalette(palette0, palletDelay,255,LINEARBLEND); // fade into orange from black only the first time around
      for (int j = 0; j < NUM_LEDS; j++) {
        leds[j] = currentColor;
      }
      FastLED.show();
    }
    if (palletCounter == 1){
      CRGB currentColor = ColorFromPalette(palette, palletDelay,255,LINEARBLEND);
      for (int j = 0; j < NUM_LEDS; j++) {
        leds[j] = currentColor;
      }
      FastLED.show();
    }
    if (palletCounter == 2){
        CRGB currentColor = ColorFromPalette(palette2, palletDelay,255,LINEARBLEND); 
      for (int j = 0; j < NUM_LEDS; j++) {
        leds[j] = currentColor;
      }
      FastLED.show();
      }
    if (palletCounter == 3){
        CRGB currentColor = ColorFromPalette(palette3, palletDelay,255,LINEARBLEND); 
      for (int j = 0; j < NUM_LEDS; j++) {
        leds[j] = currentColor;
      }
      FastLED.show();
      }
    if (palletCounter == 4){
        CRGB currentColor = ColorFromPalette(palette4, palletDelay,255,LINEARBLEND); 
      for (int j = 0; j < NUM_LEDS; j++) {
        leds[j] = currentColor;
      }
      FastLED.show();
      }
    }
  }
}




void sunrise() {              //red pallet up to white, hold for a bit, then blend from red to black and repeat.
  
  CRGBPalette16 paletteM = CRGBPalette16(     //0xFFCC00 yellow
    0x000000,
    0xFF3300,
    0xFF6600,
    0xFFFF50
  );
//  #000000,
//  #FF3300,
//  #FF6600,
//  #FFFF60
  
  CRGB color = ColorFromPalette(paletteM, heatIndex);
  fill_solid(leds, NUM_LEDS, color);

  EVERY_N_MILLISECONDS(50) {            //rise for 244 counts, day for 500 counts, set for 244 counts, night for 500 counds
    palletDelay++;
    if (palletDelay <= 244) {     
      heatIndex++;
    }
    if (150 < palletDelay && palletDelay <= 405) {     
      brightLEDfade++;
      analogWrite(brightLEDPin,brightLEDfade);
    }
    if (700 < palletDelay && palletDelay <= 955) {     
      brightLEDfade--;
      analogWrite(brightLEDPin,brightLEDfade);
    }

    if (800 < palletDelay && palletDelay < 1045) {     
       heatIndex--;
    }
    if (palletDelay == 1045) {     
      heatIndex = 0;
      brightLEDfade = 0;
    }
    if (palletDelay == 1444) {     
      palletDelay = 0;
      heatIndex = 0;
    }
  }
  FastLED.show();
  //Serial.print("heatIndex: ");
  //Serial.println(heatIndex);
}



//   void fourseasons2()
//   {
//   // slowly change colors of TOP RGB LEDS from    orange to blue to pink to green
//   // slowly change colors of bottom RGB LEDS from Green to Brown to Green to Blue

//   //CRGBPalette16 palette = RainbowColors_p; // Define a color palette
//   CRGBPalette16 palette = CRGBPalette16(
//     CRGB::Orange, CRGB::Blue, CRGB::Pink, CRGB::Green
//   );
//   CRGBPalette16 palette2 = CRGBPalette16(
//     CRGB::Green, CRGB::Brown, CRGB::Green, CRGB::Blue
//   );
//     for (int i = 0; i < 256; i++) {
//       CRGB currentColor = ColorFromPalette(palette, i); // Get the current color from the palette
//       for (int j = 0; j < lowerHalfend; j++) {
//         leds[j] = currentColor;
//         delay(100);
//       }
//       for (int i = 0; i < 256; i++) {
//         CRGB currentColor2 = ColorFromPalette(palette2, i); // Get the current color from the palette
//         for (int j = upperHalfstart; j < NUM_LEDS; j++) {
//           leds[j] = currentColor2;
//           delay(100);
//         }
//       FastLED.show();
//       delay(blendDelay);
//     }
//   }
// }




void aurora()
{  
  auroraCounter++;
  
  if (auroraCounter < auroraThreshold){
     
  if (twinkle1++ < target1){
  leds[pos] += CHSV( 0, 0, twinkle1);
  }
  else {
    pos = random16(0,NUM_LEDS);  //did start at upperHalfstart+1
    twinkle1 = 2; 
    target1 = random16(255);
  }
  if (twinkle2++ < target2){
  leds[pos2] += CHSV( 100, 255, twinkle2);
  }
  else {
    pos2 = random16(0,NUM_LEDS);
    twinkle2 = 2;
    target2 = random16(255); 
  }
    if (twinkle3++ < target3){
  leds[pos3] += CHSV( 85, 255, twinkle3);
  }
  else {
    pos3 = random16(0,NUM_LEDS);
    twinkle3 = 3;
    target3 = random16(255); 
  }
  if (twinkle4++ < target4){
  leds[pos4] += CHSV( 140, 255, twinkle3);
  }
  else {
    pos4 = random16(0,NUM_LEDS);
    twinkle4 = 3;
    target4 = random16(255); 
   }
   
  if (twinkle5++ < target5){
  leds[pos5] += CHSV( 200, 255, twinkle5);
  }
  else {
    pos5 = random16(0,NUM_LEDS);
    twinkle5 = 1;
    target5 = random16(5,255); 
   } 
   
  blur1d(leds, NUM_LEDS, 172);
  blur1d(leds, NUM_LEDS, 172); 
  }
  
  else if (auroraCounter > auroraThreshold){
  fadeToBlackBy( leds, NUM_LEDS, 1);
  //digitalWrite(fanPin, LOW);
  analogWrite(fanPin, 0);
  Serial.println("Fan OFF");
   //Serial.println("Limit"); 
  }
  if (auroraCounter > (auroraThreshold + 150)){
    Serial.println("Fan ON");
    auroraCounter = 0;
    auroraThreshold = random(10,200);
    //digitalWrite(fanPin, HIGH);
    analogWrite(fanPin, 255);
  }
  FastLED.show();
  delay(35);
 }



void volcano()
{
  // random colored speckles that blink in and fade smoothly
  
  if (twinkle1++ < target1){
  leds[pos] += CHSV( 7, 255, twinkle1);
  }
  else {
    pos = random16(NUM_LEDS);
    twinkle1 = 2; 
    target1 = random16(255);
  }
  if (twinkle2++ < target2){
  leds[pos2] += CHSV( 3, 255, twinkle2);
  }
  else {
    pos2 = random16(NUM_LEDS);
    twinkle2 = 2;
    target2 = random16(255); 
  }
    if (twinkle3++ < target3){
  leds[pos3] += CHSV( 18, 255, twinkle3);
  }
  else {
    pos3 = random16(upperHalfstart,NUM_LEDS);
    twinkle3 = 3;
    target3 = random16(255); 
  }
  if (twinkle4++ < target4){
  leds[pos4] += CHSV( 155, 255, twinkle3);
  leds[pos4+1] += CHSV( 155, 255, twinkle3);
  leds[pos4-1] += CHSV( 155, 255, twinkle3);  
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
  leds[pos] += CHSV( 0, 0, twinkle1);
  }
  else {
    pos = random16(NUM_LEDS);
    twinkle1 = 1; 
    target1 = random16(255);
  }
  if (twinkle2++ < target2){
  leds[pos2] += CHSV( 0, 0, twinkle2);
  }
  else {
    pos2 = random16(NUM_LEDS);
    twinkle2 = 1;
    target2 = random16(255); 
  }
    if (twinkle3++ < target3){
  leds[pos3] += CHSV( 0, 0, twinkle3);
  }
  else {
    pos3 = random16(0,NUM_LEDS);  // was starting at upperHalfstart
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
  }
  
  
  
  void turnoffLEDs()
  {
      for (int led = 0; led < NUM_LEDS; led++)        
    { //turn off LEDs
      leds[led] = CHSV( 100, 0, 0);
    }
    digitalWrite(pumpPin, LOW);   
    analogWrite(fanPin, 0);  
    analogWrite(brightLEDPin,0);
    digitalWrite(mister1Pin, LOW);   
    digitalWrite(mister2Pin, LOW); 
    //digitalWrite(extractionPin, LOW); 
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
    //Serial.print("checking buttons");
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
      RGB_color(0, 0, 255); // Mode 1:  Sunlight         Yellow
      delay(timma);
      turnoffLEDs();       //turn off LEDs
      FastLED.show(); 
      Serial1.write("1");
      mode = 1;
      EEPROM.update(1, mode);
      buttonState1 = digitalRead(buttonPin1);
  
      if (buttonState1 == LOW){             // check to see if the button is still being held
        Serial.println("Button 1 HELD");
        RGB_color(0, 255, 0); // Mode 2:  Twilight         Purple   
      Serial1.write("2");
      delay(timma);
      mode = 2;
      EEPROM.update(1, mode);
      }
    }
     
    else if (buttonState2 == LOW) {     
      Serial.println("Button 2 Pressed");
      RGB_color(255, 240, 20); // Mode 3:  Sunshower        Mid Blue
      delay(timma); 
      turnoffLEDs();       //turn off LEDs
      FastLED.show();
      Serial1.write("3");
      mode = 3;
      EEPROM.update(1, mode);
      buttonState2 = digitalRead(buttonPin2);
      
      if (buttonState2 == LOW){             // check to see if the button is still being held
        Serial.println("Button 2 HELD");
        RGB_color(255, 50, 200); // Mode 4:  Thunderstorm     Teal
        delay(timma);
      Serial1.write("4");
      mode = 4;
      EEPROM.update(1, mode);
      }
    }

    else if (buttonState3 == LOW) {     
      Serial.println("Button 3 Pressed");
      RGB_color(255, 225, 255); // Mode 5:  Midnight Rain    Green
      delay(timma); 
      turnoffLEDs();       //turn off LEDs
      FastLED.show();
      Serial1.write("5");
      mode = 5;
      EEPROM.update(1, mode);
      buttonState3 = digitalRead(buttonPin3);
      
      if (buttonState3 == LOW){             // check to see if the button is still being held
        Serial.println("Button 3 HELD");
        RGB_color(0, 255, 225); //  Mode 6:  Ocean Volcano    Red
        delay(timma);
      Serial1.write("6");
      mode = 6;
      EEPROM.update(1, mode);
      }
    }

    else if (buttonState4 == LOW) {     
      Serial.println("Button 4 Pressed");
      RGB_color(255, 0, 0); // Mode 7:  Partly Cloudy    Aqua
      delay(timma); 
      turnoffLEDs();       //turn off LEDs
      FastLED.show();
      Serial1.write("7");
      mode = 7;
      EEPROM.update(1, mode);
      buttonState4 = digitalRead(buttonPin4);
      
      if (buttonState4 == LOW){             // check to see if the button is still being held
        Serial.println("Button 4 HELD");
        RGB_color(255, 255, 225); // Mode 8:  Fog              Navy
        delay(timma);
      Serial1.write("8");
      mode = 8;
      EEPROM.update(1, mode);
      }
    }

    else if (buttonState5 == LOW) {     
      Serial.println("Button 5 Pressed");
      RGB_color(0, 200, 128); // Mode 9:  Rainbow          Fuchsia
      delay(timma);
      turnoffLEDs();       //turn off LEDs
      FastLED.show(); 
      Serial1.write("9");
      mode = 9;
      EEPROM.update(1, mode);
      buttonState5 = digitalRead(buttonPin5);
      
      if (buttonState5 == LOW){             // check to see if the button is still being held
        Serial.println("Button 5 HELD");
        RGB_color(240, 0, 240); // Mode 10: Aurora           Lime
        delay(timma);
      Serial1.write("a");
      mode = 10;
      EEPROM.update(1, mode);
      }
    }

    else if (buttonState6 == LOW) {     
      Serial.println("Button 6 Pressed");
      RGB_color(0, 210, 255); // Mode 11: Four Seasons     Orange
      delay(timma); 
      turnoffLEDs();       //turn off LEDs
      FastLED.show();
      Serial1.write("b");
      mode = 11;
      EEPROM.update(1, mode);
      buttonState6 = digitalRead(buttonPin6);
      
      if (buttonState6 == LOW){             // check to see if the button is still being held
        Serial.println("Button 6 HELD");
        RGB_color(0, 0, 0); // Mode 12: Stand-by         White
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
    extrationOn = 1;
    EEPROM.update(1, mode);
    EEPROM.update(2, extrationOn);
  }

  else { 
    Serial.println("Old EPROM!");                           //not new eeprom
    mode =        (int)EEPROM.read(1);
    Serial.print("mode :   ");
    Serial.println(mode);
    if (mode < 0 || mode > 12){    // safety in case bad eprom reading
      mode = 0;
    }

    extrationOn = (int)EEPROM.read(2);
    Serial.print("extrationOn :   ");
    Serial.println(extrationOn);
    if (extrationOn < 0 || extrationOn > 1){    // safety in case bad eprom reading
      extrationOn = 1;
    }

      // check if the pushbutton is pressed.
    // if it is, the buttonState is HIGH:
    if (mode == 1){
      RGB_color(0, 0, 255); // Mode 1:  Sunlight         Yellow
      Serial1.write("1");
      }

     else if (mode == 2){
      RGB_color(0, 255, 0); // Mode 2:  Twilight         Purple   
      Serial1.write("2");
      }
     
     else if (mode == 3){
      RGB_color(255, 240, 20); // Mode 3:  Sunshower        Mid Blue
      Serial1.write("3");
      }
      
     else if (mode == 4){
      RGB_color(255, 50, 200); // Mode 4:  Thunderstorm     Teal
      Serial1.write("4");
      }

     else if (mode == 5){
      RGB_color(255, 225, 255); // Mode 5:  Midnight Rain    Green
      Serial1.write("5");
      }

     else if (mode == 6){
      RGB_color(0, 255, 225); //  Mode 6:  Ocean Volcano    Red
      Serial1.write("6");
      }

     else if (mode == 7){
      RGB_color(255, 0, 0); // Mode 7:  Partly Cloudy    Aqua
      Serial1.write("7");
      }

     else if (mode == 8){
      RGB_color(255, 255, 225); // Mode 8:  Fog              Navy
      Serial1.write("8");
      }

     else if (mode == 9){
      RGB_color(0, 200, 128); // Mode 9:  Rainbow          Fuchsia
      Serial1.write("9");
      }

     else if (mode == 10){
      RGB_color(240, 0, 240); // Mode 10: Aurora           Lime
      Serial1.write("a");
      }

     else if (mode == 11){
      RGB_color(0, 210, 255); // Mode 11: Four Seasons     Orange
      Serial1.write("b");
      }

     else if (mode == 12){
      RGB_color(0, 0, 0); // Mode 12: Stand-by         White
      Serial1.write("c");
      }
     }
    }
