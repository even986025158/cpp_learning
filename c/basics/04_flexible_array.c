/* 知识点 4: flexible array member (柔性数组成员)
 *
 * 核心心法: struct 最后一个成员写成 char data[];  它 sizeof 为 0,
 *          真正的空间由 malloc(sizeof(S) + len) 一次性分配在头部后面。
 *          好处: 头 + 数据在同一块连续内存 -> 一次 malloc / 一次 free /
 *                一次 memcpy 就能写盘发网络,cache 友好。
 *          PG 里叫 FLEXIBLE_ARRAY_MEMBER,HeapTupleHeaderData、varlena、
 *          ItemPointer 数组等到处在用。
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

/* ---------- 4.1 最小例子 ---------- */
struct S {
    int  n;             /* 数据长度 */
    char data[];        /* flexible array member: 必须是最后一个成员 */
};

/* ---------- 4.2 对照: 用指针的传统写法 ---------- */
struct SPtr {
    int   n;
    char *data;         /* 两块内存,两次 malloc,两次 free */
};

/* ---------- 4.3 PG 风格: 变长元组头 ---------- */
typedef struct Tuple {
    unsigned int t_len;         /* 整条记录总长 */
    unsigned short t_natts;     /* 字段数 */
    unsigned short t_infomask;  /* 标志位 */
    char        t_bits[];       /* 变长部分: null bitmap + 实际数据 */
} Tuple;

static struct S *make_s(const char *src)
{
    size_t len = strlen(src) + 1;
    /* 关键: sizeof(struct S) 不含 data,再加上要的字节数 */
    struct S *s = malloc(sizeof(struct S) + len);
    s->n = (int) len;
    memcpy(s->data, src, len);
    return s;
}

static Tuple *make_tuple(const char *payload, unsigned short natts)
{
    size_t plen = strlen(payload) + 1;
    size_t total = offsetof(Tuple, t_bits) + plen;   /* 推荐用 offsetof 更严谨 */
    Tuple *t = malloc(total);
    memset(t, 0, total);                              /* 清 padding */
    t->t_len = (unsigned int) total;
    t->t_natts = natts;
    t->t_infomask = 0x0001;
    memcpy(t->t_bits, payload, plen);
    return t;
}

int main(void)
{
    printf("=== 4.1 sizeof: data[] 不占空间 ===\n");
    printf("  sizeof(struct S)         = %zu  <- 只有 int n\n", sizeof(struct S));
    printf("  offsetof(struct S, data) = %zu  <- data 紧跟在 n 后面\n", offsetof(struct S, data));
    printf("  sizeof(struct SPtr)      = %zu  <- int + padding + char* \n", sizeof(struct SPtr));

    printf("\n=== 4.2 分配与使用 ===\n");
    struct S *s = make_s("hello flexible array");
    printf("  s        = %p\n", (void *) s);
    printf("  s->data  = %p  <- 只差 %td 字节,数据紧贴在头后面\n",
           (void *) s->data, (char *) s->data - (char *) s);
    printf("  s->n = %d, s->data = \"%s\"\n", s->n, s->data);
    printf("  malloc 了 %zu 字节 = sizeof(S) %zu + 数据 %d\n",
           sizeof(struct S) + s->n, sizeof(struct S), s->n);
    free(s);                    /* 一次 free 全部搞定 */

    printf("\n=== 4.3 对照传统指针写法 ===\n");
    struct SPtr sp;
    sp.n = 6;
    sp.data = malloc(sp.n);
    memcpy(sp.data, "hello", 6);
    printf("  &sp      = %p\n", (void *) &sp);
    printf("  sp.data  = %p  <- 完全不同的内存区域(要跳一次指针,cache miss)\n", (void *) sp.data);
    free(sp.data);              /* 必须分两次 free,容易漏 */

    printf("\n=== 4.4 PG 风格变长元组 ===\n");
    Tuple *t = make_tuple("id=1,name=pg", 2);
    printf("  offsetof(Tuple, t_bits) = %zu\n", offsetof(Tuple, t_bits));
    printf("  t_len=%u t_natts=%u t_infomask=0x%04x payload=\"%s\"\n",
           t->t_len, t->t_natts, t->t_infomask, t->t_bits);
    printf("  整条记录连续 %u 字节 -> 可以直接 write() 到磁盘页\n", t->t_len);

    printf("\n=== 4.5 变长数组元素(不只是 char) ===\n");
    struct IntList { int count; int items[]; };
    int n = 5;
    struct IntList *il = malloc(sizeof(struct IntList) + n * sizeof(int));
    il->count = n;
    for (int i = 0; i < n; i++) il->items[i] = (i + 1) * (i + 1);
    printf("  count=%d items=", il->count);
    for (int i = 0; i < il->count; i++) printf("%d ", il->items[i]);
    printf("\n");
    free(il);

    printf("\n=== 规则速记 ===\n");
    printf("  1. 必须是 struct 的最后一个成员,且前面至少有一个其他成员\n");
    printf("  2. sizeof(struct) 不包含它\n");
    printf("  3. 不能整体赋值 (*a = *b 不会拷贝变长部分),要用 memcpy\n");
    printf("  4. 不能放进数组,也不能作为另一个 struct 的非末尾成员\n");
    printf("  5. 长度必须自己在 struct 里记住(n / t_len),没人帮你记\n");
    return 0;
}
