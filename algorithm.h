#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

class code_class{
public:

    int code_number;
    string code_type;
    string code_string;
    code_class();
    code_class(string);
    bool operator <(const code_class &x) {
        return (this->code_number < x.code_number);
    }
};


bool judge(string);

#endif // ALGORITHM_H
