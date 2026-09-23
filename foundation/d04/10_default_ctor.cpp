/*
 * 本文件只证明：类没有声明任何构造函数时，编译器生成默认构造函数，
 * 并执行成员自己的构造和默认成员初始化器。
 * 不证明：写了带参构造之后默认构造是否仍存在。
 */
#include "check.h"
#include <string>

struct NoCtor {
    std::string text;
    int flag = 7;
};

int main()
{
    NoCtor object;
    FD_CHECK(object.text.empty(), "compiler-generated default ctor constructs string member");
    FD_CHECK(object.flag == 7, "default member initializer sets flag");
    return 0;
}
