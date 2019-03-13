// LEDStrip.h
// Headers for LEDStrip

#define USE_OCTOWS2811 1

#if USE_OCTOWS2811
#include <OctoWS2811.h>
#else
#include <Adafruit_NeoPixel.h>
#endif

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
    
    #if USE_OCTOWS2811
    LEDStrip(OctoWS2811 strip);
    #else
    LEDStrip(Adafruit_NeoPixel strip);
    #endif
    
    void setMode(LEDStripMode* mode);

    void update();

    void setAll(uint32_t color);
    void set(int i, uint32_t c);

    uint32_t Color(int r, int g, int b);
    
    #if USE_OCTOWS2811
    OctoWS2811 _strip = OctoWS2811(0, NULL, NULL, WS2811_GRB);
    #else
    Adafruit_NeoPixel _strip;
    #endif
    
    LEDStripMode* _mode;
    
    uint32_t lerp(uint32_t c1, uint32_t c2, float t);
};

