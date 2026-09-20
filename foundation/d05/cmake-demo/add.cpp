/*
 * 本文件只证明：定义放进静态库，可执行文件通过链接拿到符号。
 * 不证明：动态库、dlopen。
 */
#include "add.hpp"

int fd_add(int a, int b)
{
    return a + b;
}
