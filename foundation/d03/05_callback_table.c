/*
 * 本文件只证明：签名相同的函数可以放进一张表，用下标/名字选中后再调用。
 * 不证明：签名不同怎么办（下一份）、运行时装库。
 */
#include "check.h"
#include <string.h>

static int add(int a, int b) { return a + b; }
static int mul(int a, int b) { return a * b; }
static int min_int(int a, int b) { return a < b ? a : b; }

typedef int (*BinOp)(int, int);

typedef struct {
    const char *name;
    BinOp fn;
} OpEntry;

int main(void)
{
    const OpEntry table[] = {
        {"add", add},
        {"mul", mul},
        {"min", min_int},
    };
    const int n = (int)(sizeof(table) / sizeof(table[0]));

    FD_CHECK(table[0].fn(2, 3) == 5, "table[0] is add");
    FD_CHECK(table[1].fn(2, 3) == 6, "table[1] is mul");
    FD_CHECK(table[2].fn(2, 3) == 2, "table[2] is min");

    const char *wanted = "mul";
    BinOp picked = NULL;
    for (int i = 0; i < n; i++) {
        if (strcmp(table[i].name, wanted) == 0) {
            picked = table[i].fn;
            break;
        }
    }
    FD_CHECK(picked != NULL, "name lookup finds mul");
    FD_CHECK(picked == mul, "looked-up pointer equals mul");
    FD_CHECK(picked(6, 7) == 42, "call after lookup is 42");
    return 0;
}
