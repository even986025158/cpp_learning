/*
 * 本文件只证明：经基类指针 delete 一个派生对象时，析构顺序是
 *   派生析构函数体 → 成员按声明的逆序 → 基类析构函数体。
 * 不证明：缺虚析构时的未定义行为、多基类、构造顺序（构造是这条链的逆过程，本组不记构造日志）。
 *
 * 相对旧版只改了两处可见结构：给 Derived 写了析构函数体以便看见“自己”；
 * 把一个 Marker 拆成 First / Second 两个成员，以便看见成员是声明序还是逆序。
 */
#include "check.h"
#include <string>

static std::string destruction_log;

struct First {
    ~First() { destruction_log += "~First "; }
};

struct Second {
    ~Second() { destruction_log += "~Second "; }
};

struct Base {
    virtual ~Base() { destruction_log += "~Base "; }
};

struct Derived : Base {
    First first;   // 先声明
    Second second; // 后声明
    ~Derived() { destruction_log += "~Derived "; }
};

int main()
{
    Base *object = new Derived();
    delete object;

    // 若成员按声明序拆：     "~Derived ~First ~Second ~Base "
    // 若先拆成员再跑自己：   "~Second ~First ~Derived ~Base "
    // 若先拆基类：           "~Derived ~Second ~First " 且 ~Base 在更前
    FD_CHECK(destruction_log == "~Derived ~Second ~First ~Base ",
             "derived body, then members in reverse declaration order, then base");
    return 0;
}
