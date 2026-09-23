/*
 * 本文件只证明：抛出的 runtime_error 能被对应的 catch 接住。
 * 不证明：assert、noexcept、跨线程异常。
 */
#include "check.h"
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
    return 0;
}
