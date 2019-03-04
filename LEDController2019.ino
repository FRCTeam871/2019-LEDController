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

Adafruit_NeoPixel astrip1 = Adafruit_NeoPixel(15, 6, NEO_GRB + NEO_KHZ800);
LEDStrip strip1(astrip1);

Adafruit_NeoPixel astrip2 = Adafruit_NeoPixel(8, 7, NEO_GRB + NEO_KHZ800);
LEDStrip strip2(astrip2);

LEDStrip strips[] = {strip1, strip2};
int nStrips = sizeof(strips) / sizeof(strips[0]);

LEDStripModeOff mOff;
LEDStripModeSolid mSolid;
LEDStripModeChase mChase;
LEDStripModeBounce mBounce;
LEDStripModeWave mWave;
LEDStripModeFade mFade;
LEDStripModeFPSTest mFPS;

LEDStripMode *modes[] = {&mOff, &mSolid, &mChase, &mBounce, &mWave, &mFade, &mFPS};
char *keys[] = {"OFF", "SOLID", "CHASE", "BOUNCE", "WAVE", "FADE", "FPS"};

bool wasPlaced = false;

char readString[200];
int readIndex = 0;

long lastFPS = millis();
int fps = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("LEDController");
  Serial.println("Version " VERSION);
  Serial.println("Source at git.io/fhxV9");
  Serial.println("");
  
  Serial.println("initialize");
  Serial.print("nStrips = ");
  Serial.println(nStrips);
  astrip1.begin();
  astrip2.begin();
  for(int i = 0; i < nStrips; i++){
    strips[i].setMode(&mChase);
    strips[i].setParams(5, (uint32_t)100, (uint32_t)1000, (uint32_t)500, 0xff0000, 0x0000ff);
  }
  pinMode(5, INPUT_PULLUP);
}

//0;SOLID;0xff00ff
//0;CHASE;50;1000;500;0xff0000;0x0000ff
//0;WAVE;0xff0000;0x0000ff;100;200
void loop() {
  if (Serial.available() > 0) {
    delay(2);  //delay to allow buffer to fill 
    
    //Serial.println(Serial.available());
    
    bool parse = false;
    while (Serial.available()) {
      char c = Serial.read();  //gets one byte from serial buffer
      if(c == '\n') {
        parse = true;
        readIndex = 0;
        break;
      }
      readString[readIndex] = c; //makes the string readString
      readIndex++;
      readString[readIndex] = '\0';
    }
    
    if(parse){
      parse = false;
      Serial.println(readString);
      
      char *p = readString;
      char *str;
      int i = 0;
      int strip = 0;
      char *mode;
      int numParams = count_chars(readString, ';') - 1;
      uint32_t params[numParams];
      while ((str = strtok_r(p, ";", &p)) != NULL){ // delimiter is the semicolon
        //Serial.print(i);
        //Serial.print(" ");
        //Serial.println(str);
        if(i == 0){
          strip = strtol(str, NULL, 10);
          if(strcmp(str, "A") == 0) strip = -1;
          Serial.print("Strip = ");
          Serial.println(strip);
        }
        if(i == 1) {
          mode = str;
          Serial.print("Mode = \"");
          Serial.print(mode);
          Serial.println("\"");
          if(numParams > 0){
            Serial.println("Params:");
          }
        }
        if(i > 1) {
          if(str[0] == '0' && str[1] == 'x'){
            params[i-2] = strtoul(str, NULL, 16);
          }else{
            params[i-2] = strtoul(str, NULL, 10);
          }
          Serial.print(i-2);
          Serial.print(" = ");
          Serial.println(params[i-2]);
        }
        i++;
      }

      for(int j = strip == -1 ? 0 : strip; j < (strip == -1 ? nStrips : strip+1); j++){
        if(strcmp(mode, "/BR") == 0){
          strips[j].brightness = params[0] / 100.0;
        }else if(strcmp(mode, "/RV") == 0){
          strips[j].reverse = params[0] == 1;
        }else{
          strips[j].setMode(getMode(mode));
          strips[j].setParams(params);
        }
      }
      
    }
  }
  for(int i = 0; i < nStrips; i++){
    strips[i].update();
  }
  
  fps++;
  if(millis() - lastFPS >= 1000){
    lastFPS = millis();
    Serial.print("UPS = ");
    Serial.println(fps);
    fps = 0;
  }
  
  delay(1); // give it a little break
}

int count_chars(const char* string, char ch){
    int count = 0;
    for(; *string; count += (*string++ == ch)) ;
    return count;
}

LEDStripMode* getMode(char* key){
  for(int i = 0; i < sizeof(keys)/2; i++){
    //Serial.println(keys[i]);
    if(strcmp(keys[i], key) == 0) {
      //Serial.print("Found key \"");
      //Serial.print(key);
      //Serial.print("\" at index ");
      //Serial.println(i);
      return modes[i];
    }
  }
  //Serial.print("No entry for key \"");
  //Serial.print(key);
  //Serial.println("\"");
  return modes[0];
}

int freeRam() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

