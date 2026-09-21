/*
 * 本文件只证明：栈上对象离开作用域就自动析构；同一块里后造的先拆；函数返回也会拆局部对象。
 * 不证明：delete / 堆对象、虚析构、异常栈展开。
 */
#include "check.h"
#include <string>

struct Probe {
    const char *name;
    std::string *log;

    Probe(const char *n, std::string *l) : name(n), log(l)
    {
        *log += name;
        *log += '+';
    }

    ~Probe()
    {
        *log += name;
        *log += '-';
    }
};

static void make_one(std::string *log)
{
    Probe t("T", log);
} /* T 在这里被拆掉，不必写 t.~Probe() */

int main()
{
    std::string log;
    {
        Probe a("A", &log);
        {
            Probe b("B", &log);
            FD_CHECK(log == "A+B+", "ctors run outer then inner");
        } /* 内层结束：先拆 B */
        FD_CHECK(log == "A+B+B-", "inner dtor runs at inner brace");
        make_one(&log);
        FD_CHECK(log == "A+B+B-T+T-", "function-local object dies on return");
    } /* 外层结束：再拆 A */
    FD_CHECK(log == "A+B+B-T+T-A-", "dtor order is reverse of construction");
    return 0;
}
