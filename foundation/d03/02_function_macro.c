/*
 * 本文件只证明：函数宏是文本替换；缺括号会改变运算顺序；参数会被代入多次。
 * 不证明：do-while-zero、可变参数宏、SQUARE(j++) / SQUARE(bump()) 这类未定序副作用
 * （它们是 UB，只用 -E 观察展开，绝不运行）。
 */
#include "check.h"

#define SQUARE_BAD(x) x *x
#define SQUARE(x) ((x) * (x))

int main(void)
{
    FD_CHECK(SQUARE_BAD(1 + 2) == 5, "SQUARE_BAD(1+2) is 1+2*1+2 == 5");
    FD_CHECK(SQUARE(1 + 2) == 9, "SQUARE(1+2) is ((1+2)*(1+2)) == 9");

    int i = 3;
    int once = SQUARE(i);
    FD_CHECK(once == 9 && i == 3, "SQUARE(i) does not change i");

    FD_CHECK(SQUARE(4) == 16, "SQUARE(4) is ((4)*(4))");
    printf("note: use the textbook's stdin preprocessor probe for SQUARE(bump()); do not run it\n");
    return 0;
}
