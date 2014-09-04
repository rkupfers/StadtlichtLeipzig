#ifndef Logging_H
#define Logging_H

using namespace std;

class Logging{

    public:
    Logging(Serial *serial){ pc=serial; buffer[0] = '\0';};
    bool log(char Log[]);
    private:
    FILE *fp;
    char buffer[600];
    void write_persistent();
    Serial *pc;
};
#endif 