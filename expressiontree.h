#ifndef EXPRESSIONTREE_H
#define EXPRESSIONTREE_H
#include<iostream>
#include<cstring>
#include<stack>
#include<queue>
#include<map>
using namespace std;


class tree_node{
    public:
    string operator_string = "pureValue";
    string variable_name = "";
    int value_1 = 0;
    int value_2 = 0;
    int level = 1;
    int ans = 0;
    tree_node* left_tree = nullptr, *right_tree = nullptr;
    tree_node() {};
    tree_node(int number):operator_string("pureValue"), value_1(number), value_2(0), ans(number), left_tree(nullptr), right_tree(nullptr) {
    };
    tree_node(string string_name,int number):operator_string("variable"),variable_name(string_name), value_1(number), value_2(0), ans(number), left_tree(nullptr), right_tree(nullptr) {
    };
    tree_node(string op,int v1, int v2, int a, tree_node* l, tree_node* r):operator_string(op), value_1(v1), value_2(v2), ans(a), left_tree(l), right_tree(r) {

        //cout << "tree created! " << endl;
        //cout << *this << endl;
    }
    friend ostream & operator<<( ostream & os,const tree_node & t){
        if (t.operator_string == "pureValue") {
            os << "  " << t.operator_string << endl << "null        null" << endl;
            return os;
        }
        os << "  " << t.operator_string << endl << (t.left_tree==nullptr?-999:t.left_tree->ans) << "        " << (t.right_tree==nullptr?-999:t.right_tree->ans)  << endl;
        return os;
    }

};

string parseExpression (string, int&, map<string, int>&, int);

#endif // EXPRESSIONTREE_H
