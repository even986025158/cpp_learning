/*
 * 本文件只证明：函数宏是文本替换；缺括号会改变运算顺序；参数会被代入多次。
 * 不证明：do-while-zero、可变参数宏、SQUARE(j++) 那种未定序副作用（那是 UB，不在这里跑）。
 */
#include "check.h"

#define SQUARE_BAD(x) x *x
#define SQUARE(x) ((x) * (x))

static int g_bump;

static int bump(void)
{
    int current = g_bump;
    g_bump += 1;
    return current;
}

int main(void)
{
    FD_CHECK(SQUARE_BAD(1 + 2) == 5, "SQUARE_BAD(1+2) is 1+2*1+2 == 5");
    FD_CHECK(SQUARE(1 + 2) == 9, "SQUARE(1+2) is ((1+2)*(1+2)) == 9");

    int i = 3;
    int once = SQUARE(i);
    FD_CHECK(once == 9 && i == 3, "SQUARE(i) does not change i");

    g_bump = 3;
    int twice = SQUARE(bump());
    FD_CHECK(twice == 12, "bump() was substituted twice: 3*4 or 4*3");
    FD_CHECK(g_bump == 5, "two sequenced calls advanced the counter from 3 to 5");
    return 0;
}
