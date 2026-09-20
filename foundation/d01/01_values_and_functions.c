/*
 * 本文件只证明：C 的值、表达式和函数调用按值传递，返回值能带回一个结果。
 * 不证明：指针、多文件链接、预处理宏、未定义行为。
 */
#include "check.h"

static int add(int a, int b)
{
    return a + b;
}

static int scale(int x)
{
    x = x * 2;
    return x;
}

int main(void)
{
    int a = 3;
    int b = 4;
    int sum = add(a, b);
    FD_CHECK(sum == 7, "add(3, 4) == 7");
    FD_CHECK(a == 3 && b == 4, "caller values unchanged after add");

    int x = 5;
    int y = scale(x);
    FD_CHECK(x == 5, "scale's parameter is a copy");
    FD_CHECK(y == 10, "scale returns doubled copy");
    return 0;
}
