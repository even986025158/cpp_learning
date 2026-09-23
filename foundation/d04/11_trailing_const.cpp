/*
 * 本文件只证明：成员函数尾随 const 后，可以通过 const 对象调用。
 * 不证明：返回类型中的 const、override 签名错误。
 */
#include "check.h"

struct Readable {
    int value = 5;
    char *name;
    int read() const { return value; }
    //bool set(char *name) const  {this->name = name;} // 编译失败，`const` 在参数表后面，念作「这个函数不改对象」，禁止修改成员变量
};

int main()
{
    const Readable object{};
    FD_CHECK(object.read() == 5, "trailing const permits a call through a const object");
    return 0;
}
