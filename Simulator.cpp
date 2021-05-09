#include "Simulator.h"


Sim::Sim(Codes* codes_pointer) {
    variables.clear();
    now_status = SAOK;
    now_PC = codes_pointer->getFirstPC();
    codes = codes_pointer;
    latest_out = "";
}

void Sim::refreshCodes(Codes* codes_pointer) {
    variables.clear();
    now_status = SAOK;
    now_PC = codes_pointer->getFirstPC();
    codes = codes_pointer;
    latest_out = "";
}


void Sim::singleStepSim(int &return_status) {
    return_status = now_status;
    if (now_status == SFIN) {
        cout << "程序运行结束！" << endl;
        return;
    }
    else if (now_status == SINS) {
        cout << "程序运行过程中出现问题！" << endl;
        return;
    }
    else if (now_status == SHLT) {
        cout << "程序暂停，等待中！" << endl;
        return;
    }
    int PC = now_PC;

    int status = codes->searchByPC(PC);
    if (status == -1) {
        cout << "没有找到 PC =  " << PC << endl;
        now_status = SINS;  //没有找到当前PC的情况
        return_status = now_status;
        return;
    }
    Code now_ins = (*codes->getCodesVec())[status];
    cout << "----------------now_PC = " << PC << endl;
    //找到了当前PC
    latest_code = now_ins;
    int valP = codes->getNextPC(PC);
    string type = now_ins.getCodeType();
    if (type == "LET") {
        int exp_val;
        string exp1 = now_ins.getExp1();
        try {
            parseExpression(exp1, exp_val, variables, 1);
            variables[now_ins.getVariable()] = exp_val;
            auto iter=str_variables.find(now_ins.getVariable());
            if (iter != str_variables.end()) str_variables.erase(iter);
        }  catch (...) {
            string str_variable;
            parse_str_variable(exp1, return_status, str_variable);
            if (return_status == -1) {
                now_status = SINS;
                return_status = now_status;
                return;
            }
            else {
                str_variables[now_ins.getVariable()] = str_variable;
                auto iter=variables.find(now_ins.getVariable());
                if (iter != variables.end()) variables.erase(iter);
            }
        }

        now_PC = valP;
    }
    else if (type == "REM") {
        now_PC = valP;
    }
    else if (type == "END") {

        now_status = SFIN;
    }
    else if (type == "PRINT") {
        int exp_val;
        try {
            parseExpression(now_ins.getExp1(), exp_val, variables, 1);
        }  catch (...) {
            cout << "PRINT中表达式解析出错!" << endl;
            now_status = SINS;  //表达式解析出错的情况
            return_status = now_status;
            return;
        }
        latest_out = to_string(exp_val);
        now_PC = valP;
    }
    else if (type == "INPUT") {
        now_status = SHLT;
        now_PC = valP;
        return_status = now_status;
        return;
    }
    else if (type == "IF") {
        int exp1_val, exp2_val;
        string compare_symbol = now_ins.getCompareSymbol();
        int valE = now_ins.getNextPC();
        try {
            parseExpression(now_ins.getExp1(), exp1_val, variables, 1);
            parseExpression(now_ins.getExp2(), exp2_val, variables, 1);
        }  catch (...) {
            cout << "IF中表达式解析出错!" << endl;
            now_status = SINS;  //表达式解析出错的情况
            return_status = now_status;
            return;
        }


        if (       (compare_symbol == ">" && exp1_val > exp2_val)
                || (compare_symbol == "<" && exp1_val < exp2_val)
                || (compare_symbol == "=" && exp1_val == exp2_val)) {
            now_PC = valE;
        }
        else {
            now_PC = valP;
        }
    }
    else if (type == "GOTO") {
        int valE = now_ins.getNextPC();
        now_PC = valE;
    }
    else if (type == "PRINTF") {
        vector<string> &structured_strings = *now_ins.getStructuredStrings();
        int len = structured_strings.size();

        if (len == 0) {
            now_status = SINS;
            return_status = now_status;
            return;
        }
        string basic_string = structured_strings[0];
        char basic_str[MAXLENGTH];
        strcpy(basic_str, basic_string.c_str());
        char *head = basic_str;
        char *tail_tmp = basic_str + strlen(basic_str) - 1;
        int status;
        head = parse_space(head, status);
        tail_tmp = reverse_parse_space(tail_tmp, status);
        tail_tmp[1] = '\0';
        if (head[0] != tail_tmp[0]) {
            now_status = SINS;
            return_status = now_status;
            return;
        }
        if (head[0] == '\'' || head[0] == '\"') {
            head++;
            tail_tmp[0] = '\0';
        }
        else {
            now_status = SINS;
            return_status = now_status;
            return;
        }


        string result = "";
        int cnt = 1;
        while(head[0] != '\0') {
            if (head[0] != '{' && head[0] != '}') {
                result += head[0];
                head++;
            }
            else if (head[0] == '{') {
                while (head[0] != '}') {
                    head++;
                }
                char local_str[MAXLENGTH];
                strcpy(local_str, structured_strings[cnt].c_str());
                local_str[structured_strings[cnt].size()] = '\0';
                if (isDigit(local_str[0])) {
                    int parsed_number;
                    parse_number(local_str, parsed_number);
                    result += " " + to_string(parsed_number);
                }
                else if (isQuotationMark(local_str[0])) {
                    int len = strlen(local_str);
                    for (int i = len - 1; i >= 0; --i) {
                        if (isQuotationMark(local_str[i])) {
                            local_str[i] = '\0';
                            break;
                        }
                    }
                    char *head = local_str + 1;
                    result += " " + string(head);
                }
                else {
                    try {
                        int exp_val;
                        parseExpression(local_str, exp_val, variables, 1);
                        result += " " + to_string(exp_val);
                    }  catch (...) {
                        char* head = local_str,* tail = local_str + strlen(local_str) - 1;
                        int status;
                        head = parse_space(head, status);
                        tail = reverse_parse_space(tail, status);
                        tail[1] = '\0';
                        string str_variable(head);
                        if (str_variables.find(str_variable) != str_variables.end()) {
                            char tmp_str[MAXLENGTH];
                            strcpy(tmp_str, str_variables[str_variable].c_str());
                            tmp_str[strlen(tmp_str) - 1] = '\0';
                            result += " " + string(tmp_str + 1);
                        }
                        else {
                            now_status = SINS;
                            return_status = now_status;
                            return;
                        }
                    }
                }
                head++;
                cnt++;
            }
        }
        cout << "result = " << result << endl;
        latest_out = result;
        now_PC = valP;
    }
    else {
        now_status = SINS;
    }
    return_status = now_status;
}

string Sim::showVariables() {
    string result = "\n";
    for(auto iter = variables.rbegin(); iter != variables.rend(); iter++){
        result += iter->first + ": INT = " + to_string(iter->second) + '\n';
    }
    for(auto iter = str_variables.rbegin(); iter != str_variables.rend(); iter++){
        result += iter->first + ": STR = " + iter->second + '\n';
    }
    return result;
}
