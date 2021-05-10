#include "code.h"
#include "utils.h"


Code::Code(){
    code_number = -1;
    code_type = "";
    code_string = "";
    grammer = "undefined";
    exp1 = "";
    exp2 = "";
    comment = "";
    variable = "";
    next_PC = -1;
    structured_strings.clear();
}



Code::Code(string s){   //����ʱ�����ַ��������Զ������ַ��������﷨��
    cout << "���ڹ���Code:" << s << endl;
    code_string = s;
    char code_str[MAXLENGTH];
    strcpy(code_str, code_string.c_str());  //���ƽ�code_str
    char* header = code_str;
    int return_status = 0;
    cout << "------------" << header << "-------------" << endl;
    header = parse_number(header, code_number);
    if (code_number == -1) {
        //û�ж�����ŵ������������������ֱ�������print��let
    }
    header = parse_string(header, return_status, code_type);
    if (code_type == "LET") {
        //let���������ʽΪ variable + equal + expression
        int equal_symbol_parsed = 0;
        header = parse_string(header, return_status, variable);
        header = parse_equal(header, equal_symbol_parsed);
        header = parse_expression(header, return_status, exp1);
        if (1) {
            //... û�ж����Ⱥš����������ʽ�����
        }
        cout << "variable: " << variable << endl;
        cout << "exp1: " << exp1 << endl;
    }
    else if (code_type == "PRINT") {
        //print���������ʽΪ expression
        header = parse_expression(header, return_status, exp1);
        if (1) {
            //... û�ж������ʽ�����
        }
        cout << "exp1: " << exp1 << endl;
    }
    else if (code_type == "PRINTF") {
        while (header[0] != '\0') {
            int return_status = 0;
            header = parse_space(header, return_status);
            char *header1 = parse_comma(header, return_status);
            char n_str[MAXLENGTH];
            strncpy(n_str, header, return_status);
            if (return_status == 0) break;
            n_str[return_status] = '\0';
            cout << "pushed string = " << string(n_str) << endl;
            structured_strings.push_back(string(n_str));
//            cout << "n_str = " << n_str << endl;
            header = header1 + 1;
        }
        for (size_t i = 0; i < structured_strings.size(); ++i) {
            cout <<  structured_strings[i] << endl;
        }

    }
    else if (code_type == "INPUT") {
        //input���������ʽΪ variable
        header = parse_string(header, return_status, variable);
        if (1) {
            //... û�ж������������
        }
    }
    else if (code_type == "INPUTS") {
        //input���������ʽΪ variable
        header = parse_string(header, return_status, variable);
        if (1) {
            //... û�ж������������
        }
    }
    else if (code_type == "GOTO") {
        header = parse_number(header, next_PC);
        if (next_PC == -1) {
            //..û�ж����кŵ����
        }
    }
    else if (code_type == "IF") {
        //��ʽΪ exp1 compare_symbol exp2 THEN number;
        string then_symbol;
        header = parse_expression(header, return_status, exp1);
        header = parse_compare_symbol(header, return_status, compare_symbol);
        //Ψһ�أ���Ҫ�ڴ˴���һ�����⴦����α�֤exp2�в������THEN�ͺ����number�أ�
        //�˴����õ��Ŷ�ȡһ��number��һ��string�ķ����������Ƚ�����
        int len = strlen(header);
        char* tail = header + len - 1;  //�õ����һ���ַ�������
        tail = reverse_parse_number(tail, next_PC);
        tail = reverse_parse_string(tail, return_status, then_symbol);
        if (next_PC == -1 || then_symbol != "THEN") {
            //�쳣����
        }
        tail[1] = '\0';
        header = parse_expression(header, return_status, exp2);
        cout << "then_symbol: " << then_symbol << endl;
        cout << "possible_next_PC: " << next_PC << endl;
        cout << "exp1: " << exp1 << endl;
        cout << "exp2: " << exp2 << endl;

    }
    else if (code_type == "END") {
        //��ʽΪEND
    }
    else if (code_type == "REM") {
        parse_line(header, return_status, comment);
    }
    cout << "code_number: " << code_number << endl;
    cout << "type: " << code_type << endl;
    cout << "----------------------------------" << endl;
    createGrammerTree();
}

