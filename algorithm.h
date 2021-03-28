#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

class code_class{
public:

    int code_number;//代码行序号
    string code_type; //代码行内容
    string code_string;//代码行
    string grammer;
    code_class();
    code_class(string);
    bool operator <(const code_class &x) { //按照序号比大小
        return (this->code_number < x.code_number);
    }
};


bool judge(string);

#endif // ALGORITHM_H
