/*
 * 本文件只证明：可执行文件链接静态库后能调用 fd_add，ctest 检查退出码。
 * 不证明：find_package、install、跨目录的复杂依赖图。
 */
#include "../../common/check.h"
#include "add.hpp"

int main()
{
    FD_CHECK(fd_add(2, 3) == 5, "static lib fd_add(2, 3) == 5");
    FD_CHECK(fd_add(-1, 1) == 0, "fd_add(-1, 1) == 0");
    return 0;
}
