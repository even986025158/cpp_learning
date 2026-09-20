/*
 * 本文件只证明：.c 里放定义，链接器能用这里的符号满足 main 的未定义引用。
 * 不证明：头文件搜索（-I）、静态库、重复定义。
 */
#include "calc.h"

int calc_add(int a, int b)
{
    return a + b;
}

int calc_mul(int a, int b)
{
    return a * b;
}
