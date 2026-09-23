/*
 * 本文件只证明：通过基类指针调用 virtual 函数会走到派生类 override。
 * 不证明：多重继承、纯虚接口的全部设计、delete 基类指针时没虚析构的 UB（这里写了虚析构）。
 */
#include "check.h"
#include <string>

struct Expr {
    virtual ~Expr() = default;
    // 尾部的 `const`， `const` 在参数表后面，念作「这个函数不改对象」，禁止修改成员变量，修改的话编译就会失败
    virtual int evaluate() const = 0; // virtual
    virtual const char *kind() const { return "Expr"; }
};

struct Literal : Expr {
    int value;
    explicit Literal(int v) : value(v) {}
    int evaluate() const override { return value; }
    const char *kind() const override { return "Literal"; }
};

struct Negate : Expr {
    int value;
    explicit Negate(int v) : value(v) {}
    int evaluate() const override { return -value; }
    const char *kind() const override { return "Negate"; }
};

int main()
{
    Literal lit(5);
    Negate neg(5);
    const Expr *a = &lit;
    const Expr *b = &neg;

    FD_CHECK(a->evaluate() == 5, "virtual call on Literal");
    FD_CHECK(b->evaluate() == -5, "virtual call on Negate");
    FD_CHECK(std::string(a->kind()) == "Literal", "override replaces base kind()");
    FD_CHECK(std::string(b->kind()) == "Negate", "different dynamic type, different kind");
    FD_CHECK(a != b, "two objects, two addresses");
    return 0;
}
