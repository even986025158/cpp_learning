/*
 * 本文件只证明：栈上对象离开作用域就析构，析构顺序与构造相反。
 * 不证明：异常路径上的栈展开细节、锁的真实并发行为。
 */
#include "check.h"
#include <string>

struct Guard {
    const char *name;
    std::string *log;
    Guard(const char *n, std::string *l) : name(n), log(l)
    {
        *log += name;
        *log += '+';
    }
    ~Guard()
    {
        *log += name;
        *log += '-';
    }
};

int main()
{
    std::string log;
    {
        Guard outer("A", &log);
        {
            Guard inner("B", &log);
            FD_CHECK(log == "A+B+", "constructors run A then B");
        }
        FD_CHECK(log == "A+B+B-", "inner destructor runs when its block ends");
    }
    FD_CHECK(log == "A+B+B-A-", "outer destructor runs later, reverse of construction");
    return 0;
}
