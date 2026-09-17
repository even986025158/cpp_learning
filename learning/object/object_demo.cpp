//
// Created by even on 2022/3/7.
//

#include <iostream>

/**
 *  new语法:
 *  new 类(构造方法列表) //返回对象指针
 *
 *  引用:
 *  作用:给变量起别名
 *  语法: 数据类型 &别名 = 原名
 *
 *  注: 引用必须初始化，且初始化后不可改变
 *  引用的本质是指针常量,而且使用时会自动解引用
 *  int & ref = a;
 *  => int * const ref = &a;
 */

using namespace std;

int * func(){
    int * p = new int(10); //在堆中开辟内存，存储数据，返回对象指针；
    int * arr = new int[10];
    return p;
}


int main(){

    int * p= func();
    cout << *p << endl; //10
    cout << *p << endl; //10;堆内存不会自动释放，需要用户自己手动释放
    delete p ;  //通过delete 释放堆内存
    cout << *p << endl; //1033977904

    //别名
    int a = 10;
    int &b = a;
    cout << a << endl; //10
    b = 20;  //改b就是改a
    cout << a << endl; //20



}