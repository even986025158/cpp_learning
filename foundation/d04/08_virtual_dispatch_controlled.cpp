/*
 * 本文件只证明：调用形式、对象和函数体相同时，基类函数是否写 virtual
 * 决定经基类引用调用时得到 0 还是 5。
 * 不证明：vtable 的内存布局、指针绑定、对象切片、析构、const、explicit。
 */
#include "check.h"

namespace without_virtual {
struct Base {
    int evaluate() const { return 0; }
};

struct Derived : Base {
    int evaluate() const { return 5; }
};
} // namespace without_virtual

namespace with_virtual {
struct Base {
    virtual int evaluate() const { return 0; } // 唯一因果旋钮：多了 virtual
};

struct Derived : Base {
    int evaluate() const { return 5; }
};
} // namespace with_virtual

int main()
{
    without_virtual::Derived plain_object;
    const without_virtual::Base &plain_base_name = plain_object;
    FD_CHECK(plain_object.evaluate() == 5,
             "control: derived name calls the derived function");
    FD_CHECK(plain_base_name.evaluate() == 0,
             "without virtual: base name calls the base function");

    with_virtual::Derived virtual_object;
    const with_virtual::Base &virtual_base_name = virtual_object;
    FD_CHECK(virtual_object.evaluate() == 5,
             "control: derived name still calls the derived function");
    FD_CHECK(virtual_base_name.evaluate() == 5,
             "with virtual: base name dispatches to the derived function");
    return 0;
}
