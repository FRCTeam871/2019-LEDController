 #include <stdint.h>

class LEDStrip;

class LEDStripMode {
  public:
    uint32_t params[20];
    long lastUpdate;
    
    virtual void render(LEDStrip* strip) = 0;
    void setParams(uint32_t params[]);
    uint32_t Color(int r, int g, int b);
    //virtual ~LEDStrip*Mode(){}
};

class LEDStripModeOff : public LEDStripMode {
  public:
    virtual void render(LEDStrip* strip);
};

class LEDStripModeSolid : public LEDStripMode {
  public:
    virtual void render(LEDStrip* strip);
};

class LEDStripModeChaseTest : public LEDStripMode {
  public:
    virtual void render(LEDStrip* strip);
};

class LEDStripModeFPSTest : public LEDStripMode {
  public:
    virtual void render(LEDStrip* strip);
};

class LEDStripModeMeme : public LEDStripMode {
  public:
    virtual void render(LEDStrip* strip);
};

class LEDStripModeBounce : public LEDStripMode {
  public:
    virtual void render(LEDStrip* strip);
};

class LEDStripModeChase : public LEDStripMode {
  public:
    virtual void render(LEDStrip* strip);
};

class LEDStripModeWave : public LEDStripMode {
  public:
    virtual void render(LEDStrip* strip);
};

class LEDStripModeFade : public LEDStripMode {
  public:
    virtual void render(LEDStrip* strip);
};

class LEDStripModeFire : public LEDStripMode {
  public:
    virtual void render(LEDStrip* strip);
    void tickFire(int Cooling, int Sparking);
    void setPixelHeatColor(int Pixel, uint8_t temperature);
};

class LEDStripModeRainbow : public LEDStripMode {
  public:
    virtual void render(LEDStrip* strip);
};
