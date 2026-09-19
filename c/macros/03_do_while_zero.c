/* ============================================================
 * 宏第 3 课：为什么 PG 宏常常写成 do { ... } while (0)
 * ============================================================
 *
 * 若宏要做「两件以上的事」，直接替换会把 if 弄坏。看 BAD：
 *
 *   #define TWICE_BAD(stmt)  stmt; stmt //执行两次stmt
 *   if (ok)
 *       TWICE_BAD(printf("x\n"));
 *
 * 替换后变成：
 *   if (ok)
 *       printf("x\n");
 *   printf("x\n");          <- 第二句已经跑到 if 外面了
 *
 * 包进 do { ... } while (0) 之后，整坨在语法上是「一条语句」，
 * 可以跟在 if / else 后面，且 while (0) 保证只跑一遍。
 *
 * 先运行看 BAD 和 GOOD 各打印几次。
 * 再用 clang -E -P 03_do_while_zero.c 看替换后的文本。
 */
#include <stdio.h>

#define TWICE_BAD(stmt)  stmt; stmt

#define TWICE(stmt)      do { stmt; stmt; } while (0)

int main(void)
{
    int ok = 0;    /* 假：if 不该进 */

    printf("=== 3.1 BAD：第二句会逃出 if ===\n");
    if (ok)
        TWICE_BAD(printf("  BAD 跑了一次\n")); // 这里还是会输出一次 BAD 跑了一次
    /*
     * 替换后:  if (ok) printf("  BAD 跑了一次\n"); printf("  BAD 跑了一次\n"); 由于if没有{},所以第二个printf会输出
     */
    printf("  （上面哪怕 ok==0，BAD 仍可能打印一次）\n");

    printf("\n=== 3.2 GOOD：整坨是一条语句，ok==0 时一次都不跑 ===\n");
    if (ok)
        TWICE(printf("  GOOD 跑了一次\n"));
    printf("  （ok==0，这里不该看到 GOOD 的打印）\n");

    ok = 1;
    printf("\n=== 3.3 ok==1 时 GOOD 应打印两行 ===\n");
    if (ok)
        TWICE(printf("  GOOD 跑了一次\n"));

    return 0;
}

/* ------------------------------------------------------------
 * 这一课只记 2 件事：
 *   1. 多语句宏必须在语法上像「一条语句」
 *   2. PG 的 ereport / PG_TRY 外面那层 do { ... } while (0) 就是干这个
 *
 * 看懂这三课，就可以回头看 elog.h 里的 #define ereport ...
 * ------------------------------------------------------------ */
