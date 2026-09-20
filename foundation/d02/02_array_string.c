/*
 * 本文件只证明：数组名在表达式里退化成首元素指针；指针加减只在同一数组内。
 * 不证明：堆字符串、柔性数组成员、把指针当二维数组用。
 */
#include "check.h"
#include <string.h>

int main(void)
{
    int xs[4] = {10, 20, 30, 40};
    int *p = xs;

    FD_CHECK(p == &xs[0], "array decays to pointer to first element");
    FD_CHECK(sizeof(xs) == 4 * sizeof(int), "sizeof(array) is the whole object");
    FD_CHECK(sizeof(p) == sizeof(int *), "sizeof(pointer) is just the pointer");
    FD_CHECK(p[2] == 30, "p[i] is *(p + i) inside the same array");
    FD_CHECK((p + 3) - p == 3, "pointer subtraction of two cells in xs");
    FD_CHECK(&xs[3] > &xs[0], "later element has a greater address");

    char msg[] = "hi";
    FD_CHECK(sizeof(msg) == 3, "char array includes the '\\0'");
    FD_CHECK(strlen(msg) == 2, "strlen stops before '\\0'");
    FD_CHECK(msg[2] == '\0', "C string is terminated");
    return 0;
}
