#include "utils.h"
#include <iostream>
using namespace std;

#define MAXLENGTH 100

bool isLetter(char local) {
    if ((local >= 'a' && local <= 'z') || (local >= 'A' && local <= 'Z')) return true;
    else return false;
}

bool isDigit(char local) {
    if (local >= '0' && local <= '9') return true;
    else return false;
}

bool isSpace(char local) {
    return local == ' '? true : false;
}

bool isOperator(char local) {
    if (local == '+' || local == '-' || local == '*' || local == '/') return true;
    else return false;
}

bool isConditionalOperator(char local) {
    if (local == '=' || local == '<' || local == '>') return true;
    else return false;
}

bool isQuotationMark(char local) {
    if (local == '\"' || local == '\'') return true;
    else return false;
}

bool isBrace(char local) {
    if (local == '(' || local == ')') return true;
    else return false;
}

char* parse_space(char* char_head, int &return_status) {
    return_status = 0;  //这个返回值是消去了多少个空格
    while(char_head[0] == 32) {
        char_head++;
        return_status++;
    }
    return char_head;
}

char* parse_equal(char* char_head, int &return_status) {
    char_head = parse_space(char_head, return_status);
    return_status = 0;  //这个返回值是读取是否读取了等号
    if(char_head[0] == '=') {
        char_head++;
        return_status = 1;
    }
    return char_head;
}

char* parse_number(char* char_head, int &return_status) {
    char_head = parse_space(char_head, return_status);
    return_status = 0;
    while (isDigit(char_head[0])) {
        return_status *= 10;
        return_status += (char_head[0] - '0');
        char_head++;
    }
    return char_head;
}

char* parse_quotation_mark(char* char_head, int &return_status) {
    char_head = parse_space(char_head, return_status);
    while(char_head[0] != '\0') {
        if (char_head[0] == '\'') {
             return char_head;
        }
        else if (char_head[0] == '\"') {
            return char_head;
        }
        char_head++;
        return_status++;
    }
    return_status = -1;
    return char_head;
}

char* parse_comma(char* char_head, int &return_status) {
    char_head = parse_space(char_head, return_status);

    while(char_head[0] != '\0') {
        if (char_head[0] == ',') {
             return char_head;
        }
        char_head++;
        return_status++;
    }
    return char_head;
}

char* strict_parse_comma(char* char_head, int &return_status) {
    char_head = parse_space(char_head, return_status);
    if(char_head[0] != '\0') {
        if (char_head[0] == ',') {
            return_status = 1;
            return char_head;
        }
        else {
            return_status = 0;
            return char_head;
        }
    }
    else {
        return_status = 2;
        return char_head;
    }

}

char* parse_string(char* char_head, int &return_status, string &parsed_string) {
    char_head = parse_space(char_head, return_status);
    char* start_head = char_head;
    return_status = 0;
    while (isDigit(char_head[0]) || isLetter(char_head[0])) {
        return_status++;
        char_head++;
    }

    char parsed_string_str[MAXLENGTH];
    strncpy(parsed_string_str, start_head, return_status);
    parsed_string.assign(parsed_string_str, return_status);

    return char_head;
}

/*
    获得表达式:输入一个字符串，返回这个字符串的合法表达式子串。
    @Prarm head: 字符串头，如果检测成功，就向后移动一位。
    @Param return_status: 返回值，返回检测成功的长度
    @Param expression: 返回解析到的表达式子串

*/
char* parse_expression(char* head, int &return_status, string &expression) {
    return_status = 0;  //这个返回值是读取字符的数目
    char* start_head = head;
    while(isDigit(head[0]) || isLetter(head[0]) || isSpace(head[0]) || isOperator(head[0]) || isQuotationMark(head[0]) || isBrace(head[0])) {
        head++;
        return_status++;
    }

    char expression_str[MAXLENGTH];
    strncpy(expression_str, start_head, return_status);
    expression.assign(expression_str, return_status);

    return head;
}

char* parse_line(char* head, int &return_status, string &line) {
    return_status = 0;  //这个返回值是读取字符的数目
    char* start_head = head;
    while(head[0] != '\0' && head[0] != '\n') {
        head++;
        return_status++;
    }

    char line_str[MAXLENGTH];
    strncpy(line_str, start_head, return_status);
    line.assign(line_str, return_status);

    return head;
}


char* parse_compare_symbol(char* head, int &return_status, string &expression) {
    head = parse_space(head, return_status);
    return_status = 0;

    char* start_head = head;
    if (isConditionalOperator(head[0])) {
        head++;
        return_status = 1;
    }

    if (return_status == 0) {
        expression = "";
        return head;
    }
    char parsed_string_str[MAXLENGTH];
    strncpy(parsed_string_str, start_head, return_status);
    expression.assign(parsed_string_str, return_status);

    return head;
}

char* reverse_parse_space(char* tail, int &return_status) {
    return_status = 0;  //这个返回值是消去了多少个空格
    while(tail[0] == 32) {
        tail--;
        return_status++;
    }
    return tail;
}

char* reverse_parse_number(char* tail, int &return_status) {
    tail = parse_space(tail, return_status);
    return_status = 0;
    int count = 0;
    while (isDigit(tail[0])) {
        int now_digit = tail[0] - '0';
        for (int i = 0; i < count; ++i) {
            now_digit *= 10;
        }
        return_status += now_digit;
        count++;
        tail--;
    }
    return tail;
}

char* reverse_parse_string(char* tail, int &return_status, string &parsed_string) {
    tail = reverse_parse_space(tail, return_status);
    char* start_tail = tail;
    return_status = 0;
    while (isDigit(tail[0]) || isLetter(tail[0])) {
        return_status++;
        tail--;
    }
    if (return_status == 0) return start_tail;

    char parsed_string_str[MAXLENGTH];
    strncpy(parsed_string_str, tail + 1, return_status);
    parsed_string.assign(parsed_string_str, return_status);

    return tail;
}

void parse_str_variable(string exp, int &return_status, string &parsed_string) {
    char head[MAXLENGTH];
    strcpy(head, exp.c_str());
    char* now_head = head, *now_tail = head + exp.size() - 1;
    return_status = -1;
    now_head = parse_space(now_head, return_status);
    now_tail = reverse_parse_space(now_tail, return_status);
    cout << "now_head = " << now_head << endl;
    cout << "now_tail = " << now_tail << endl;
    now_tail[1] = '\0';
    if (now_tail <= now_head) {
        return;
    }
    if (       (now_head[0] == '\'' && now_tail[0] == '\'')
            || (now_head[0] == '\"' && now_tail[0] == '\"')) {
        for (char* i = now_head + 1; i < now_tail; i++) {
            if (i[0] == '\'' || i[0] == '\"') {
                return_status = -1;
                return;
            }
        }
        return_status = 0;
        parsed_string = now_head;
        return;
    }
    return_status = -1;
    return;
}
