#include "algorithm.h"
#include "mainwindow.h"
#include <map>
#include <queue>
#include <stdlib.h>





code_class::code_class(){

}

code_class::code_class(string s){
    code_string = s;
}

save_class saver;
extern queue<string>result_strings, gammer_strings;
extern string process_expression(string, int&, int);

//push进新代码，如果序号相同则覆盖，如果序号存在表达式为空则删除
void uniquePush(const code_class& a, vector<code_class>& codes_vector) {
    for (auto i = codes_vector.begin(); i != codes_vector.end(); ++i) { //此处O(N)查找可以降低到O(logN)
        if (a.code_number == (*i).code_number && a.code_type != "") {
            (*i) = a;
            return;
        }
        else if (a.code_number == (*i).code_number && a.code_type == "") {
            codes_vector.erase(i); //删除代码的情况
            return;
        }
        else continue;
    }
    if (a.code_type != "") codes_vector.push_back(a);
    sort(codes_vector.begin(), codes_vector.end());
    return;
}



string& replace_all(string& str, const string& old_value, const string& new_value)
{
    string::size_type pos=0;
    while((pos=str.find(old_value,pos))!= string::npos)
    {
        str=str.replace(pos,old_value.length(),new_value);
        if(new_value.length()>0)
        {
            pos+=new_value.length();
        }
    }
    return str;

}

//特判在控制台中直接输入的情况，以及input时输入数字的情况
int judge(string s, int &expression_value) {
    if (s.substr(0, 5) == "PRINT") {
        try {
            stringstream ssr;
            string codeNumber = ""; string tmpb;
            s = s.substr(6);
            ssr << replace_all(s, "=", " = ");
            string expression = "";
            while(ssr) {
                string tmp;
                ssr >> tmp;
                expression += tmp;
            }
            qDebug() << "expression: " << QString::fromStdString(expression) << Qt::endl;
            process_expression(expression,  expression_value, 1);
            return 2;
        }  catch (...) {
            return 0;
        }


    }
    else if (s.substr(0,3) == "LET") {
        try {
            stringstream ssr;
            string codeNumber = ""; string tmpb;
            string expression = "";
            string left_variable = "";
            s = s.substr(4);
            ssr << replace_all(s, "=", " = ");
            ssr >> left_variable >> tmpb; //按照 序号 命令 表达式的格式输入 102 LET A =
            while(ssr) {
                string tmp;
                ssr >> tmp;
                expression += tmp;
            }
            int expression_value = 0;

            process_expression(expression,  expression_value, 1);
            saver.variables[left_variable] = expression_value;
            qDebug() << QString::fromStdString(left_variable) << " = " << saver.variables[left_variable] << Qt::endl;
            return 3;
        }  catch (...) {
            return 0;
        }
    }
    else if (s.substr(0,5) == "INPUT") {
        stringstream ssr;
        string left_variable = "";
        s = s.substr(5);
        ssr << s;
        ssr >> left_variable;
        saver.input_variable = left_variable;
        saver.status = 6;
        return 5;
    }
    else if (saver.status == 2 || saver.status == 6) {
        try {
            stringstream ssr;
            string expression = "";
            ssr << s;
            while(ssr) {
                string tmp;
                ssr >> tmp;
                expression += tmp;
            }
            int local_expression = 0;
            qDebug() << "准备输入 = " << QString::fromStdString(saver.input_variable) << Qt::endl;
            process_expression(expression,  local_expression, 1);

            saver.variables[saver.input_variable] = local_expression;
            qDebug() << "输入了 " << saver.variables[saver.input_variable] << Qt::endl;
            return 4;
        }  catch (...) {
            return 0;
        }

    }
    else {
        code_class local_code(s);
        stringstream ss;
        ss << s;
        ss >> local_code.code_number >> local_code.code_type; //按照 序号 命令 表达式的格式输入
        const string &type = local_code.code_type;

        if (type == "" || type == "INPUT" || type == "REM" || type == "LET" || type == "IF" || type == "PRINT" || type == "GOTO" || type == "END") {
            uniquePush(local_code, saver.codes);
            return true;
        }
        else {
            uniquePush(local_code, saver.codes);
            return false;
        }
    }
}





