// LEDStrip.cpp
// Implementation of LEDStrip.h
// Basically wraps an Adafruit_NeoPixel or OctoWS2811 (depending on is USE_OCTOWS2811 is set) and adds some functionality

#include "LEDStripMode.h"
#include "LEDStrip.h"

#if USE_OCTOWS2811
LEDStrip::LEDStrip(OctoWS2811 strip){
  _strip = strip;
  brightness = 0.5;
  reverse = false;
}
#else
LEDStrip::LEDStrip(Adafruit_NeoPixel strip){
  _strip = strip;
  brightness = 0.5;
  reverse = false;
}
#endif


void LEDStrip::setParams(uint32_t* params){
  for(int i = 0; i < 10; i++){
    this->params[i] = params[i];
  }
}

void LEDStrip::setParams(int num, ...){
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
  
  setParams(ar);
}

void LEDStrip::setMode(LEDStripMode* mode){
  _mode = mode;
  _mode->lastUpdate = millis();
}

void LEDStrip::update(){
  _mode->setParams(params);
  _mode->render(this);
  _mode->lastUpdate = millis();
  _strip.show();
}

void LEDStrip::setAll(uint32_t color){
  for(uint16_t i = 0; i < _strip.numPixels(); i++) {
    set(i, color);
  }
}

void LEDStrip::set(int i, uint32_t c){
  if(reverse) i = _strip.numPixels() - i - 1;
  #if USE_OCTOWS2811
  _strip.setPixel(i, lerp(0x000000, c, brightness));
  #else
  _strip.setPixelColor(i, lerp(0x000000, c, brightness));
  #endif
}

uint32_t LEDStrip::Color(int r, int g, int b){
  #if USE_OCTOWS2811
  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
  #else
  return _strip.Color(r, g, b);
  #endif
}

uint32_t LEDStrip::lerp(uint32_t c1, uint32_t c2, float t){
  int b1 = (c1)&0xFF;
  int g1 = (c1>>8)&0xFF;
  int r1 = (c1>>16)&0xFF;
  
  int b2 = (c2)&0xFF;
  int g2 = (c2>>8)&0xFF;
  int r2 = (c2>>16)&0xFF;
  
  int r = (int)(r1 + t * (r2 - r1));
  int g = (int)(g1 + t * (g2 - g1));
  int b = (int)(b1 + t * (b2 - b1));

  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}

