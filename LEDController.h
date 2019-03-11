// LEDController.h
// Headers for LEDController

#define MAX_STRIPS 10

class LEDController {

    public:
        int _numStrips;
        LEDStrip* _strips[MAX_STRIPS];
        
        void init();
        int addStrip(int numPixels, int pin);
        void update();
        void setMode(int i, LEDStripMode* mode);
        void setParams(int i, int num, ...);
        void setParams(int i, uint32_t params[]);
        void setBrightness(int i, float brightness);
        void setReverse(int i, bool reverse);
        
        void handleInput(char readString[]);
        
        LEDController();
};