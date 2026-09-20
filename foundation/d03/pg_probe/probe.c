/*
 * 本文件只证明：PG 头文件里的 foreach / lfirst / IsA 能被预处理器展开成普通 C。
 * 不证明：能链接 libpq、能运行、也不进入默认 ctest（仅可选 OBJECT 库）。
 *
 * 手工展开（不依赖 CMake）：
 *   export PG=/Users/even/Desktop/even/code/cpp/postgres
 *   clang -std=c17 -E -I "$PG/src/include" probe.c -o /tmp/fd_pg_probe.i
 *   sed -n '/^void demo/,$p' /tmp/fd_pg_probe.i
 */
#include "postgres.h"
#include "nodes/pg_list.h"

void demo(List *l);

void
demo(List *l)
{
    ListCell *lc;

    foreach(lc, l)
    {
        Node *n = (Node *)lfirst(lc);
        if (IsA(n, Var))
            elog(NOTICE, "var node");
    }
}
