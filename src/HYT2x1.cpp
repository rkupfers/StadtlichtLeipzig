#include "string.h"
#include "mbed.h"
#include "rtos.h"
#include "HYT2x1.h"

HYT2x1::HYT2x1(I2C *con, const unsigned char addr, Serial *serial){
    address = (int) addr;
    connection = con;
    debug = (serial != NULL) ? true : false;
    pc = serial;
}

float HYT2x1::getTemp(){
    
    int help = 0;
    float temp = 0;
    
    help = (buffer[2] << 6) | (buffer[3] >> 2);
    temp = ((165.0 / 16384) * help) - 40.0;
    if(debug)   pc->printf("Temperature: %f \n\r",temp);
    return temp;
}
float HYT2x1::getHumid(){
    
    int help = 0;
    float humid = 0;

    help = ((buffer[0] & 0x3F) << 8) | buffer[1];
    humid = (100.0 / 16384) * help;
    if(debug) pc->printf("Humidity: %f \n\r",humid);
    return humid;
}
bool HYT2x1::update(){

    int help = 0, i = 0;
    
    if(connection->write(address << 1,buffer,0) != 0)
        return false;
    if(debug) pc->printf("write was: %s \n\r",(help == 0) ? "sucessfull" : "faulty");
        
    do{
        Thread::wait(30);
        help = connection->read((address << 1), buffer, 4);
        if(debug) pc->printf("read was: %s \n\r",(help == 0) ? "sucessfull" : "faulty");
        if( help == 0){
            if(debug) pc->printf("raw sensorvalue: %x, %x, %x, %x \n\r",buffer[0],buffer[1],buffer[2],buffer[3]);
            if(debug) pc->printf("new sensorvalue: %s %x\n\r",((buffer[0] & 0x40) == 0) ? "yes" : "no", (buffer[0] & 0x40));
        }
        i++;
    }while((buffer[0] & 0x40) == 1 && i < 15);
    if(help != 0 || (buffer[0] & 0x40) == 1 || i >= 15)
        return false;
        
    return true;
}

bool HYT2x1::setAdress(const unsigned char addr, const PinName vdd){

    char buffer[3] = {0xA0,0x00,0x00};
    DigitalOut *VDD = new DigitalOut(vdd);
    
    *VDD = 0;
    Thread::wait(100);
    *VDD = 1;
    wait_ms(5); 
    if(connection->write(address << 1,buffer,3) != 0)
        return false;
    Thread::wait(100);
    buffer[0] = 0x5C; buffer[2] =addr;
    if(connection->write(address << 1,buffer,3) != 0)
        return false;
    Thread::wait(100);
    buffer[0] = 0x80; buffer[2] =0;
    if(connection->write(address << 1,buffer,3) != 0)
        return false;
    Thread::wait(100);
    if(debug) pc->printf("Address Changed!\n\r");
    address = (int) addr;
    
    free(VDD);
    
    return true;
}