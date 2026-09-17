//
// Created by even on 2022/3/1.
//
#include <iostream>

/**
 * 字符串:
 * 语法:
 * C语言风格: char 变量名[] = "字符串值";
 * C++语言风格: string 变量名 = "字符串值";
 */

int main(){
    using namespace std; //string类型属于std

    char chs[] = "hello world";
    cout << chs << endl;
    cout << "chs 占用内存大小" << sizeof(chs) << endl; //12

    string str = "hello world";
    cout << str << endl;
    cout << "str 占用内存大小" << sizeof(str) << endl; //24

}

