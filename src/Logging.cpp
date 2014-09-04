#include "mbed.h"
#include "rtos.h"
#include "Logging.h"

bool Logging::log(char Log[])
{
    time_t t2 = time(NULL);
    char buf[50]; 
    
    sprintf(buf,"%s", ctime(&t2));
    strcat(buffer,buf);
    strcat(buffer,Log);
    if(strlen(buffer)>450){
        strcat(buffer,"\0");
        write_persistent();
        pc->printf("Written\r\n");
        buffer[0]='\0';
    }
    return true;
}

void Logging::write_persistent()
{
    fp = fopen("/local/Logging.txt", "a");
    fprintf(fp, buffer);
    fclose(fp);
}