/*
 * 本文件只证明：栈上对象离开作用域就析构，析构顺序与构造相反。
 * 不证明：异常路径上的栈展开细节、锁的真实并发行为。
 */
#include "check.h"
#include <string>

struct Guard {
    const char *name;
    std::string *log;
    // 构造函数，与struct同名，默认修饰符public，name(n), log(l) 会先进行属性的赋值，然后再执行函数体
    Guard(const char *n, std::string *l) : name(n), log(l)
    {
        *log += name;
        *log += '+';
    }
    // 析构函数，～struct名(){} ，不能带参数，对象销毁时自动调用
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
        // 不是 new/malloc/make_unique 出来的对象，是在栈上，栈上的对象生命周期在本{}内，出了{}就会自动销毁
        Guard outer("A", &log);
        {
            // 构造对象，调用对应的构造方法
            Guard inner("B", &log);
            FD_CHECK(log == "A+B+", "constructors run A then B");
        } // 这里会销毁 inner ，调用其析构函数
        FD_CHECK(log == "A+B+B-", "inner destructor runs when its block ends");
    }
    FD_CHECK(log == "A+B+B-A-", "outer destructor runs later, reverse of construction");
    return 0;
}
