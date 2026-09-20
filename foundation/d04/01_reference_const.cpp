/*
 * 本文件只证明：引用是已有对象的别名；const 引用不能改对象。
 * 不证明：右值引用、生命周期延长的全部规则、指针与 optional_ptr。
 */
#include "check.h"

static void add_one(int &x)
{
    x += 1;
}

static int read_only(const int &x)
{
    return x;
}

int main()
{
    int n = 10;
    int &alias = n;
    FD_CHECK(&alias == &n, "reference occupies the same address as n");
    alias = 11;
    FD_CHECK(n == 11, "writing the reference writes n");

    add_one(n);
    FD_CHECK(n == 12, "function reference parameter aliases the caller");
    FD_CHECK(read_only(n) == 12, "const reference can still read");

    const int &frozen = n;
    FD_CHECK(&frozen == &n, "const reference still aliases n");
    n = 13;
    FD_CHECK(frozen == 13, "object changed through the non-const name");
    return 0;
}
