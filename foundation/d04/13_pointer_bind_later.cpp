/*
 * 本文件只证明：C++ 指针可以先为空，再保存一个已有派生对象的地址。
 * 不证明：虚分派（该函数返回值在这里不是实验变量）。
 */
#include "check.h"

struct Base {
    virtual ~Base() = default;
    virtual int evaluate() const { return 0; }
};

struct Derived : Base {
    int evaluate() const override { return 5; }
};

int main()
{
    Derived object;
    const Base *base_pointer = nullptr;
    FD_CHECK(base_pointer == nullptr, "before binding, pointer is explicitly empty");

    base_pointer = &object;
    FD_CHECK(base_pointer == &object, "after binding, pointer stores object's address");
    FD_CHECK(base_pointer->evaluate() == 5, "dereference happens only after binding");
    return 0;
}
