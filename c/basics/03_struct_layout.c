/* 知识点 3: struct 布局 / offsetof / 对齐填充
 *
 * 核心心法: 每个类型有"对齐要求"(通常等于自身大小),字段地址必须是它的整数倍。
 *          编译器为此在字段之间插入 padding,并把整个 struct 的大小
 *          补齐到"最大对齐要求"的整数倍(方便数组连续排列)。
 */
#include <stdio.h>
#include <stddef.h>   /* offsetof */

/* 坏布局: 大小交错 */
struct Bad {
    char  a;      /* 1 字节 */
    int   b;      /* 4 字节, 要求 4 对齐 -> 前面填 3 */
    char  c;      /* 1 字节 */
    double d;     /* 8 字节, 要求 8 对齐 -> 前面填 7 */
    char e;       /* 1 字节 */
    int f;        /* 4 字节, 要求 4 对齐 -> 前面填 3 , 对齐多少只看当前类型 */
    char g;       /* 1 字节 */
    /* 由于最后不是 最大类型字节(double 8字节)的倍数，所以最后还需要补齐 7 字节 */
    // 所以总共 40字节
};

/* 好布局: 从大到小排列 */
struct Good {
    double d;
    int    b;
    char   a;
    char   c;
};

/* 嵌套 struct: 继承内部最大对齐 */
struct Inner { int x; char y; }; //内部最大是int 4字节，所以最后还需要补齐3字节，所以总共8字节
struct Outer {
    char head;  // 1字节
    struct Inner in; //需要先补3字节，然后自己占8字节
    char tail; // 1 字节，最后不是4倍数，所以还要补3字节
    // 所以 Outer 总数是 1 + 3 + 8 + 1 + 3 = 16;
};


/* 位域: 把多个标志压进一个整数 */
struct Flags {
    unsigned int is_null : 1;
    unsigned int is_toast: 1;
    unsigned int len     : 30;
};

#define DUMP(T, F) \
    printf("  %-10s offset=%2zu  size=%2zu  align=%2zu\n", \
           #F, offsetof(T, F), sizeof(((T *)0)->F), _Alignof(__typeof__(((T *)0)->F)))

int main(void) {
    printf("=== 3.0 基本类型的大小/对齐 ===\n");
    printf("  char=%zu/%zu  short=%zu/%zu  int=%zu/%zu  long=%zu/%zu  double=%zu/%zu  ptr=%zu/%zu  (size/align)\n",
           sizeof(char), _Alignof(char), sizeof(short), _Alignof(short),
           sizeof(int), _Alignof(int), sizeof(long), _Alignof(long),
           sizeof(double), _Alignof(double), sizeof(void *), _Alignof(void *));

    printf("\n=== 3.1 struct Bad (字段乱序) ===\n");
    DUMP(struct Bad, a);
    DUMP(struct Bad, b);
    DUMP(struct Bad, c);
    DUMP(struct Bad, d);
    DUMP(struct Bad, e);
    DUMP(struct Bad, f);
    DUMP(struct Bad, g);
    /*
     * 总大小 = 40, 对齐 = 8, 有效数据只有 20 字节 -> padding 20 字节
     *
     */
    printf("  总大小 = %zu, 对齐 = %zu, 有效数据只有 %zu 字节 -> padding %zu 字节\n",
           sizeof(struct Bad), _Alignof(struct Bad),
           sizeof(char) * 4 + sizeof(int) * 2 + sizeof(double),
           sizeof(struct Bad) - (sizeof(char) * 4 + sizeof(int) * 2 + sizeof(double)));

    printf("\n=== 3.2 struct Good (从大到小) ===\n");
    DUMP(struct Good, d);
    DUMP(struct Good, b);
    DUMP(struct Good, a);
    DUMP(struct Good, c);
    printf("  总大小 = %zu  <- 同样的字段,省了 %zu 字节\n",
           sizeof(struct Good), sizeof(struct Bad) - sizeof(struct Good));

    printf("\n=== 3.3 嵌套 struct ===\n");
    printf("  sizeof(Inner) = %zu, alignof(Inner) = %zu\n",
           sizeof(struct Inner), _Alignof(struct Inner));
    DUMP(struct Outer, head);
    DUMP(struct Outer, in);
    DUMP(struct Outer, tail);
    printf("  sizeof(Outer) = %zu\n", sizeof(struct Outer));

    printf("\n=== 3.4 位域 (PG 的 t_infomask 思路) ===\n");
    printf("  sizeof(struct Flags) = %zu  <- 32 bit 塞进 4 字节\n", sizeof(struct Flags));
    struct Flags f = {0}; // 这个是对整个 Flags 结构体置0，不是 f.is_null = 0
    printf("  is_null=%u is_toast=%u len=%u\n", f.is_null, f.is_toast, f.len);
    f.is_null = 1; f.len = 123;
    printf("  is_null=%u is_toast=%u len=%u\n", f.is_null, f.is_toast, f.len);

    printf("\n=== 3.5 用实际地址验证 offsetof ===\n");
    struct Bad b;
    printf("  &b       = %p\n", (void *) &b);
    printf("  &b.d     = %p  (差值 %td, offsetof 说 %zu)\n",
           (void *) &b.d, (char *) &b.d - (char *) &b, offsetof(struct Bad, d));
    printf("  offsetof 就是 ((char*)&obj.field - (char*)&obj),但不需要真实对象\n");

    printf("\n=== 3.6 padding 里是垃圾数据 ===\n");
    printf("  所以比较 struct 不能用 memcmp,必须逐字段比;\n");
    printf("  写磁盘/发网络前要 memset 清零,否则会泄漏栈上残留内容。\n");
    return 0;
}
