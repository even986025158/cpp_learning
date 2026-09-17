/* ============================================================
 * 第 1 课补充：(void *) 和 (char *) 到底有什么区别
 * ============================================================
 *
 * 一句话答案：
 *   两者存的编号完全一样，区别只在「编译器允许你用它做什么」。
 *
 *   (void *)  = 「一个编号，但不告诉你那里放的是什么」
 *               -> 不能 *p 读内容，不能 p+1 移动。只能传递和比较。
 *               -> printf 的 %p 要求它：因为 printf 只想打印编号本身。
 *
 *   (char *)  = 「一个编号，那里放的是 1 字节」
 *               -> 能 *p 读那一个字节，能 p+1（正好走 1 个柜子）。
 *               -> 所以用它做地址相减，得到的差值单位正好是「字节数」。
 *
 * 核心机制：指针类型决定「步长」（一步跨几个柜子）。
 *   char*   步长 1        int*    步长 4        double* 步长 8
 *   void*   没有步长（宽度未知，所以标准不允许算术）
 */
#include <stdio.h>
#include <string.h>

int main(void)
{
    int x = 0x11223344;      /* 4 字节，故意每个字节都不一样，方便观察 */
    int y = 99;

    printf("=== 5.1 同一个地址，转成不同类型，编号完全一样 ===\n");
    printf("  (void *) &x = %p\n", (void *) &x);
    printf("  (char *) &x = %p   <- 一模一样的数字\n", (void *) (char *) &x);
    printf("  (int  *) &x = %p   <- 还是一样\n", (void *) &x);
    printf("  结论：转换不改变编号，只改变「编译器认为这里放的是多宽的东西」\n");

    printf("\n=== 5.2 区别在步长：+1 走几个柜子 ===\n");
    char   *pc = (char *)   &x;
    int    *pi =            &x;
    double *pd = (double *) &x;   /* 只为演示步长，不真的去读 */
    printf("  char*   起点 %p, +1 后 %p  -> 走了 %td 个柜子\n",
           (void *) pc, (void *) (pc + 1), (char *) (pc + 1) - (char *) pc);
    printf("  int*    起点 %p, +1 后 %p  -> 走了 %td 个柜子\n",
           (void *) pi, (void *) (pi + 1), (char *) (pi + 1) - (char *) pi);
    printf("  double* 起点 %p, +1 后 %p  -> 走了 %td 个柜子\n",
           (void *) pd, (void *) (pd + 1), (char *) (pd + 1) - (char *) pd);
    printf("  ^ 同一个起点，+1 的结果不同，因为类型决定了一个元素多宽\n");

    printf("\n=== 5.3 为什么地址相减要先转 (char *) ===\n");
    printf("  想问「x 和 y 隔了几个字节」：\n");
    printf("  (char *)&y - (char *)&x = %td   <- 单位是字节，这才是我们想问的\n",
           (char *) &y - (char *) &x);
    printf("  &y - &x                 = %td   <- 单位是「int 个数」，被除以 4 了\n",
           &y - &x);
    printf("  ^ 两者都合法但含义不同。想数「隔几个柜子」必须用 char*，因为 char 正好 1 柜\n");
    printf("  ^ 而 (void*)&y - (void*)&x 根本编译不过：void 宽度未知，不能做算术\n");

    printf("\n=== 5.4 char* 能真的读出每一个字节 ===\n");
    /* x = 0x11223344，用 char* 逐字节看它在内存里怎么排的 */
    unsigned char *bytes = (unsigned char *) &x;
    printf("  x = 0x%08X，在内存里逐字节是：", x);
    for (size_t i = 0; i < sizeof(x); i++)
        printf("%02X ", bytes[i]);
    printf("\n  ^ 打出 44 33 22 11 说明是小端序（低位字节在低地址）\n");
    printf("  ^ 这件事只有 char* 能做到：void* 不能解引用，int* 一次读 4 字节\n");

    printf("\n=== 5.5 void* 能做什么、不能做什么 ===\n");
    void *pv = &x;
    printf("  能：赋值、传参、和 NULL 比较、打印 %%p     -> pv = %p\n", pv);
    printf("  不能：*pv          （不知道该读几个字节）\n");
    printf("  不能：pv + 1       （不知道该走几个柜子）\n");
    printf("  不能：pv[0]        （同上）\n");
    printf("  要用内容，必须先转回具体类型：*(int *)pv = %d\n", *(int *) pv);

    printf("\n=== 5.6 void* 的真正用途：写「不关心类型」的通用函数 ===\n");
    /* memcpy / memset / malloc / free / qsort 的参数都是 void*，
     * 因为它们逐字节干活，压根不需要知道你传的是 int 还是 struct。 */
    printf("  memcpy(void *dst, const void *src, size_t n)\n");
    printf("  malloc 返回 void*  -> 所以赋给任何类型的指针都不用显式转换\n");
    int a[3] = {1, 2, 3}, b[3] = {0};
    memcpy(b, a, sizeof(a));       /* 传 int[] 进 void* 参数，自动转 */
    printf("  memcpy 之后 b = %d %d %d  <- 它不知道也不关心这是 int\n",
           b[0], b[1], b[2]);

    printf("\n=== 5.7 回答「为什么 printf 的 %%p 要 (void *)」 ===\n");
    printf("  printf 是可变参数函数，编译器不检查类型。\n");
    printf("  %%p 的规范要求实参必须是 void*（或 char*），传别的是未定义行为。\n");
    printf("  实践中传 int* 通常也能打对，但 -Wall -Wpedantic 会警告，\n");
    printf("  且理论上在「不同指针类型宽度不同」的架构上会出错。所以固定写 (void *)。\n");

    (void) pd;   /* 只为演示步长，避免 -Wunused 警告 */
    return 0;
}

/* ------------------------------------------------------------
 * 速记表
 *
 * | 场景                     | 用哪个   | 为什么                        |
 * |--------------------------|----------|-------------------------------|
 * | printf("%p", ...)        | (void *) | %p 的规范要求                 |
 * | 算「隔了几个字节」        | (char *) | char 宽 1，相减结果单位是字节 |
 * | 逐字节查看内存内容        | (unsigned char *) | 唯一能安全逐字节读的类型 |
 * | 写通用函数（memcpy 那种） | void *   | 表达「我不关心类型」          |
 * | 要真正读写内容            | 具体类型 | void* 不能解引用              |
 *
 * 一句话：void* 是「只要编号，不要类型」；char* 是「编号 + 步长为 1」。
 * ------------------------------------------------------------ */
