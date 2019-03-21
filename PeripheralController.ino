///////////////////////////////////
//      PeripheralController     //
//         v 2.1.0-alpha         //
//                               //
// by David Mahany, FRC Team 871 //
//         git.io/fhxV9          //
///////////////////////////////////

// PeripheralController.ino
// Contains setup for the different peripheral components.

#define VERSION "2.1.0-alpha"


#include "LEDStrip.h"

#if USE_OCTOWS2811
#include <OctoWS2811.h>
#else
#include <Adafruit_NeoPixel.h>
#endif
#ifdef __AVR__
#include <avr/power.h>
#endif

#include "LEDStripMode.h"
#include "LEDController.h"

LEDController ledController;

char readString[200];
int readIndex = 0;

long lastFPS = millis();
int fps = 0;

bool readFirstByte = false;
bool destForLED = false;

#if USE_OCTOWS2811
const int ledsPerStrip = 60;

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];

const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 astr = OctoWS2811(ledsPerStrip, displayMemory, drawingMemory, config);


LEDStrip lstr(astr, 15, ledsPerStrip * 0);
LEDStrip lstr2(astr, 12, ledsPerStrip * 3);
#else
Adafruit_NeoPixel astr = Adafruit_NeoPixel(15, 6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel astr = Adafruit_NeoPixel(12, 7, NEO_GRB + NEO_KHZ800);

LEDStrip lstr(astr);
LEDStrip lstr2(astr);
#endif

void setup() {
  Serial.begin(9600);
  Serial.println("PeripheralController");
  Serial.println("Version " VERSION);
  Serial.println("Source at git.io/fhxV9");
  Serial.println("");
  
  Serial.println("initialize");
  Serial.print("nStrips = ");
  
  #if USE_OCTOWS2811
  //TODO: why does it need this? (I think if you instantiate more than one it uses the params from the newest?)
  astr = OctoWS2811(ledsPerStrip, displayMemory, drawingMemory, config);
  #endif
  
  astr.begin();
  
  ledController.init();
  ledController.addStrip(&lstr);
  ledController.addStrip(&lstr2);
  Serial.println(ledController._numStrips);
  
}

//L0;SOLID;0xff00ff
//L0;CHASE;50;1000;500;0xff0000;0x0000ff
//L0;WAVE;0xff0000;0x0000ff;100;200
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
  #if USE_OCTOWS2811
  return 0;
  #else
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
  #endif
}
