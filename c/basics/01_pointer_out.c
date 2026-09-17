/* 知识点 1: T* / T** —— out 参数
 *
 * 核心心法: C 的函数只有"一个返回值",想让函数改动调用方的东西,
 *          就必须把"那个东西的地址"传进去。
 *          想改 int   -> 传 int*
 *          想改 int*  -> 传 int**   (最常见: 函数内部 malloc,把指针交给调用方)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------- 1.1 T*: 修改调用方的 int ---------- */
void add_one(int *out)
{
    *out += 1;              /* * 解引用 = "顺着地址回到调用方的那块内存" */
}

/* 错误示范: 值传递改不动调用方 */
void add_one_wrong(int out)
{
    out += 1;               /* 只改了自己的副本 */
}

/* ---------- 1.2 T*: 一个函数返回多个结果(PG 风格) ---------- */
/* 返回值当"成功/失败",真正的数据从 out 参数出去 */
int divide(int a, int b, int *quotient, int *remainder)
{
    if (b == 0)
        return 0;           /* false: 失败 */
    *quotient  = a / b;
    *remainder = a % b;
    return 1;               /* true: 成功 */
}

/* ---------- 1.3 T**: 函数内部分配内存,交给调用方 ---------- */
void make_string(char **out, const char *src)
{
    /* out 是"指向 char* 的指针",*out 就是调用方那个 char* 变量本身
     * strlen(src) 表示 获取字符串长度，+1 是因为 C 字符串末尾还要一个 '\0'，所以要 9 个字节。少 +1 就会写出界。
     * malloc 表示 申请 strlen(src) + 1 长度的地址空间，然后返回这个地址的首地址
     * 所以 char * *out = 指向这个空间的指针
     * malloc 时会记录 这一块空间被分配了，free的时候就会清空这块空间。但是越界不是内核检查出来的，是ASan等工具记录发现的。
     * 如果没有ASan等工具，则可能存在越界不立即报错的情况，但是在其他操作，比如free会发现这里地址是有问题的，导致程序崩溃
     * */
    *out = malloc(strlen(src) + 1);
    // strcpy 表示 将char * src 这个首地址开始到 '\0' 的内容复制到 *out 中
    strcpy(*out, src);
}

/* 错误示范: 只传 char*,函数内改的是副本,调用方拿不到 */
void make_string_wrong(char *out, const char *src)
{
    out = malloc(strlen(src) + 1);   /* 改的是形参副本 */
    strcpy(out, src);
    /* 调用方的指针没变 -> 内存泄漏 + 调用方拿到野指针 */
}

/* ---------- 1.4 int** 的原型题: void get(int **out) ---------- */
void get(int **out)
{
    static int shared = 42;
    *out = &shared;         /* 把一个已存在对象的地址交给调用方 */
}

int main(void)
{
    printf("=== 1.1 T* 改 int ===\n");
    int x = 10;
    add_one_wrong(x);
    printf("add_one_wrong 之后 x = %d   (没变,值传递)\n", x);
    add_one(&x);            /* & 取地址 */
    printf("add_one       之后 x = %d   (变了)\n", x);

    printf("\n=== 1.2 多返回值 ===\n");
    int q, r;
    if (divide(17, 5, &q, &r))
        printf("17 / 5 = %d 余 %d\n", q, r);
    if (!divide(17, 0, &q, &r))
        printf("17 / 0 -> 返回 false,q/r 不被写入\n");

    printf("\n=== 1.3 T** 分配内存 ===\n");
    char *s = NULL;
    printf("调用前 s = %p\n", (void *) s);
    make_string(&s, "hello PG");
    printf("调用后 s = %p, 内容 = \"%s\"\n", (void *) s, s);
    free(s);

    char *bad = NULL;
    make_string_wrong(bad, "leak");
    printf("make_string_wrong 之后 bad = %p  (还是 NULL,内存泄漏了)\n", (void *) bad);

    printf("\n=== 1.4 void get(int **out) ===\n");
    int *p = NULL;
    get(&p);                /* 传 int* 的地址 => int** */
    printf("p = %p, *p = %d\n", (void *) p, *p);
    *p = 100;
    get(&p);
    printf("改成 100 后再 get, *p = %d (同一块 static 内存)\n", *p);

    /* 层级速记 */
    printf("\n--- 类型层级 ---\n");
    printf("int    x  : 一个整数\n");
    printf("int   *p  : 存着 int 的地址        -> 想改 int   就传它\n");
    printf("int  **pp : 存着 int* 的地址       -> 想改 int*  就传它\n");
    return 0;
}
