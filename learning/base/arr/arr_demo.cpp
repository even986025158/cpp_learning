//
// Created by even on 2022/3/2.
//
#include <iostream>

/**
 * 数组
 * 定义方法:
 * 1.数组类型 数组名[数组长度]; //这种没赋值不会取默认值
 * 2.数组类型 数组名[数组长度] = {值1，值2...}; //没有给的值则取默认值
 * 3.数组类型 数组名[] = {值1，值2...}; //有多少个元素就是数组长度
 *
 * 二维数组: 数据类型 数组名[行数][列数]
 */

int main(){
    using namespace std;

    int arr[5];
    arr[0] = 10;
    cout << arr[0] << endl;
    cout << arr[1] << endl; //32759
    cout << arr[2] << endl; //111052595

    int arr2[10] = {1,2,3};
    cout << arr2[7] << endl; //0
    cout << sizeof(arr2) << endl; //40 ;
    cout << "arr2数组长度:" << sizeof(arr2)/sizeof(arr2[0]) << endl; //10
    cout << "arr2数组首地址:" << arr2 << endl; //0x7ff7bb48c850
    cout << "arr2数组第一个元素首地址:" << &arr2[0] << endl; //0x7ff7bb48c850
    cout << "arr2数组第二个元素首地址:" << &arr2[1] << endl; //0x7ff7bfe06854
    cout << "arr2数组第二个元素首地址 - arr2数组第一个元素首地址:" << (char * ) &arr2[1] - (char * ) &arr2[0] << endl;
}
