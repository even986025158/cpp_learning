//
// Created by even on 2022/3/4.
//
#include <iostream>

/**
 * 指针: 可以间接访问内存
 *
 * 定义: 数据类型 * 指针变量名
 * 表示指向该变量头地址，且包含数据类型长度的数据
 * 赋值:
 * 指针变量名 = &变量名 [表示指向该变量的地址]
 *
 * 使用:
 *  *指针变量名 : 表示解引用，可以获取指针指向内存的信息
 *
 * 指针大小: 32位4字节； 64位8字节
 *
 * 空指针: 指针指向内存编号为0的指针；
 * 用途:初始化指针变量
 * 注:空指针指向的内存不可访问【系统占用】
 *
 * 野指针: 即随意指向一个内存空间;如:
 * int * p = (int *)0x1100;
 *
 * const 修饰指针
 * 1. const修饰指针: -- 常量指针 : const 数据类型 * 指针变量名 : 不能通过指针变量来修改对应的值;  *指针变量名 = ... 错误的; 相当于const修饰 *，所以*指针变量名不能修改
 * 2. const修饰常量: -- 指针常量 : 数据类型 * const 指针变量名 : 不能修改指针指向的地址;  指针变量名 = &... 错误的;相当于const修饰指针变量，所以指针变量的值不能修改
 * 3. const即修饰指针又修饰常量: const 数据类型 * const 指针变量名 :即不能修改指针指向的地址也不能修改指针变量的值
 *
 * 指针访问数组:
 *  int * pArr = arr; 表示指向数组头地址
 *  pArr++ : 表示向后偏移一个数据单位，即取数组下一个元素
 */

using namespace std;

int main(){

    int a = 10;
    int * p;
    p = &a;
    cout << "a的地址: " << p << endl; //0x7ff7b1aaf88c
    cout << "p指向内存的值: " << *p << endl; //10

//    p = 1000; //类型不匹配
    *p = 1000;
    cout << "a: " << *p << endl; //1000

    cout << "指针占用内存大小: " << sizeof(p) << endl; // 8

    //空指针:
    int * nullP =NULL;
    //空指针指向的内存不可访问 【系统占用: 0~255】
//    *nullP = 100; //报错:退出代码139 (interrupted by signal 11: SIGSEGV)

    //野指针:随意指向，不知道会出现什么情况，大概率会没有权限访问
    int * unknownP = (int *)0x1100;
//    cout << "unknownP指向内存的值: " << *unknownP << endl; //报错:退出代码139 (interrupted by signal 11: SIGSEGV)


    //const指针
    int b = 10;
    //1. const修饰: 指针的指向可以修改，但是指针指向的值不可以修改;
    const int * constP = &a; //可以const int * constP;
    a = 20;
    cout << "a: " << *constP << endl; //20
//    *constP = 20; //编译错误; 即不能通过指针进行值的修改，但是如果有变量指向该内存，则可以通过该变量进行修改
    constP = &b; //可以修改指向的指向

    //2. 指针常量: 指针的指向不可以修改，但是指针指向的值可以修改;
    int * const pConst = &a; //int * const pConst;会报错，说明必须要赋值
    *pConst = 100;
    cout << "a: " << *pConst << endl; //100
//    pConst = &b; //编译错误;不可以修改指针指向的地址

    //3.const即修饰指针又修饰常量:即不能修改指针指向的地址也不能修改指针变量的值
    const int *  const pConstP = &a;
//    *pConstP = 50; //编译错误;
//    pConstP = &b; //编译错误;

    int arr[10] = {1,2,3,4,5,6,7,8,9,10};
    int * pArr = arr;
    for(int i = 0;i < 10;i++){
        cout << "arr" << i << ": " << *pArr << endl;
        pArr++; //表示向后偏移一个数据单位，即取数组下一个元素
    }
}
