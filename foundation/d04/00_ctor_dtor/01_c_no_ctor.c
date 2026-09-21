/*
 * 本文件只证明：C 的 struct 没有构造/析构，字段必须自己写，离开作用域没有回调。
 * 不证明：C++ 构造、柔性数组、未初始化局部变量的取值。
 */
#include "check.h"

struct Point {
    int x;
    int y;
};

static void fill(struct Point *p, int x, int y)
{
    p->x = x;
    p->y = y;
}

int main(void)
{
    struct Point p;
    fill(&p, 3, 4);
    FD_CHECK(p.x == 3 && p.y == 4, "C struct fields are filled by a function you call");
    /* 出了 main，p 的内存没了。没有 ~Point，不会再跑任何你写的函数。 */
    return 0;
}
