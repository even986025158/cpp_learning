/*
 * 本文件只证明：参数个数不同的函数，必须先统一成「一个盒子」，才能放进同一张表。
 * 不证明：真正的 Datum / PG_FUNCTION_ARGS 展开、类型安全（统一后编译器不再检查个数）。
 */
#include "check.h"
#include <stdint.h>
#include <string.h>

#define MAX_ARGS 4

typedef struct {
    int nargs;
    int64_t args[MAX_ARGS];
} CallInfo;

typedef int64_t (*UniformFn)(CallInfo *);

#define GETARG_INT(ci, n) ((int)(ci)->args[(n)])
#define RETURN_INT(x) return (int64_t)(x)

static int64_t u_add(CallInfo *ci)
{
    RETURN_INT(GETARG_INT(ci, 0) + GETARG_INT(ci, 1));
}

static int64_t u_abs(CallInfo *ci)
{
    int x = GETARG_INT(ci, 0);
    RETURN_INT(x < 0 ? -x : x);
}

static int64_t u_now(CallInfo *ci)
{
    (void)ci;
    RETURN_INT(1758000000);
}

typedef struct {
    const char *name;
    UniformFn fn;
} Entry;

int main(void)
{
    const Entry catalog[] = {
        {"add", u_add},
        {"abs", u_abs},
        {"now", u_now},
    };

    CallInfo ci;
    memset(&ci, 0, sizeof(ci));
    ci.nargs = 2;
    ci.args[0] = 10;
    ci.args[1] = 3;
    FD_CHECK(catalog[0].fn(&ci) == 13, "add via uniform box");

    ci.nargs = 1;
    ci.args[0] = -42;
    FD_CHECK(catalog[1].fn(&ci) == 42, "abs via the same call shape");

    ci.nargs = 0;
    FD_CHECK(catalog[2].fn(&ci) == 1758000000, "now ignores the box contents");
    FD_CHECK(catalog[0].fn != catalog[1].fn, "table entries are distinct functions");
    return 0;
}
