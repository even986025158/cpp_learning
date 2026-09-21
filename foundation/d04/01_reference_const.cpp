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
    // ok: reference occupies the same address as n; 说明 alias 和 n 指向的是同一个地址
    FD_CHECK(&alias == &n, "reference occupies the same address as n");
    alias = 11;
    // ok: writing the reference writes n; 修改了alias， n 也变了，再次证明两个是一个地址，且alias也可以修改其中的数据
    FD_CHECK(n == 11, "writing the reference writes n");

    // 虽然传的是n，但是由于 add_one(int &x) 声明的是 &x，所以在函数中 x 和 alias 一样 指向了传进来那个参数的地址，所以可以直接修改对应地址的数据
    add_one(n);
    FD_CHECK(n == 12, "function reference parameter aliases the caller");
    FD_CHECK(read_only(n) == 12, "const reference can still read");

    // const 修饰的引用不能修改对应地址中的数据
    const int &frozen = n;
    // frozen = 13; // 编译就不通过
    FD_CHECK(&frozen == &n, "const reference still aliases n");
    // 但是 n 不是const的，所以可以修改n
    n = 13;
    // ok: object changed through the non-const name；修改了n，同样会影响frozen，
    // 所以const只是让这个地址下的数据不能通过这个引用进行修改，但是引用中的数据可以被其他指向这个地址的其他非const引用修改
    FD_CHECK(frozen == 13, "object changed through the non-const name");

    // int &r; 编译不通过，引用必须初始化赋值
    return 0;
}
