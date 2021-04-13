#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <map>
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

class save_class {
public:
    int status = 0;
    //status = 0，初始化阶段，还未开始执行代码，可以清空一切
    //status = 1，代码执行阶段，可能是点击了run，可能是在外部使用了let，此时代码不能被修改
    //status = 2，input阶段，需要处理input
    vector <code_class> codes;
    map<string, int> variables;
    queue <code_class*> pendingCodes;
    string input_variable = "";
    int input_code_number = 0;
    void reinit() {
        codes.clear();
        variables.clear();
        while(pendingCodes.empty() != true) {
            pendingCodes.pop();
        }
    }
};

bool judge(string);

#endif // ALGORITHM_H
