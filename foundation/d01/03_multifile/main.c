/*
 * 本文件只证明：main 只看到声明就能调用，真正的目标文件由链接器拼进来。
 * 不证明：undefined reference / duplicate symbol（见 04_link_errors/README）。
 */
#include "calc.h"
#include "check.h"

int main(void)
{
    FD_CHECK(calc_add(2, 3) == 5, "linked calc_add(2, 3) == 5");
    FD_CHECK(calc_mul(4, 5) == 20, "linked calc_mul(4, 5) == 20");
    return 0;
}
