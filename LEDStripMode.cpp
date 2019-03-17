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

uint32_t LEDStripMode::Color(int r, int g, int b){
  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
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

  for(int i = 0; i < strip->numPixels(); i++) {
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
  
  int pixels = strip->numPixels();
  
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

  for(int i = 0; i < strip->numPixels(); i++) {
//    Serial.println(i);
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

  fpsPos = (fpsPos + 1) % strip->numPixels();

  for(int i = 0; i < strip->numPixels(); i++) {
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

  for(int i = 0; i < strip->numPixels(); i++){
    strip->set(i, lightBlue);
    if(i+1 < ((sin((2 * PI * millis())/(float)params[3]) + 1) / 2) * (strip->numPixels() + 1) * (height / 100.0)){
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

#define FIRE_SAMPLES 70
long fire_lastTick = 0;
uint8_t fire_heat[FIRE_SAMPLES];
uint32_t fire_color[FIRE_SAMPLES];
void LEDStripModeFire::render(LEDStrip* strip) {

  //Serial.println("render fire");
  if(millis() - fire_lastTick > 0) tickFire(55, 200);
  for(int i = 0; i < strip->numPixels(); i++){
    strip->set(i, strip->HueRotate(fire_color[(int)(i / (float)strip->numPixels() * FIRE_SAMPLES)], params[0]));
  }
}

void LEDStripModeFire::tickFire(int Cooling, int Sparking) {
 
  fire_lastTick = millis();
 
  //Serial.println("tickFire");
  int cooldown;
  
  // Step 1.  Cool down every cell a little
  for( int i = 0; i < FIRE_SAMPLES; i++) {
    cooldown = random(0, ((Cooling * 10) / FIRE_SAMPLES) + 2);
    
    if(cooldown>fire_heat[i]) {
      fire_heat[i]=0;
    } else {
      fire_heat[i]=fire_heat[i]-cooldown;
    }
  }
  
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k= FIRE_SAMPLES - 1; k >= 2; k--) {
    fire_heat[k] = (fire_heat[k - 1] + fire_heat[k - 2] + fire_heat[k - 2]) / 3;
  }
    
  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if( random(255) < Sparking ) {
    int y = random(7);
    fire_heat[y] = fire_heat[y] + random(160,255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for( int j = 0; j < FIRE_SAMPLES; j++) {
    //Serial.print((int)((fire_heat[j] / 255.0) * 9));
    //Serial.print(" ");
    setPixelHeatColor(j, fire_heat[j] );
  }
  //Serial.println();
}

void LEDStripModeFire::setPixelHeatColor (int Pixel, uint8_t temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  int t192 = round((temperature/255.0)*191);
 
  // calculate ramp up from
  int heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
 
  if( t192 > 0x80) {                     // hottest
    if(heatramp > 50) heatramp = 50;
    fire_color[Pixel] = Color(255, 200, 0);
  } else if( t192 > 0x40 ) {             // middle
    fire_color[Pixel] = Color(255, heatramp * 0.8, 0);
  } else {                               // coolest
    fire_color[Pixel] = Color(heatramp, heatramp/20, 0);
  }
}

void LEDStripModeRainbow::render(LEDStrip* strip) {
  strip->setAll(strip->HueRotate(255, 0, 0, (int)(millis() / (double)params[0] * 360) % 360));
}

void LEDStripModeRainbowChase::render(LEDStrip* strip) {
  for(int i = 0; i < strip->numPixels(); i++){
    strip->set(i, strip->HueRotate(255, 0, 0, (int)((millis() + params[1] * i) / (double)params[0] * 360) % 360));
  }
}

