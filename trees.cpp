#include<iostream>
#include<cstring>
#include<stack>
#include<queue>
using namespace std;



class tree_node{
        public:
        string operator_string = "pureValue";
        int value_1 = 0;
        int value_2 = 0;
        int level = 1;
        int ans = 0;
        tree_node* left_tree = nullptr, *right_tree = nullptr;
        tree_node() {};
        tree_node(int number):operator_string("pureValue"), value_1(number), value_2(0), ans(number), left_tree(nullptr), right_tree(nullptr) {
        };
        tree_node(string op,int v1, int v2, int a, tree_node* l, tree_node* r):operator_string(op), value_1(v1), value_2(v2), ans(a), left_tree(l), right_tree(r) {

                cout << "tree created! " << endl;
                cout << *this << endl;
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

class tree_subnode{
        public:
        string saver;
        int level = 1;
        tree_subnode* left = nullptr, *right = nullptr;
        tree_subnode() {};
        tree_subnode(string s, tree_subnode* l = nullptr, tree_subnode* r = nullptr):saver(s), left(l), right(r) {};
};

tree_subnode* subnodeSplit(tree_node* local_root) {
        if (local_root->operator_string == "pureValue") {
                tree_subnode* leave = new tree_subnode(to_string(local_root->ans));
                return leave;
        }
        else {

                //tree_subnode* local_subnode_left = new tree_subnode(to_string(local_root->value_1), subnodeSplit(local_root->left_tree));
                //tree_subnode* local_subnode_right = new tree_subnode(to_string(local_root->value_2), subnodeSplit(local_root->right_tree));
                tree_subnode* local_subnode_root = new tree_subnode(local_root->operator_string, subnodeSplit(local_root->left_tree), subnodeSplit(local_root->right_tree));
                return local_subnode_root;
        }
}


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

void inorderTraversal (tree_node* root) {
        queue<tree_node*> helper_queue;
        helper_queue.push(root);
        int now_level = 1;
        int discount = 0;
        while(helper_queue.empty() != true) {
                tree_node* local_tree_node = helper_queue.front(); helper_queue.pop();
                if (local_tree_node == nullptr) continue; //到叶子结点了，返回
                if (local_tree_node->level > now_level) { //开始输出下一层的树，换行
                        now_level++;
                        cout << endl;
                }

                cout << local_tree_node->operator_string << " "; //输出本层的一棵树
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

void subtree_print(tree_subnode* subroot) {
        queue<tree_subnode*> helper_queue;
        helper_queue.push(subroot);
        int now_level = 1;
        int discount = 0;
        while(helper_queue.empty() != true) {
                tree_subnode* local_tree_node = helper_queue.front(); helper_queue.pop();
                if (local_tree_node == nullptr) continue; //到叶子结点了，返回
                if (local_tree_node->level > now_level) { //开始输出下一层的树，换行
                        now_level++;
                        cout << endl;

                }
                for (int i = 0; i < now_level; ++i) cout << " ";
                cout << local_tree_node->saver << endl; //输出本层的一棵树
                if (local_tree_node->left != nullptr) {
                        helper_queue.push(local_tree_node->left); //下一层的左右子树入队
                        local_tree_node->left->level = local_tree_node->level + 1;
                }
                if (local_tree_node->right != nullptr) {
                        helper_queue.push(local_tree_node->right);
                        local_tree_node->right->level = local_tree_node->level + 1;
                }

        }
}

stack <string> operatorStack;
stack <tree_node*> valueStack;

/*
int main () {
        string s = "332**(2)/2-9";
        for (int i = 0; i < s.size(); ++i) {

                if (s[i] >= '0' && s[i] <= '9') {
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
                        cout << "read a number " << now_tree_node->ans << endl;
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
                                while (operatorStack.empty() == false && getPriority(operatorStack.top()) > priority) { //栈不空且栈顶元素的优先级大于你，那么这个操作符先等着，先做栈里的操作
                                        dosomething(operatorStack, valueStack);
                                }
                                if (priority == 4) operatorStack.push(s.substr(i++, 2));
                                else operatorStack.push(s.substr(i, 1));
                        }
                }
                print_stack(operatorStack);
                //print_stack(valueStack);
                cout << endl;
        }
        while (operatorStack.empty() == false) { //栈不空就做栈里的操作
                dosomething(operatorStack, valueStack);
                print_stack(operatorStack);
                print_stack(valueStack);
                cout << endl;
        }
        tree_node* root = valueStack.top();
        inorderTraversal(root);
        cout << endl;
        tree_subnode* subroot = subnodeSplit(root);
        subtree_print(subroot);
        return 0;
}
*/
