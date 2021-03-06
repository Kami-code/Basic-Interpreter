# Basic Interpreter
## 一、项目介绍

	这是一个Basic语言解释器。

## 二、系统设计

### 1.内核类

本软件主要分为界面类、表达式树类和通用功能函数接口。

 

#### (1)表达式树类

```
	class tree_node{
    public:
    string operator_string = "pureValue";
    string variable_name = "";
    int value_1 = 0;
    int value_2 = 0;
    int level = 1;
    int ans = 0;
    tree_node* left_tree = nullptr, *right_tree = nullptr;
	}
```
	表达式树类的定义如上所示，定义了左值和右值、操作符、左子树和右子树。如果一个节点不由任何运算派生，则运算符设置为pureValue。

### 2.图形界面

图形界面提供了Basic解释器的图形化实现，包括代码、运行结果、语句和语法树以及命令载入窗口。

![image-20210413193138093](Basic-Interpreter.assets/image-20210413193138093.png)

## 三、程序逻辑

程序启动后，可以选择从硬盘中载入代码、或者从命令载入窗口手动加载代码。如果代码中有序号，则加入到代码区等待执行。如果代码中不含序号，则直接将执行保存到程序内存中并将对应的结果显示在运行结果中。

![image-20210413193105381](Basic-Interpreter.assets/image-20210413193105381.png)

图 1 代码执行结果

在代码执行环节中，程序首先逐行读入指令进行语法初筛，通常只要符合<代码序号> + <命令>的代码都会被正确输入到代码段中。
点击单步执行/运行后，程序先会为每行代码创建语法树，如果不成功，则会把对应行标红，语法树显示Invaild.
语法解析完成后，程序会开始模拟Basic执行，绿色为下一行待执行指令。期间，如果存在INPUT和INPUTS指令。程序模拟执行会等待命令行输入合法的表达式后，再继续模拟执行。如果执行到了一个语法树解析失败的指令、语法树解析成功，但是引用了未定义的变量、字符串格式错误（字符串中间存在多余的引号等），会有提示语句执行失败。会退出单步执行/运行模式。
