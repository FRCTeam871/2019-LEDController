// LEDStrip.h
// Headers for LEDStrip

#include <Adafruit_NeoPixel.h>

class LEDStripMode;

class LEDStrip {
  public:
    int _p1;
    int _p2;
    int _p3;
    int _p4;
    uint32_t params[10];
    float brightness;
    bool reverse;
    
    void setParams(uint32_t params[]);
    void setParams(int num, ...);
    
    LEDStrip(Adafruit_NeoPixel strip);
    
    void setMode(LEDStripMode* mode);

    void update();

    void setAll(uint32_t color);
    void set(int i, uint32_t c);

    uint32_t Color(int r, int g, int b);

    Adafruit_NeoPixel _strip;
    
    LEDStripMode* _mode;
    
    uint32_t lerp(uint32_t c1, uint32_t c2, float t);
};

