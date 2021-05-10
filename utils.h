#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <stdio.h>
#include <string.h>
using namespace std;

bool isLetter(char);
bool isDigit(char);
bool isSpace(char);
bool isOperator(char);
bool isQuotationMark(char);
bool isConditionalOperator(char);
bool isBrace(char);

char* parse_space(char*, int &);
char* parse_equal(char*, int &);
char* parse_expression(char*, int &, string &);
char* parse_number(char*, int &);
char* parse_string(char*, int &, string &);
char* parse_line(char*, int &, string &);
char* parse_compare_symbol(char*, int &, string &);
char* reverse_parse_space(char*, int &);
char* reverse_parse_number(char*, int &);
char* reverse_parse_string(char*, int &, string &);
void parse_str_variable(string, int &, string &);
char* parse_quotation_mark(char* , int &);
char* parse_comma(char* , int &);

#endif // UTILS_H