void Code::createGrammerTree() {
    grammer = "invaild";
    int return_status = 0;
    map<string, int> placeholder;
    if (code_type == "IF") {
        try {
            string grammer_string_1 = parseExpression(exp1, return_status, placeholder, 0);
            string grammer_string_2 = parseExpression(exp2, return_status, placeholder, 0);
            grammer =
                    to_string(code_number) + " IF THEN\n"
                    + grammer_string_1 + '\n'
                    + "    " + compare_symbol + '\n'
                    + grammer_string_2 + '\n'
                    + "    " + to_string(next_PC) + '\n';
        }  catch (...) {

        }

    }
    else if (code_type == "REM") {
        grammer =
                to_string(code_number) + " REM\n"
                + "    " + comment;
    }
    else if (code_type == "LET") {
        try {
            string grammer_string_1 = parseExpression(exp1, return_status, placeholder, 0);
            grammer =
                    to_string(code_number) + " LET =\n"
                    + "    " + variable + '\n'
                    + grammer_string_1 + '\n';
        }  catch (...) {
            string str_variable;
            parse_str_variable(exp1, return_status, str_variable);
            if (return_status == -1) {
                return;
            }
            else {
                grammer =
                        to_string(code_number) + " LET =\n"
                        + "    " + str_variable + "\n";
            }
        }
    }
    else if (code_type == "GOTO") {
        grammer =
                to_string(code_number) + " GOTO\n"
                + "    " + to_string(next_PC) + '\n';
    }
    else if (code_type == "INPUT") {
        grammer =
                to_string(code_number) + " INPUT\n"
                + "    " + variable + "\n";
    }
    else if (code_type == "INPUTS") {
        grammer =
                to_string(code_number) + " INPUTS\n"
                + "    " + variable + "\n";
    }
    else if (code_type == "END") {
        grammer =
                to_string(code_number) + " END\n";
    }
    else if (code_type == "PRINT") {
        try {
            string grammer_string_1 = parseExpression(exp1, return_status, placeholder, 0);
            grammer =
                    to_string(code_number) + " PRINT\n"
                    + grammer_string_1 + '\n';
        }  catch (...) {

        }
    }
    else if (code_type == "PRINTF") {
        grammer =
                to_string(code_number) + " PRINTF\n";
        for (size_t i = 0; i < structured_strings.size(); ++i) {
            grammer += + "    " + structured_strings[i] + '\n';
        }
    }

}


Codes::Codes() {
    codes_vec.clear();
}

void Codes::uniquePush(Code code) {
    for (auto i = codes_vec.begin(); i != codes_vec.end(); ++i) { //���ڵ����
        if (code.getCodeNumber() == (*i).getCodeNumber() && code.getCodeType() != "") {
            (*i) = code;
            return;
        }
        else if (code.getCodeNumber() == (*i).getCodeNumber() && code.getCodeType() == "") {
            codes_vec.erase(i); //ɾ����������
            return;
        }
        else continue;
    }
    if (code.getCodeType() != "") codes_vec.push_back(code);
    sort(codes_vec.begin(), codes_vec.end());
    while(codes_hightlight_index.empty() != true) codes_hightlight_index.pop_back();
//    codes_hightlight_index.clear();
    int cnt = 0;
    for (size_t i = 0; i < codes_vec.size(); ++i) {
        int len = codes_vec[i].getCodeString().size();
        cnt += len;
        codes_hightlight_index.push_back(cnt);
        cout << cnt << " ";
    }
    cout << endl;
    return;
}

void Codes::showCodes() {
    cout << endl << "-----------showCodes-------------" << endl;
    for (auto i = codes_vec.begin(); i != codes_vec.end(); ++i) {
        cout << (*i).getCodeString() << endl;
    }
    cout << "----------------------------------" << endl;
}

void Codes::showGrammers() {
    cout << endl << "-----------showGrammers-------------" << endl;
    for (auto i = codes_vec.begin(); i != codes_vec.end(); ++i) {
        cout << (*i).getGrammer() << endl;
    }
    cout << "----------------------------------" << endl;
}

int Codes::searchByPC(int PC) {
    for (size_t i = 0; i < codes_vec.size(); ++i) {
        if (codes_vec[i].getCodeNumber() == PC)
            return i;
    }
    return -1;       //û�ҵ������
}

int Codes::getNextPC(int PC) {
    for (auto i = codes_vec.begin(); i != codes_vec.end(); ++i) {
        if ((*i).getCodeNumber() > PC) {
            return (*i).getCodeNumber();   //�����ҵ��˳������
        }
    }
    return -1;
}

int Codes::getFirstPC() {
    for (auto i = codes_vec.begin(); i != codes_vec.end(); ++i) {
        return (*i).getCodeNumber();
    }
    return -1;
}

vector<Code>* Codes::getCodesVec() {
    return &codes_vec;
}

int Codes::getHightlightByIndex(int index) {
    if (index < 0 || index > codes_hightlight_index.size() - 1) return -1;
    return codes_hightlight_index[index];
}
