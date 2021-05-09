#ifndef CODE_H
#define CODE_H
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <map>
#include <string.h>
#include <stdio.h>
#include <algorithm>
#include "ExpressionTree.h"
#include "utils.h"
using namespace std;
#define MAXLENGTH 100

class Code{
private:
    int code_number;    //代码行序号
    string code_type;   //代码行种类
    string code_string; //代码行内容
    string grammer;
    string exp1, exp2, comment; //PRINT有exp1，IF有exp1、exp2，REM有comment
    string variable;        //LET, INPUT特有
    string compare_symbol;  //IF特有
    vector<string> structured_strings;
    int next_PC;
public:
    Code();
    Code(string);
    string getCodeType() {return code_type;}
    int getCodeNumber() {return code_number;}
    string getExp1() {return exp1;}
    string getExp2() {return exp2;}
    string getComment() {return comment;}
    string getVariable() {return variable;}
    string getCompareSymbol() {return compare_symbol;}
    string getCodeString() {return code_string;}
    int getNextPC() {return next_PC;}
    string getGrammer() {return grammer;}
    bool operator < (const Code &x) { //按照序号比大小
        return (this->code_number < x.code_number);
    }
    vector<string>* getStructuredStrings() { return &structured_strings; }
    void createGrammerTree();
};

class Codes{
private:
    vector<Code> codes_vec;
    vector<int> codes_hightlight_index;
public:
    Codes();
    vector<Code>* getCodesVec();
    void uniquePush(Code);
    void showCodes();
    void showGrammers();
    int searchByPC(int);
    int getNextPC(int);
    int getFirstPC();
    int getHightlightByIndex(int);
};


#endif // CODE_H
