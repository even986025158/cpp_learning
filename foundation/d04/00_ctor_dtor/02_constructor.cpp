/*
 * 本文件只证明：C++ 造对象时自动跑构造；初始化列表先于函数体；写了带参构造就不会再有默认构造。
 * 不证明：拷贝构造、移动构造、explicit、虚析构。
 */
#include "check.h"
#include <string>

struct Box {
    int x;
    int y;
    std::string *log;

    Box(int a, int b, std::string *l) : x(a), y(b), log(l) /* (a) 先填成员 */
    {
        *log += "body"; /* (b) 成员填完才进函数体 */
    }
};

struct Alias {
    int &r;
    Alias(int &n) : r(n) {} /* 引用成员必须在初始化列表里绑上 */
};

int main()
{
    std::string log;
    Box b(1, 2, &log); /* (c) 这一行就是在构造，没有人再写 b.Box(...) */
    FD_CHECK(b.x == 1 && b.y == 2, "ctor arguments become members");
    FD_CHECK(log == "body", "initializer list runs before ctor body");

    int n = 10;
    Alias a(n);
    FD_CHECK(&a.r == &n, "reference member is bound in the initializer list");
    a.r = 11;
    FD_CHECK(n == 11, "writing the member reference writes n");
    return 0;
}
