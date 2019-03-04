 #include <stdint.h>

class LEDStrip;

class LEDStripMode {
  public:
    uint32_t params[20];
    long lastUpdate;
    
    virtual void render(LEDStrip* strip) = 0;
    void setParams(uint32_t params[]);
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
