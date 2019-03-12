///////////////////////////////////
//         LEDController         //
//         v 1.0.0-alpha         //
//                               //
// by David Mahany, FRC Team 871 //
//         git.io/fhxV9          //
///////////////////////////////////

// LEDController2019.ino
// Contains most of the logic. 

#define VERSION "1.0.0-alpha"

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#include "LEDStrip.h"
#include "LEDStripMode.h"
#include "LEDController.h"

LEDController ledController;

bool wasPlaced = false;

char readString[200];
int readIndex = 0;

long lastFPS = millis();
int fps = 0;

bool readFirstByte = false;
bool destForLED = false;

Adafruit_NeoPixel astr = Adafruit_NeoPixel(8, 6, NEO_GRB + NEO_KHZ800);
LEDStrip lstr(astr);

void setup() {
  Serial.begin(9600);
  Serial.println("LEDController");
  Serial.println("Version " VERSION);
  Serial.println("Source at git.io/fhxV9");
  Serial.println("");
  
  Serial.println("initialize");
  Serial.print("nStrips = ");

  astr.begin();
  
  ledController.init();
  ledController.addStrip(&lstr);
  Serial.println(ledController._numStrips);
  
  pinMode(5, INPUT_PULLUP);
}

//0;SOLID;0xff00ff
//0;CHASE;50;1000;500;0xff0000;0x0000ff
//0;WAVE;0xff0000;0x0000ff;100;200
void loop() {
  if (Serial.available() > 0) {
    delay(2);  //delay to allow buffer to fill 
    
    //Serial.println(Serial.available());
    
    while (Serial.available()) {
      char c = Serial.read();  //gets one byte from serial buffer
      if(c == '\n') { // finished reading message

        if(destForLED){
          ledController.handleInput(readString);
        }
        
        readIndex = 0;
        destForLED = false;
        readFirstByte = false;
        break;
      }else if(!readFirstByte){
        Serial.println(c);
        if(c == 'L') { // if the message starts with 'L', it will be destined for the LEDController
          destForLED = true;
        }
        readFirstByte = true;
      }else{
        readString[readIndex] = c; //makes the string readString
        readIndex++;
        readString[readIndex] = '\0';
      }
      
    }
  }
  ledController.update();
  
  fps++;
  if(millis() - lastFPS >= 1000){
    lastFPS = millis();
    Serial.print("UPS = ");
    Serial.println(fps);
    Serial.print("freeRam() = ");
    Serial.println(freeRam());
    fps = 0;
  }
  
  delay(1); // give it a little break
}

int freeRam() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

