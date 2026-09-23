//
// Created by even on 2022/3/2.
//

/*
 * 头文件: 其实就是为了将 声明放在一个统一的地方，然后后面需要这些声明的时候 #include 就可以，而不用在每个文件中写这些声明
 * 头文件还有一个特点，就是编译器在预处理时 每个 cpp 文件只会 复制一次 头文件的内容，防止多次#include导致的重复声明
 */
int add(int num1, int num2);
int max(int num1, int num2);
long multi(int num1, int num2);
