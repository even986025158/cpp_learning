//
// Created by even on 2022/3/1.
//
#include <iostream>

/**
 * 数据类型: 实际就是分配内存空间用的
 * 整型:
 * short: 2字节
 * int: 4字节
 * long:windows 4字节; linux (32位)4字节、(64位)8字节
 * long long : 8字节
 *
 * 浮点型:
 * float : 4字节 ； float 变量名 = 数值f;
 * double : 8字节
 *
 * 字符型:
 * char : 1字节 ；char 变量名 = '单字符'
 *
 * 布尔类型:
 * bool : 1字节; true为1【非0代表真】，false为0
 */

/**
 * sizeof 关键字: 统计类型所占内存大小
 * 语法: sizeof(数据类型 或者 变量名/常量名 或者 表达式)
 * @return
 */
#define A 10
#define B "宏"
#define C 'c'
int main(){
    using namespace std;
    cout << "short 占用内存大小" << sizeof(short) << endl; //2
    cout << "int 占用内存大小" << sizeof(int) << endl; //4
    cout << "long 占用内存大小" << sizeof(long) << endl; //8
    cout << "long long 占用内存大小" << sizeof(long long) << endl; //8
    cout << "A 占用内存大小" << sizeof(A) << endl; //4
    cout << "B 占用内存大小" << sizeof(B) << endl; //4
    cout << "B 占用内存大小" << sizeof(C) << endl; //1
    cout << "c : " << C+1 << endl; //100
    cout << "C+1 占用内存大小" << sizeof(C+1) << endl; //4
    cout << "\t 占用内存大小" << sizeof('\t') << endl; //1

    float pif = 3.14f;
    double pi = 3.14;
    cout << "float 占用内存大小" << sizeof(pif) << endl; //4
    cout << "double 占用内存大小" << sizeof(pi) << endl; //8

    bool flag = true;
    bool error = false;
    cout << "true : " << flag << endl; // 1
    cout << "false : " << error << endl; // 0
    cout << "flag is true " << (true == flag) << endl; // 1
    cout << "flag is 1 " << (1 == flag) << endl; // 1
    cout << "flag is 100 " << (100 == flag) << endl; // 0
    cout << "false is 1 " << (1 == error) << endl; // 0
}

