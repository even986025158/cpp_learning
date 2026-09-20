/*
 * 本文件只证明：多语句宏必须在语法上是一条语句，do { ... } while (0) 能跟在 if 后面。
 * 不证明：宏里 return/goto 的控制流、PG ereport 的全部展开。
 */
#include "check.h"

#define TWICE_BAD(stmt) stmt; stmt
#define TWICE(stmt)                                                                    \
    do {                                                                               \
        stmt;                                                                          \
        stmt;                                                                          \
    } while (0)

int main(void)
{
    int escaped = 0;
    if (0)
        TWICE_BAD(escaped++);
    FD_CHECK(escaped == 1, "TWICE_BAD second statement left the if");

    int guarded = 0;
    if (0)
        TWICE(guarded++);
    FD_CHECK(guarded == 0, "TWICE stays inside the if when condition is false");

    int ran = 0;
    if (1)
        TWICE(ran++);
    FD_CHECK(ran == 2, "TWICE runs both statements when condition is true");
    return 0;
}
