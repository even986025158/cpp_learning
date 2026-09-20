/*
 * 本文件只证明：对象有地址，指针保存地址，解引用读写的是同一块对象。
 * 不证明：数组退化、指向指针的指针、堆分配、指针运算出界。
 */
#include "check.h"

int main(void)
{
    int x = 41;
    int *p = &x;

    FD_CHECK(p == &x, "p holds the address of x");
    FD_CHECK(*p == 41, "dereference reads x");
    *p = 42;
    FD_CHECK(x == 42, "writing through p updates x");
    FD_CHECK(p != NULL, "non-null pointer is not NULL");
    (void)p;
    return 0;
}
