#include "algorithm.h"
#include "mainwindow.h"
#include <map>
#include <queue>



vector <code_class> codes;
map<string, int> variables;

code_class::code_class(){

}

code_class::code_class(string s){
    code_string = s;
}

//push进新代码，如果序号相同则覆盖
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
    ss >> local_code.code_number >> local_code.code_type; //按照 序号 命令 表达式的格式输入
    const string &type = local_code.code_type;

    if (type == "REM" || type == "LET" || type == "IF" || type == "PRINT" || type == "GOTO" || type == "END") {
        uniquePush(local_code, codes);
        return true;
    }
    else {
        uniquePush(local_code, codes);
        return false;
    }

}

extern string process_expression(string, map<string, int>&, int&);


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

//处理
int process_code(queue <string> &result_strings, queue <string> &grammer_strings, code_class *code_pointer) {
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
        qDebug() << "expression: " << QString::fromStdString(expression) << Qt::endl;
        int expression_value = 0;
        string grammer_string = process_expression(expression, variables, expression_value);
        result_strings.push(to_string(expression_value));
        local_code.grammer = codeNumber + " PRINT\n" + grammer_string;
        grammer_strings.push(codeNumber + " PRINT");
        grammer_strings.push(grammer_string);
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
        string grammer_string = process_expression(expression, variables, expression_value);
        variables[left_variable] = expression_value;
        local_code.grammer =
                codeNumber + " LET =\n"
                + "    " + left_variable + '\n'
                + grammer_string;
        qDebug() << QString::fromStdString(left_variable) << " = " << variables[left_variable] << Qt::endl;
        return 0;
    }
    else if (local_code.code_type == "REM") {
        string expression = "";
        string left_variable = "";
        ss >> codeNumber >> tmpb ; //按照 序号 命令 表达式的格式输入 102 REM XXX XXX XXX
        local_code.grammer = codeNumber + " REM\n" + "    ";
        for (int i = 0; i < ss.str().length(); ++i) {
            if (ss.str().substr(i, 3) == "REM") {
                local_code.grammer += ss.str().substr(i + 3);
                break;
            }
        }
        return 0;
    }
    else if (local_code.code_type == "GOTO") {
        int next_number = -1;
        ss >> codeNumber >> tmpb >> next_number; //按照 序号 命令 表达式的格式输入
        local_code.grammer =
                codeNumber + " GOTO\n"
                + "    " + to_string(next_number);
        return next_number; //没找到codes
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
        string grammer_string_1 = process_expression(expression1, variables, expression1_value);
        string grammer_string_2 = process_expression(expression2, variables, expression2_value);
        local_code.grammer =
                codeNumber + " IF THEN\n"
                +"    " + op + '\n'
                + grammer_string_1 + '\n'
                + grammer_string_2 + '\n'
                + "    " +  to_string(next_number);
        qDebug() << "next_number: " << next_number << Qt::endl;
        qDebug() << "expression1_value: " << expression1_value << Qt::endl;
        qDebug() << "expression2_value: " << expression2_value << Qt::endl;
        if (((op == ">") && (expression1_value > expression2_value)) || ((op == "<") && (expression1_value < expression2_value)) || ((op == "=") && (expression1_value == expression2_value))) {
            qDebug() << "into it! " << Qt::endl;
            return next_number;
        }
        qDebug() << "not into it! " << Qt::endl;
        return 0; //没找到codes
    }
    else if (local_code.code_type == "END") {
        ss >> codeNumber;
        local_code.grammer =
                codeNumber + " END\n";
        return -2;
    }
    else { //非法指令的情况
        ss >> codeNumber;
        local_code.grammer =
                codeNumber + " CODE ERROR\n";
        return -2;
    }
}

void execute_codes(vector<code_class>& codes_vector, queue <string> &result_strings, queue <string> &grammer_strings) { //执行CODEs
    queue <code_class*> pendingCodes;
    for (auto i = codes_vector.begin(); i != codes_vector.end(); ++i) { //此处O(N)查找可以降低到O(logN)
        pendingCodes.push(&(*i));
        code_class *frontCode = pendingCodes.front(); pendingCodes.pop();
        qDebug() << "CODE：" << QString::fromStdString(frontCode->code_string) << Qt::endl;
        int next_number;
        try {
            next_number = process_code(result_strings, grammer_strings, frontCode);
        }  catch (...) {
            qDebug() << "Code Error!" << endl;
            grammer_strings.push("Code Error!");
            result_strings.push("Code Error!");
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
             return;
        }
    }
}
