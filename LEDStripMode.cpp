// LEDStripMode.cpp
// Implementation of LEDStripMode.h
// Basically contains


#include "LEDStrip.h"
#include "LEDStripMode.h"

void LEDStripMode::setParams(uint32_t params[]){
  for(int i = 0; i < 10; i++){
    this->params[i] = params[i];
  }
}

void LEDStripModeOff::render(LEDStrip* strip) {
  strip->setAll(strip->Color(0, 0, 0));
}

void LEDStripModeSolid::render(LEDStrip* strip) {
  strip->setAll(params[0]);
}

void LEDStripModeChaseTest::render(LEDStrip* strip) {
  uint32_t c1 = strip->Color(100, 0, 0);
  uint32_t c2 = strip->Color(0, 100, 0);

  for(int i = 0; i < strip->_strip.numPixels(); i++) {
    if((millis() + (i*20)) % 500 >= 250){
      strip->set(i, c1);
    }else{
      strip->set(i, c2);
    }
  }
}

float bounce_ball[][2] = {
  {0, 0},
  {1, 0},
  {2, 0},
  {3, 0},
  {4, 0},
  {5, 0},
  {6, 0},
  {7, 0},
  {8, 0},
  {9, 0}
};
float bounce_acc = 100.0;
//BOUNCE;6;0xff0000;0x00ff00;0x0000ff;0xffff00;0xff00ff;0x00ffff
//BOUNCE;3;0xff0000;0x00ff00;0x0000ff
void LEDStripModeBounce::render(LEDStrip* strip) {
  long now = millis();
  long dt = now - lastUpdate;
  uint32_t c0 = strip->Color(0, 0, 0);
  
  int pixels = strip->_strip.numPixels();
  
  //Serial.println(bounce_ball[0][0]);
  
  for(int i = 0; i < params[0]; i++){
    if(bounce_ball[i][0] <= 0) {
      bounce_ball[i][1] *= -0.9;
      bounce_ball[i][0] = 0;
      if(bounce_ball[i][1] < 6){
        float d = (random(60, 100) / 100.0) * (pixels - 1);
        float vi = sqrt(2 * bounce_acc * d); // vf^2 = vi^2 + 2ad -> vi = sqrt(2ad)
        bounce_ball[i][1] = vi;
      }
    } else {
      bounce_ball[i][1] -= bounce_acc * (dt / 1000.0);
    }
    
    bounce_ball[i][0] += bounce_ball[i][1] * (dt / 1000.0);
  }
  
  for(int i = 0; i < pixels; i++) {
    
    strip->set(i, c0);  
    
    uint32_t col = 0;
    for(int j = 0; j < params[0]; j++){
      float dist = abs(bounce_ball[j][0] - i);
      if(dist < 1){
        if(col == 0){
          col = strip->lerp(params[j+1], c0, dist);
        }else{
          col = strip->lerp(strip->lerp(params[j+1], c0, dist), col, dist);
        }
      }
    }
    
    strip->set(i, col);
    
  }
  
}

//CHASE;50;1000;500;0xff0000;0x0000ff
void LEDStripModeChase::render(LEDStrip* strip) {
  uint32_t c1 = params[3];
  uint32_t c2 = params[4];

  for(int i = 0; i < strip->_strip.numPixels(); i++) {
    if((millis() + (i*params[0])) % params[1] >= params[2]){
      strip->set(i, c1);
    }else{
      strip->set(i, c2);
    }
  }
}

int fpsPos = 0;
//FPS
void LEDStripModeFPSTest::render(LEDStrip* strip) {
  uint32_t c1 = 0xff0000;
  uint32_t c2 = 0x000000;

  fpsPos = (fpsPos + 1) % strip->_strip.numPixels();

  for(int i = 0; i < strip->_strip.numPixels(); i++) {
    if(i == fpsPos){
      strip->set(i, c1);
    }else{
      strip->set(i, c2);
    }
  }
}

//WAVE;0xff0000;0x0000ff;100;200
void LEDStripModeWave::render(LEDStrip* strip) {

  uint32_t lightBlue = params[0];
  uint32_t hotPink = params[1];
  int height = params[2];

  for(int i = 0; i < strip->_strip.numPixels(); i++){
    strip->set(i, lightBlue);
    if(i+1 < ((sin((2 * PI * millis())/(float)params[3]) + 1) / 2) * (strip->_strip.numPixels() + 1) * (height / 100.0)){
      strip->set(i, lightBlue);
    } else {
      strip->set(i, hotPink);
    }
  }
}

//FADE;0xff0000;0x0000ff;1000;50
void LEDStripModeFade::render(LEDStrip* strip) {

  uint32_t c1 = params[0];
  uint32_t c2 = params[1];

  for(int i = 0; i < 69; i++){
    strip->set(i, strip->lerp(c1, c2, ((sin((2 * PI * (millis() + params[3]*i))/(float)params[2]) + 1) / 2)));
  }
}

