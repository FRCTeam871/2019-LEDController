// LEDController.cpp
// Implementation of LEDController.h
// Wraps several LEDStrips and handles input

#include "LEDStrip.h"
#include "LEDStripMode.h"

#include "LEDController.h"

LEDStripModeOff mOff;
LEDStripModeSolid mSolid;
LEDStripModeChase mChase;
LEDStripModeBounce mBounce;
LEDStripModeWave mWave;
LEDStripModeFade mFade;
LEDStripModeFire mFire;
LEDStripModeFPSTest mFPS;
LEDStripModeRainbow mRainbow;
LEDStripModeRainbowChase mRainbowChase;
LEDStripModeBinary mBinary;

LEDStripMode *modes[] = {&mOff, &mSolid, &mChase, &mBounce, &mWave, &mFade, &mFire, &mFPS, &mRainbow, &mRainbowChase, &mBinary};
char *keys[] = {"OFF", "SOLID", "CHASE", "BOUNCE", "WAVE", "FADE", "FIRE", "FPS", "RAINBOW", "RAINBOWCHASE", "BINARY"};

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

LEDController::LEDController(){

}

void LEDController::init(){
    _numStrips = 0;
}

void LEDController::addStrip(LEDStrip* str){
  str->_strip.show();
  //lstr._strip.show();
  _strips[_numStrips] = str;
  _numStrips++;
  str->setMode(&mChase);
  str->setParams(5, (uint32_t)100, (uint32_t)1000, (uint32_t)500, 0xff0000, 0x0000ff);

}

void LEDController::update(){
  for(int i = 0; i < _numStrips; i++){
      _strips[i]->update();
//      Serial.println(i);
  }
}

void LEDController::setMode(int i, LEDStripMode* mode){
  _strips[i]->setMode(mode);
}

void LEDController::setParams(int i, int num, ...){
  va_list arguments;
  va_start (arguments, num);
  uint32_t ar[10];
  
  for(int i = 0; i < 10; i++){
    if(i < num){
    ar[i] = va_arg(arguments, uint32_t);
    }else{
      ar[i] = 0;
    }
    //Serial.println(ar[i]);
  }
  
  va_end (arguments);
  _strips[i]->setParams(ar);
}

void LEDController::setParams(int i, uint32_t* params){
  _strips[i]->setParams(params);
}

void LEDController::setBrightness(int i, float brightness){
  _strips[i]->brightness = brightness;
}

void LEDController::setReverse(int i, bool reverse){
  _strips[i]->reverse = reverse;
}

void LEDController::handleInput(char readString[]){
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
      if(str[0] == 'R'){
        params[i-2] = RAINBOW_CONST;
      }else if(str[0] == '0' && str[1] == 'x'){
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

  for(int j = strip == -1 ? 0 : strip; j < (strip == -1 ? _numStrips : strip+1); j++){
    if(strcmp(mode, "/BR") == 0){
      setBrightness(j, params[0] / 100.0);
    }else if(strcmp(mode, "/RV") == 0){
      setReverse(j, params[0] == 1);
    }else{
      setMode(j, getMode(mode));
      setParams(j, params);
    }
  }
}
