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
        cout << "�������н�����" << endl;
        return;
    }
    else if (now_status == SINS) {
        cout << "�������й����г������⣡" << endl;
        return;
    }
    else if (now_status == SHLT) {
        cout << "������ͣ���ȴ��У�" << endl;
        return;
    }
    int PC = now_PC;

    int status = codes->searchByPC(PC);
    if (status == -1) {
        cout << "û���ҵ� PC =  " << PC << endl;
        now_status = SINS;  //û���ҵ���ǰPC�����
        return;
    }
    Code now_ins = (*codes->getCodesVec())[status];
    cout << "----------------now_PC = " << PC << endl;
    //�ҵ��˵�ǰPC
    latest_code = now_ins;
    int valP = codes->getNextPC(PC);
    string type = now_ins.getCodeType();
    if (type == "LET") {
        int exp_val;
        try {
            parseExpression(now_ins.getExp1(), exp_val, variables, 1);
        }  catch (...) {
            cout << "LET�б��ʽ��������!" << endl;
            now_status = SINS;  //���ʽ������������
            return;
        }
        variables[now_ins.getVariable()] = exp_val;
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
            cout << "PRINT�б��ʽ��������!" << endl;
            now_status = SINS;  //���ʽ������������
            return;
        }
        latest_out = to_string(exp_val);
        now_PC = valP;
    }
    else if (type == "INPUT") {
        now_status = SHLT;
        now_PC = valP;
    }
    else if (type == "IF") {
        int exp1_val, exp2_val;
        string compare_symbol = now_ins.getCompareSymbol();
        int valE = now_ins.getNextPC();
        try {
            parseExpression(now_ins.getExp1(), exp1_val, variables, 1);
            parseExpression(now_ins.getExp2(), exp2_val, variables, 1);
        }  catch (...) {
            cout << "IF�б��ʽ��������!" << endl;
            now_status = SINS;  //���ʽ������������
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
}

string Sim::showVariables() {
    string result = "\n";
    for(auto iter = variables.rbegin(); iter != variables.rend(); iter++){
        result += iter->first + ": INT = " + to_string(iter->second) + '\n';
    }
    return result;
}
