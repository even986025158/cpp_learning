/* ============================================================
 * 第 1 课：函数也住在柜子里，也有编号
 * ============================================================
 *
 * warmup/02 里你学过：
 *   int x = 5;
 *   &x   取地址：从"名字" -> "编号"
 *   *p   解引用：从"编号" -> "那个柜子"
 *
 * 那时候柜子里放的是**数据**（整数 5）。
 *
 * 这一课只讲一件事：**函数也在内存里，也有编号**。
 * 区别只是柜子里放的不是数据，是**一串机器指令**。
 *
 * 所以 "add" 这个名字，和 "x" 这个名字，本质是同一种东西：
 * 都是某个内存位置的别名。
 *
 * 这一课不引入任何新语法，只是让你亲眼看见"函数有地址"这件事。
 */
#include <stdio.h>
#include <inttypes.h>

int add(int a, int b)
{
    return a + b;
}

int sub(int a, int b)
{
    return a - b;
}

int main(void)
{
    int x = 5;

    printf("=== 1.1 变量有编号（这个你已经知道了）===\n");
    printf("  x  的值   = %d\n", x);
    printf("  x  的编号 = %#" PRIxPTR "\n", (uintptr_t) &x);

    printf("\n=== 1.2 函数也有编号（这是今天的新东西）===\n");
    /* 注意：函数名后面【不加括号】。
     *   add()  = 调用它，拿到返回值
     *   add    = 不调用，就是它的编号本身
     * 这个区别是今天所有内容的地基。 */
    printf("  add 的编号 = %#" PRIxPTR "\n", (uintptr_t) add);
    printf("  sub 的编号 = %#" PRIxPTR "\n", (uintptr_t) sub);

    printf("\n  两个函数编号不同 -> 它们是内存里两个不同的位置。\n");

    printf("\n=== 1.3 加不加括号，差别在这里 ===\n");
    printf("  add(2, 3) = %d          <- 加括号：去执行，拿回结果\n", add(2, 3));
    printf("  add       = %#" PRIxPTR "  <- 不加括号：只是个编号\n", (uintptr_t) add);

    printf("\n=== 1.4 &add 和 add 是同一个东西 ===\n");
    /* 对函数来说，函数名本身就代表地址，所以 & 写不写都行。
     * 这一点和数组名很像（你在 basics/02_array_decay 见过类似的退化）。 */
    printf("  add  = %#" PRIxPTR "\n", (uintptr_t) add);
    printf("  &add = %#" PRIxPTR "   <- 完全一样，& 是多余的\n", (uintptr_t) &add);

    printf("\n【记住这一句】函数名 = 那段代码在内存里的编号。\n");
    printf("既然是编号，就能存进变量里 —— 那就是下一课的「函数指针」。\n");

    return 0;
}
