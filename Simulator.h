#ifndef SIMULATOR_H
#define SIMULATOR_H
#include <vector>
#include <map>
#include <cstring>
#include "Code.h"
using namespace std;

enum status { SAOK = 0, SHLT, SINS, SFIN};

class Sim{
private:
    map<string, int> variables;
    status now_status;
    int now_PC;
    Codes *codes;
    string latest_out;
    Code latest_code;
public:
    Sim(Codes*);
    status getStatus() { return now_status; }
    void setStatus(status a) {now_status = a; }
    void singleStepSim(int &);
    string showVariables();
    void refreshCodes(Codes*);
    int get_PC() { return now_PC; }
};


#endif // SIMULATOR_H
