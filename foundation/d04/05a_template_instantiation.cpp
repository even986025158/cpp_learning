/*
 * 本文件只证明：同一函数模板能以 int 和 long 两种 T 实例化并运行。
 * 不证明：生成了几份机器码（那需要符号/反汇编证据）、SFINAE、完美转发。
 */
#include "check.h"

template <typename T>
T identity(T value)
{
    return value;
}

int main()
{
    FD_CHECK(identity(7) == 7, "identity<int> returns int value");
    FD_CHECK(identity(3L) == 3L, "identity<long> returns long value");
    return 0;
}
