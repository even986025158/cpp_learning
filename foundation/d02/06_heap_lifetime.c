/*
 * 本文件只证明：malloc 得到一块独立寿命的内存，free 之后必须丢掉指针，不能再读。
 * 不证明：use-after-free 的崩溃形态（那种变体只写在注释里，正常程序不执行）。
 */
#include "check.h"
#include <stdlib.h>
#include <string.h>

int main(void)
{
    int *p = malloc(sizeof(int) * 2);
    FD_CHECK(p != NULL, "malloc two ints");
    p[0] = 11;
    p[1] = 22;
    FD_CHECK(p[1] - p[0] == 11, "both cells are writable before free");

    int *alias = p;
    FD_CHECK(alias == p, "alias equals the live pointer");

    free(p);
    p = NULL;
    alias = NULL;
    FD_CHECK(p == NULL && alias == NULL, "all copies are nulled; memory is not read after free");

    /* 错误变体（不要取消注释）：free 后读 *alias 是 UB，ASan 通常会报 heap-use-after-free。 */
    return 0;
}