//处理词法
int process_code(code_class *code_pointer, int flag = 1) {
    stringstream ss;
    string codeNumber = ""; string tmpb;
    code_class &local_code = *(code_pointer);

    ss << replace_all(local_code.code_string, "=", " = ");
    if (local_code.code_type == "PRINT") {

        string expression = "";
        ss >> codeNumber >> tmpb; //按照 序号 命令 表达式的格式输入
        while(ss) {
            string tmp;
            ss >> tmp;
            expression += tmp;
        }
        //得到表达式
        int expression_value = 0;
        string grammer_string = process_expression(expression,  expression_value, flag);
        qDebug() << "expression: " << QString::fromStdString(expression) << " = " << expression_value  << Qt::endl;
        if (flag == 1) {
            qDebug() << "添加到了result中" << expression_value << endl;
            result_strings.push(to_string(expression_value));
        }
        else if (flag == 0) local_code.grammer = codeNumber + " PRINT\n" + grammer_string;
        return 0;
    }
    else if (local_code.code_type == "LET") {
        string expression = "";
        string left_variable = "";
        ss >> codeNumber >> tmpb >> left_variable >> tmpb; //按照 序号 命令 表达式的格式输入 102 LET A =
        while(ss) {
            string tmp;
            ss >> tmp;
            expression += tmp;
        }
        int expression_value = 0;
        string grammer_string = process_expression(expression, expression_value, flag);
        saver.variables[left_variable] = expression_value;

        if (flag == 0)
        local_code.grammer =
                codeNumber + " LET =\n"
                + "    " + left_variable + '\n'
                + grammer_string;
        qDebug() << QString::fromStdString(left_variable) << " = " << saver.variables[left_variable] << Qt::endl;
        return 0;
    }
    else if (local_code.code_type == "REM") {
        string expression = "";
        string left_variable = "";
        ss >> codeNumber >> tmpb ; //按照 序号 命令 表达式的格式输入 102 REM XXX XXX XXX
        if (flag == 0) {
            local_code.grammer = codeNumber + " REM\n" + "    ";
            for (int i = 0; i < ss.str().length(); ++i) {
                if (ss.str().substr(i, 3) == "REM") {
                    local_code.grammer += ss.str().substr(i + 3);
                    break;
                }
            }
        }
        return 0;
    }
    else if (local_code.code_type == "GOTO") {
        int next_number = -1;
        ss >> codeNumber >> tmpb >> next_number; //按照 序号 命令 表达式的格式输入
        if (flag == 0)
        local_code.grammer =
                codeNumber + " GOTO\n"
                + "    " + to_string(next_number);
        return next_number; //没找到codes
    }
    else if (local_code.code_type == "INPUT") {
        string left_variable = "";
        ss >> codeNumber >> tmpb >> left_variable; //按照 序号 命令 表达式的格式输入
        if (flag == 0) {
            local_code.grammer =
                    codeNumber + " INPUT\n"
                    + "    " + left_variable;
        }
        else {
            saver.input_variable = left_variable;
            saver.input_code_number = stoi(codeNumber);
            qDebug() << "正在等待输入: 第 " << saver.input_code_number << " 行的： " << QString::fromStdString(saver.input_variable) << endl;
        }
        return -5;
    }
    else if (local_code.code_type == "IF") {
        int next_number = -1;
        ss >> codeNumber >> tmpb ; //按照 序号 命令 表达式的格式输入 100 IF a+3b < n1- n2 THEN 90
        string expression1 = "", expression2 = "";
        string left_variable = "";
        string op = "";
        int expression1_value = 0, expression2_value = 0;
        while(ss) {
            string tmp;
            ss >> tmp;
            if (tmp == ">" || tmp == "<" || tmp == "=") {
                op = tmp;
                break;
            }
            expression1 += tmp;
        }
        qDebug() << "expression1: " << QString::fromStdString(expression1) << Qt::endl;
        while(ss) {
            string tmp;
            ss >> tmp;
            if (tmp == "THEN") {
                break;
            }
            expression2 += tmp;
        }
        qDebug() << "expression2: " << QString::fromStdString(expression2) << Qt::endl;
        ss >> next_number;
        string grammer_string_1 = process_expression(expression1,  expression1_value, flag);
        string grammer_string_2 = process_expression(expression2,  expression2_value, flag);
        if (flag == 0)
        local_code.grammer =
                codeNumber + " IF THEN\n"
                +"    " + op + '\n'
                + grammer_string_1 + '\n'
                + grammer_string_2 + '\n'
                + "    " +  to_string(next_number);
        if (((op == ">") && (expression1_value > expression2_value)) || ((op == "<") && (expression1_value < expression2_value)) || ((op == "=") && (expression1_value == expression2_value))) {
            return next_number;
        }
        return 0; //没找到codes
    }
    else if (local_code.code_type == "END") {
        ss >> codeNumber;
        if (flag == 0)
        local_code.grammer =
                codeNumber + " END\n";
        return -2;
    }
    else { //非法指令的情况
        ss >> codeNumber;
        int codeNumber_int = -9999;
        try {
            codeNumber_int = atoi(codeNumber.c_str());
        }  catch (...) {
            local_code.grammer = "";
            return -3;
        }
        if (flag == 0 && codeNumber_int > 0) {
            local_code.grammer =
                    codeNumber + " CODE ERROR";
        }
        return -3;
    }
}


