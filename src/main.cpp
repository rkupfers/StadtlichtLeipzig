#include "mbed.h"
#include "rtos.h"
#include "DCF77.h"
#include "HYT2x1.h"
#include "Logging.h"
#include "Rain.h"

Serial pc(USBTX, USBRX);
I2C hygsens(p9, p10);        // sda, scl 
LocalFileSystem local("local");

using namespace std;

int main(){

    pc.printf("Programmstart\n\r");
    
    Logging logger(&pc);
    DCF77::timeDCF77 *t;
    DCF77 Sensor1(p6, true, &pc, &logger);
    HYT2x1 Sensor2(&hygsens, 0x28);
    HYT2x1 Sensor3(&hygsens, 0x20);
    Rain Sensor4(p7, &pc);
    char buffer[150];
    
    if(Sensor1.recieveTime()){
        t = Sensor1.getTime();
        pc.printf("%i %i am %s den %i %i %i\n\r",t->hour,t->minute,t->weekday,t->day,t->month,t->year);
        sprintf(buffer,"Time successfully recieved: %i %i am %s den %i %i %i\n\r",t->hour,t->minute,t->weekday,t->day,t->month,t->year);
        logger.log(buffer);
        Sensor1.setRTCClock(t,0);
    }
    pc.printf("Erzeugung ok\n\r");
    
    while(1){
        if(Sensor2.update()){
            pc.printf("update was: sucessfull \n\r");
            sprintf(buffer,"Temperature: %3.3f \n\r",Sensor2.getTemp());
            logger.log(buffer);
            sprintf(buffer,"Humidity: %3.3f \n\r",Sensor2.getHumid());
            logger.log(buffer);
        }else
            pc.printf("update was: faulty \n\r");
            
        if(Sensor3.update()){
            pc.printf("update was: sucessfull \n\r");
            sprintf(buffer,"Temperature: %3.3f \n\r",Sensor3.getTemp());
            logger.log(buffer);
            sprintf(buffer,"Humidity: %3.3f \n\r",Sensor3.getHumid());
            logger.log(buffer);
        }else
            pc.printf("update was: faulty \n\r");
            
        sprintf(buffer,"Rain State: %s \n\r",(Sensor4.getRain()) ? "raining":"not raining");
        logger.log(buffer);
                
        Thread::wait(6000); //6s
    }
}