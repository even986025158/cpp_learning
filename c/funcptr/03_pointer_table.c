/* ============================================================
 * 第 3 课：函数指针表 —— C 语言里的"手工 vtable"
 * ============================================================
 *
 * 上一课：一个变量存一个函数编号。
 * 这一课：把一堆函数编号**排成一张表**。
 *
 * 为什么要排成表？因为这样就能"按索引/按名字挑一个执行"，
 * 而挑哪个可以完全由运行时数据决定。
 *
 * 这就是 Java 里 interface + 实现类的 C 语言版本：
 *   Java：编译器帮你生成 vtable，你写 obj.method() 就自动分发
 *   C  ：没人帮你，vtable 得你自己用 struct + 函数指针拼出来
 *
 * ★ PG 源码里 TableAmRoutine / IndexAmRoutine 就是这个东西。
 *   任务卡说「IDE 跳不过去」，原因这一课就能看明白：
 *   调用点写的是 am->scan_next(...)，具体是谁在编译期没写死，
 *   静态索引自然跳不过去。
 */
#include <stdio.h>
#include <string.h>

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }

/* ---------- 形式一：函数指针数组 ---------- */

/* 用 typedef 给"函数指针类型"起个名，后面写起来清爽得多。
 * 读法：BinOp 是一个类型，指向「吃两个 int、还一个 int」的函数。
 * ★ PG 里的 PGFunction 就是这么定义的（fmgr.h:40）。 */
typedef int (*BinOp)(int, int);

/* ---------- 形式二：struct 里放一堆函数指针 ---------- */

/* 这个更像真正的 vtable / 接口。
 * ★ PG 的 TableAmRoutine 就是这个形状：一个 struct，几十个函数指针字段。 */
typedef struct {
    const char *name;
    BinOp       fn;
} Operator;

int main(void)
{
    printf("=== 3.1 函数指针数组：按下标挑 ===\n");

    BinOp table[3] = { add, sub, mul };   /* 三个编号排成一排 */

    for (int i = 0; i < 3; i++) {
        printf("  table[%d](10, 3) = %d\n", i, table[i](10, 3));
    }
    printf("  同一句 table[i](10,3)，i 变了结果就变。\n");

    printf("\n=== 3.2 按名字查表（更接近数据库的做法）===\n");

    /* 这张表现在有了"名字 -> 实现"的映射。
     * ★ 这正是 PG 系统表 pg_operator / pg_proc 干的事，
     *   只不过 PG 的表存在磁盘上，这里存在内存里。 */
    Operator ops[] = {
        { "+", add },
        { "-", sub },
        { "*", mul },
    };
    const int n_ops = (int) (sizeof(ops) / sizeof(ops[0]));

    /* 假装这个符号是运行时才拿到的（用户输入的 SQL 里解析出来的） */
    const char *wanted[] = { "*", "+", "-" };

    for (int w = 0; w < 3; w++) {
        BinOp found = NULL;

        for (int i = 0; i < n_ops; i++) {
            if (strcmp(ops[i].name, wanted[w]) == 0) {
                found = ops[i].fn;        /* 查到了，把编号取出来 */
                break;
            }
        }

        if (found == NULL) {
            printf("  运算符 '%s' 不认识\n", wanted[w]);
            continue;
        }
        printf("  10 %s 3 = %d\n", wanted[w], found(10, 3));
    }

    printf("\n  ★ 请注意这三步，它就是 PG 执行一个运算符的缩影：\n");
    printf("     1. 拿到符号 \"%s\"（运行时数据）\n", "*");
    printf("     2. 查表，得到一个函数编号\n");
    printf("     3. 顺着编号调用\n");
    printf("     编译这段代码时，编译器并不知道第 3 步会跳到哪。\n");

    printf("\n=== 3.3 空指针陷阱（C 不替你检查）===\n");
    /* Java 里调用未实现的接口方法，编译期就过不了。
     * C 里函数指针默认是垃圾值，调用它 = 跳到随机地址 = 崩溃或更糟。
     * ★ PG 的 TableAmRoutine 里很多字段允许为 NULL，
     *   调用前必须自己判空 —— 漏判不报错，直接段错误。 */
    BinOp maybe = NULL;

    //printf("  maybe 是 NULL，调用前必须判空，%d。\n", maybe(1, 2)); // 直接执行会 程序直接 崩溃退出

    if (maybe == NULL) {
        printf("  maybe 是 NULL，调用前必须判空。\n");
        printf("  （把这个 if 去掉直接调 maybe(1,2) 试试，会段错误。）\n");
    }


    printf("\n【记住这一句】struct + 函数指针 = 手工 vtable。\n");
    printf("Java 的 interface 由编译器生成分发表；C 里这张表要你自己建、自己填、自己判空。\n");

    return 0;
}
