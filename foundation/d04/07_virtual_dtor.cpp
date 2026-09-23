/*
 * 本文件只证明：基类有 virtual 析构时，经基类指针 delete 会跑派生析构，顺序是派生→基类；
 * 以及栈上对象按变量自己的类型析构，不经过基类指针。
 * 不证明：缺虚析构的未定义行为（那份写在 /tmp，不进默认构建）、unique_ptr 的所有权。
 */
#include "check.h"
#include <string>

struct Expr {
    std::string *log;
    explicit Expr(std::string *l) : log(l) {}
    virtual ~Expr() { *log += "~Expr "; }   /* (a) 虚析构：允许经基类指针销毁派生对象 */
    virtual int evaluate() const = 0;
};

struct Literal : Expr {
    int value;
    Literal(int v, std::string *l) : Expr(l), value(v) {}  /* (b) 先把基类子对象构造好 */
    ~Literal() override { *log += "~Literal "; }
    int evaluate() const override { return value; }
};

int main()
{
    std::string log;

    Expr *p = new Literal(5, &log);          /* (c) 指针类型是 Expr*，对象是 Literal */
    FD_CHECK(p->evaluate() == 5, "virtual call reaches Literal");
    delete p;                                /* (d) 虚析构在这里发力 */
    FD_CHECK(log == "~Literal ~Expr ", "derived dtor runs first, then base");

    log.clear();
    {
        Literal on_stack(7, &log);           /* (e) 栈上对象：类型写死，不经基类指针 */
        FD_CHECK(on_stack.evaluate() == 7, "stack object evaluates");
    }
    FD_CHECK(log == "~Literal ~Expr ", "stack object destroys the same way");
    return 0;
}
