#ifndef HYT2x1_H
#define HYT2x1_H
using namespace std;

class HYT2x1{
    
    public:
    
    HYT2x1(I2C *con, const unsigned char addr, Serial *serial = NULL);
    float getTemp();
    float getHumid();
    bool update();
    bool setAdress(const unsigned char addr, const PinName pin);

    private:
    
    bool debug;
    int address;
    char buffer[4];
    I2C *connection;
    Serial *pc;
    
};

#endif 