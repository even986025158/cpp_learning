/*
 * 本文件只提供稳定的单元式断言（通过打印 ok: / FAIL:）。
 * 不证明任何语言特性；地址比较只判断相等，不打印硬编码地址。
 */
#ifndef FOUNDATION_CHECK_H
#define FOUNDATION_CHECK_H

#include <stdio.h>

#define FD_CHECK(cond, msg)                                                        \
    do {                                                                           \
        if (!(cond)) {                                                             \
            fprintf(stderr, "FAIL: %s\n", (msg));                                  \
            return 1;                                                              \
        }                                                                          \
        printf("ok: %s\n", (msg));                                                 \
    } while (0)

#endif /* FOUNDATION_CHECK_H */
