/*
 * 本文件只证明：函数也有地址；函数指针保存并调用它，和直接写函数名是同一件事。
 * 不证明：函数指针表、统一签名、dlopen。
 */
#include "check.h"

static int add(int a, int b)
{
    return a + b;
}

static int sub(int a, int b)
{
    return a - b;
}

int main(void)
{
    int (*fn)(int, int) = add;
    FD_CHECK(fn == add, "fn holds add's address");
    FD_CHECK(fn != sub, "fn is not sub");
    FD_CHECK(fn(10, 3) == 13, "call through pointer equals add(10, 3)");

    fn = sub;
    FD_CHECK(fn == sub, "fn can be reassigned to sub");
    FD_CHECK(fn(10, 3) == 7, "same call site now runs sub");
    return 0;
}
