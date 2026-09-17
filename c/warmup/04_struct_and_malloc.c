/* ============================================================
 * 第 4 课：struct（把几个变量打包）+ malloc（自己申请柜子）
 * ============================================================
 *
 * struct = 结构体 = "把好几个变量捆成一个新类型"
 *   像 Excel 的一行：一行里有 姓名、年龄、分数 好几列。
 *
 * malloc = memory allocate = "跟操作系统要一片柜子"
 *   之前的变量都是"自动"的：函数一结束就被回收（叫栈 stack）。
 *   malloc 要来的柜子归你管，你不还就一直是你的（叫堆 heap）。
 *   用完必须 free 还回去，不还就叫"内存泄漏"。
 */
#include <stdio.h>
#include <stdlib.h>   /* malloc / free 住在这个工具箱里 */
#include <string.h>   /* strcpy / memcpy / strlen 住在这里 */

/* --- 4.1 定义一个 struct --- */
struct Person {
    char name[20];    /* 20 个柜子，放名字 */
    int  age;         /* 4 个柜子 */
    double score;     /* 8 个柜子 */
};
/* 注意：定义 struct 的分号不能漏 --^ */

int main(void)
{
    printf("=== 4.1 用 . 访问成员 ===\n");
    struct Person p;              /* 一次划走一整片柜子 */
    strcpy(p.name, "Alice");      /* 字符串不能用 = 赋值，要用 strcpy 拷贝 */
    p.age = 30;
    p.score = 95.5;
    printf("  %s, %d 岁, %.1f 分\n", p.name, p.age, p.score);

    /* 也可以定义时一次性初始化 */
    struct Person q = {"Bob", 25, 88.0};
    printf("  %s, %d 岁, %.1f 分\n", q.name, q.age, q.score);

    printf("\n=== 4.2 struct 在内存里是连着的一片 ===\n");
    printf("  sizeof(struct Person) = %zu 字节\n", sizeof(struct Person));
    printf("  &p       = %p  <- 整个 struct 的起点\n", (void *) &p);
    printf("  &p.name  = %p  <- 就在起点\n", (void *) &p.name);
    printf("  &p.age   = %p  <- 往后 %td 个柜子\n",
           (void *) &p.age, (char *) &p.age - (char *) &p);
    printf("  &p.score = %p  <- 往后 %td 个柜子\n",
           (void *) &p.score, (char *) &p.score - (char *) &p);
    printf("  ^ 这个「往后多少个柜子」就叫偏移(offset)，第 5 课细讲\n");

    printf("\n=== 4.3 指向 struct 的指针用 -> 访问 ===\n");
    struct Person *pp = &p;
    /* (*pp).age 太难看，C 给了简写 pp->age，两者完全等价 */
    printf("  (*pp).age = %d\n", (*pp).age);
    printf("  pp->age   = %d   <- 一样，但好看多了。PG 源码里全是 ->\n", pp->age);
    pp->age = 31;                 /* 通过指针改原件 */
    printf("  通过 pp 改了以后 p.age = %d\n", p.age);

    printf("\n=== 4.4 malloc：自己要柜子 ===\n");
    /* malloc(n) 意思是"给我 n 个连着的空柜子，把起始编号告诉我"。
     * 它返回的是 void*（没说明类型的编号），赋给 struct Person* 会自动转。 */
    struct Person *r = malloc(sizeof(struct Person));
    if (r == NULL) {              /* 要柜子可能失败，正规写法都要判断 */
        printf("  内存不够了\n");
        return 1;
    }
    strcpy(r->name, "Carol");
    r->age = 28;
    r->score = 91.0;
    printf("  malloc 给的编号 = %p\n", (void *) r);
    printf("  %s, %d 岁, %.1f 分\n", r->name, r->age, r->score);
    free(r);                      /* 用完还回去。还完之后 r 就是野指针，不能再用 */
    r = NULL;                     /* 好习惯：还完顺手置空，防止手滑再用 */

    printf("\n=== 4.5 malloc 一片柜子放 N 个元素（动态数组）===\n");
    int n = 5;
    /* 要 5 个 int，就要 5 * sizeof(int) = 20 个柜子 */
    int *nums = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        nums[i] = (i + 1) * (i + 1);   /* malloc 来的也能用 [] 访问 */
    printf("  ");
    for (int i = 0; i < n; i++)
        printf("%d ", nums[i]);
    printf("\n");
    free(nums);

    printf("\n=== 4.6 字符串就是「以 0 结尾的一串 char」 ===\n");
    char s[] = "hi";
    printf("  \"hi\" 占 %zu 个柜子（h, i, 还有一个看不见的结束符 \\0）\n", sizeof(s));
    printf("  s[0]=%c(%d) s[1]=%c(%d) s[2]=(结束符)%d\n",
           s[0], s[0], s[1], s[1], s[2]);
    printf("  strlen(s) = %zu  <- 只数到结束符前，不含它\n", strlen(s));

    printf("\n=== 4.7 memcpy：整片柜子照搬 ===\n");
    /* memcpy(目标编号, 来源编号, 搬几个柜子) —— 不管里面是什么，逐字节复制 */
    struct Person copy;
    memcpy(&copy, &p, sizeof(struct Person));
    printf("  照搬之后 copy = %s, %d 岁\n", copy.name, copy.age);

    return 0;
}

/* ------------------------------------------------------------
 * 这一课的关键词对照表：
 *
 *   struct S { ... };      定义新类型（末尾分号！）
 *   s.field                有对象，用点
 *   ps->field              有指针，用箭头（等于 (*ps).field）
 *   malloc(n)              要 n 个柜子，返回起始编号；失败返回 NULL
 *   free(p)                还柜子；还完别再用
 *   sizeof(struct S)       这个类型占几个柜子
 *   strcpy / strlen        字符串专用（认结束符 \0）
 *   memcpy(dst,src,n)      整片搬，不管内容是什么
 * ------------------------------------------------------------ */