void execute_codes(vector<code_class>& codes_vector) { //执行CODEs
    if (saver.status == 0) saver.status = 1;

    //生成表达式树
    for (auto i = codes_vector.begin(); i != codes_vector.end(); ++i) {
        code_class *frontCode = &(*i);
        int next_number = process_code(frontCode, 0);
    }

    //生成结果字符串
    for (auto i = codes_vector.begin(); i != codes_vector.end(); ++i) { //此处O(N)查找可以降低到O(logN)
        if (saver.status == 3) {
            qDebug() << "恢复input 的情况" << endl;
            bool found = false;
            for (auto j = codes_vector.begin(); j != codes_vector.end(); ++j) { //此处O(N)查找可以降低到O(logN)
                if ((*j).code_number == saver.input_code_number) {
                    i = j;
                    found = true;
                    qDebug() << "INPUT found!" << (*j).code_number << endl;
                    saver.status = 1;
                    break;
                }
            }
            if (found == false) return; //没找到的情况
            else {
                continue;
            }
        }
        saver.pendingCodes.push(&(*i));
        code_class *frontCode = saver.pendingCodes.front(); saver.pendingCodes.pop();
        qDebug() << "CODE：" << QString::fromStdString(frontCode->code_string) << Qt::endl;
        int next_number;
        try {
            next_number = process_code(frontCode, 1);
        }  catch (...) {
            return;
        }
        if (next_number == -3) { //非法指令的情况
            saver.status = 4;
            return;
        }
        if (next_number == -5) { //INPUT的情况
            qDebug() << "在循环中发现这是一个输入的情况！" << endl;
            saver.status = 2;
            return;
        }
        if (next_number > 0) {
            bool found = false;
            for (auto j = codes_vector.begin(); j != codes_vector.end(); ++j) { //此处O(N)查找可以降低到O(logN)
                if ((*j).code_number == next_number) {
                    i = j;
                    found = true;
                    qDebug() << "GOTO found!" << (*j).code_number << endl;
                    break;
                }
            }
            if (found == false) return; //没找到的情况
            i -= found;
        }
        else if (next_number == -2) { //end的情况
             break;
        }
    }
    qDebug() << "代码执行结束，准备打印" << endl;
    return;
}
