#include "algorithm.h"
#include "mainwindow.h"
#include <map>



vector <code_class> codes;
map<string, int> variables;

code_class::code_class(){

}

code_class::code_class(string s){
    code_string = s;
}

void uniquePush(const code_class& a, vector<code_class>& codes_vector) {
    for (auto i = codes_vector.begin(); i != codes_vector.end(); ++i) { //此处O(N)查找可以降低到O(logN)
        if (a.code_number == (*i).code_number) {
            (*i) = a;
            return;
        }
        else continue;
    }
    codes_vector.push_back(a);
    sort(codes_vector.begin(), codes_vector.end());
    return;
}

bool judge(string s) {
    code_class local_code(s);
    stringstream ss;
    ss << s;
    ss >> local_code.code_number >> local_code.code_type;
    const string &type = local_code.code_type;

    if (type == "REM") {
        qDebug() << QString::fromStdString(ss.str()) << Qt::endl;
        uniquePush(local_code, codes);
        return true;
    }
    else if (type == "LET") {
        string variable, equal; int value;
        ss >> variable >> equal >> value; //目前把value当做常数，之后要修改为表达式求值
        if (equal == "=") {
            uniquePush(local_code, codes);
            return true;
        }
        else {
            return false;
        }
    }
    else if (type == "IF") return true;
    else if (type == "PRINT") return true;
    else if (type == "GOTO") return true;
    else if (type == "END") return true;
    return false;
//    while(ss) {
//        string local_string;
//        ss >> local_string;
//        qDebug() << QString::fromStdString(local_string) << Qt::endl;
//    }
//    return true;
}
