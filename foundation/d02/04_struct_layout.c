/*
 * 本文件只证明：字段相同但顺序不同时，offsetof / sizeof 可以不同；对比的是字段不是地址字面量。
 * 不证明：位域、柔性数组、跨 ABI 稳定布局、用 memcmp 比较含 padding 的 struct。
 */
#include "check.h"
#include <stddef.h>

struct PackedFirst {
    char a;
    int b;
    char c;
};

struct Grouped {
    int b;
    char a;
    char c;
};

int main(void)
{
    FD_CHECK(offsetof(struct PackedFirst, a) == 0, "first field starts at 0");
    FD_CHECK(offsetof(struct PackedFirst, b) >= offsetof(struct PackedFirst, a) + sizeof(char),
             "b is after a (padding allowed)");
    FD_CHECK(offsetof(struct Grouped, b) == 0, "reordered struct starts with b");
    FD_CHECK(offsetof(struct Grouped, a) > offsetof(struct Grouped, b),
             "a follows b when we list b first");
    FD_CHECK(sizeof(struct PackedFirst) >= sizeof(char) + sizeof(int) + sizeof(char),
             "size is at least the raw fields");
    FD_CHECK(sizeof(struct Grouped) <= sizeof(struct PackedFirst),
             "same fields, grouped order is not larger");

    struct PackedFirst x;
    x.a = 1;
    x.b = 2;
    x.c = 3;
    FD_CHECK((char *)&x.b - (char *)&x == (ptrdiff_t)offsetof(struct PackedFirst, b),
             "offsetof matches address difference of the same object");
    FD_CHECK(x.a == 1 && x.b == 2 && x.c == 3, "fields are independently writable");
    return 0;
}
