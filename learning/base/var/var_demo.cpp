#include <iostream>

/**
 * 变量: 本质就是 数据存储在内存中，然后为了可以使用这个数据，给这块存储 起了个 名字，然后使用这个数据的时候 直接通过这个名字来使用就可以了。
 * 语法:
 * 数据类型 变量名 = 变量初始值;
 *
 * 不同的 数据类型 本质的区别 其实就是 大小，即 这个类型的变量占用了多少内存；然后你再通过这个大小的内存 构造你需要的 数据结构
 *
 * 基本数据类型:
 * bool
 * [unsigned] char
 * [unsigned] short
 * [unsigned] int
 * [unsigned] long
 * [unsigned] long long
 * float
 * double
 *
 */


int main(){
    int age = 18;
    std::cout << "my age is " << age << std::endl;

    std::cout << "size of int :  " << sizeof(int) << std::endl;
}

