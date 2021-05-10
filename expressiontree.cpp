#include "expressiontree.h"
using namespace std;





int getPriority(string a) {
    if (a == "(") return 1;
    if (a == "+" || a == "-") return 2;
    if (a == "*" || a == "/") return 3;
    if (a == "**") return 4;
    throw("invalid symbol!!");
}

int getPriority(string s, int index) {
    if (index <= s.size() - 1 && s.substr(index, 2) == "**") return getPriority(s.substr(index, 2));
    else return getPriority(s.substr(index, 1));
}

void print_stack(stack <tree_node* > a) {
    tree_node* val;
    stack <tree_node*> temp;
    while(!a.empty())
    {
        val = a.top();
        temp.push(val);
        a.pop();
    }
    //复原
    while(!temp.empty())
    {
        val = temp.top();
        temp.pop();
        a.push(val);
        cout << *val << " ";
    }
    cout << endl;
}

void print_stack(stack <string> a) {
    string val;
    stack <string> temp;
    while(!a.empty())
    {
        val = a.top();
        temp.push(val);
        a.pop();
    }
    //复原
    while(!temp.empty())
    {
        val = temp.top();
        temp.pop();
        a.push(val);
        cout << val << " ";
    }
    cout << endl;
}


void dosomething(stack<string> &operatorStack, stack<tree_node*> &valueStack) { /*这是从操作符栈中取一个操作符，从操作数栈中取两个操作数*/
    string operator_top =  operatorStack.top(); operatorStack.pop();
    tree_node* node_second = valueStack.top(); valueStack.pop();
    tree_node* node_first = valueStack.top(); valueStack.pop();
    int val_second = node_second->ans;
    int val_first = node_first->ans;
    int ans = 0;
    if (operator_top == "+") ans = val_first + val_second;
    else if (operator_top == "-") ans = val_first - val_second;
    else if (operator_top == "*") ans = val_first * val_second;
    else if (operator_top == "/") ans = val_first / val_second;
    else if (operator_top == "**") {
        ans = 1;
        for (int i = 0; i < val_second; ++i) ans *= val_first;
    }
    tree_node* now_point = new tree_node(operator_top, val_first, val_second, ans, node_first, node_second);
    valueStack.push(now_point);
    //cout << "I did " << val_first << " " << operator_top << " " << val_second << " and get " << valueStack.top() << endl;
}

string ans_string = "";

void level_print(string operator_string, int level) {
    for (int i = 0; i < level; ++i) {
        ans_string.append("    ");
    }
    ans_string.append(operator_string);
    ans_string.append("\n");
}

void trace(tree_node* root) {
    if (root == nullptr) return;
    //先push root,拿出来打印符号-, push左子树，
    if (root->operator_string == "pureValue") {
        level_print(to_string(root->value_1), root->level);
    }
    else if (root->operator_string == "variable") {
        level_print(root->variable_name, root->level);
    }
    else level_print(root->operator_string, root->level);
    trace(root->left_tree);
    trace(root->right_tree);
}

void inorderTraversal (tree_node* root) { //set level
    queue<tree_node*> helper_queue;
    helper_queue.push(root);
    int now_level = 1;
    int discount = 0;
    while(helper_queue.empty() != true) {
        tree_node* local_tree_node = helper_queue.front(); helper_queue.pop();
        if (local_tree_node == nullptr) continue; //到叶子结点了，返回
        if (local_tree_node->level > now_level) { //开始输出下一层的树，换行
            now_level++;
//            cout << endl;
        }

//        cout << local_tree_node->operator_string << " "; //输出本层的一棵树
        if (local_tree_node->left_tree != nullptr) {
            helper_queue.push(local_tree_node->left_tree); //下一层的左右子树入队
            local_tree_node->left_tree->level = local_tree_node->level + 1;
        }
        if (local_tree_node->right_tree != nullptr) {
            helper_queue.push(local_tree_node->right_tree);
            local_tree_node->right_tree->level = local_tree_node->level + 1;
        }

    }
}


stack <string> operatorStack;
stack <tree_node*> valueStack;

string parseExpression (string s, int& ans, map<string, int>& variables, int flag = 1) {
    while(valueStack.empty() != true) valueStack.pop();
    while(operatorStack.empty() != true) operatorStack.pop();
    //flag = 1，同时进行
    //flag = 0，单纯生成表达式树，ans无效。
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] == ' ') continue;  //跳过空格
        string local_variable = "";
        bool in_this = false;
        if (s[i] >= 'a' && s[i] <= 'z' || s[i] >='A' && s[i] <= 'Z') {
            local_variable += s.substr(i, 1);
            i++;
            in_this = true;
            while (s[i] >= 'a' && s[i] <= 'z' || s[i] >='A' && s[i] <= 'Z' || s[i] >= '0' && s[i] <= '9') {
                local_variable += s.substr(i, 1);
                i++;
            }
        }
        i -= in_this;

        if (local_variable != "") {
            if (variables.find(local_variable) == variables.end()) {
                if (flag == 1) {
                    throw("variable not find!");
                }
                else {
                    tree_node *now_tree_node = new tree_node(local_variable, 0);
//                    cout << "local_variable = " << local_variable << " value = " << variables[local_variable] << endl;
                    valueStack.push(now_tree_node);
                }
            }
            else {
                tree_node *now_tree_node = new tree_node(local_variable, variables[local_variable]);
//                cout << "local_variable = " << local_variable << " value = " << variables[local_variable] << endl;
                valueStack.push(now_tree_node);
            }
        }
        else if (s[i] >= '0' && s[i] <= '9') {
            int number_count = 0;
            int origin_i = i;
            while (s[i] >= '0' && s[i] <= '9') {
                number_count++;
                i++;
            }
            i--;
            string now = s.substr(origin_i, number_count);
            tree_node *now_tree_node = new tree_node(stoi(now));
            valueStack.push(now_tree_node);
        }
        else {
            //s[i]是操作符的情况
            //如果为空，或者栈顶的优先级小于等于自己，那么可以直接推入
            if (s[i] == '(') {
                operatorStack.push(s.substr(i, 1));
            }
            else if (s[i] == ')') {
                while (operatorStack.top() != "(") {
                    dosomething(operatorStack, valueStack);
                }
                operatorStack.pop();
            }
            else  {
                int priority = getPriority(s, i);
                while (operatorStack.empty() == false && getPriority(operatorStack.top()) >= priority) { //栈不空且栈顶元素的优先级大于你，那么这个操作符先等着，先做栈里的操作
                    dosomething(operatorStack, valueStack);
                }
                if (priority == 4) operatorStack.push(s.substr(i++, 2));
                else operatorStack.push(s.substr(i, 1));
            }
        }
        //cout << endl;
    }
    while (operatorStack.empty() == false) { //栈不空就做栈里的操作
        dosomething(operatorStack, valueStack);
    }
    if (valueStack.empty() == true) throw("error");
    if (valueStack.size() != 1) throw("error");
    ans = valueStack.top()->ans;
    //cout << valueStack.top()->ans << endl;
    tree_node* root = valueStack.top();
    inorderTraversal(root);
    ans_string = "";
    trace(root);

    return ans_string.substr(0,ans_string.length() - 1);
}
