/*
 * 本文件只证明：条件为真时，assert 不终止程序，控制流会继续。
 * 不证明：失败 assert 的进程信号、NDEBUG、DuckDB 的 D_ASSERT。
 */
#include "check.h"
#include <cassert>

int main()
{
    assert(2 + 2 == 4);
    FD_CHECK(true, "control reaches this line after a passing assert");
    return 0;
}
