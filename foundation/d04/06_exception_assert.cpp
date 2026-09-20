/*
 * 本文件只证明：抛出的异常能被对应的 catch 接住；通过的 assert 不终止进程。
 * 不证明：noexcept、跨线程异常、故意失败的 assert（那会 abort，不进默认测试）。
 */
#include "check.h"
#include <cassert>
#include <stdexcept>
#include <string>

static int checked_div(int a, int b)
{
    if (b == 0) {
        throw std::runtime_error("div-by-zero");
    }
    return a / b;
}

int main()
{
    FD_CHECK(checked_div(8, 2) == 4, "normal path returns 4");

    std::string what;
    try {
        (void)checked_div(1, 0);
        FD_CHECK(0, "div-by-zero should have thrown");
    } catch (const std::runtime_error &ex) {
        what = ex.what();
    }
    FD_CHECK(what == "div-by-zero", "catch received the runtime_error");

    assert(2 + 2 == 4);
    FD_CHECK(true, "assert that holds does not abort");
    return 0;
}
