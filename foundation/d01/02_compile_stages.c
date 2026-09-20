/*
 * 本文件只证明：一份合法翻译单元能走完预处理→编译→汇编→链接，并带上 C17 宏。
 * 不证明：链接报错、头文件搜索路径、故意失败的 -E/-c 变体（那些写在 04_link_errors/README）。
 */
#include "check.h"

#define STAGE_MARK 17

int main(void)
{
    FD_CHECK(STAGE_MARK == 17, "object-like macro survives to runtime as 17");
    FD_CHECK(__STDC_VERSION__ >= 201710L, "compiled as C17 or newer");
    printf("note: gcc -E / -S / -c are manual stages; this binary is the last stage\n");
    return 0;
}
