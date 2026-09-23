/*
 * 本文件只证明：单参构造加不加 explicit，决定 int 能不能隐式转成这个类型。
 *   没有 explicit：int 可以当成 Money 用（传参、= 初始化都自动构造）。
 *   加了 explicit：这条隐式通道关闭，必须显式写出类型；但显式构造仍然可用。
 * 两个 namespace 的唯一旋钮就是构造函数前的 explicit。
 * 不证明：多参构造、explicit 转换运算符、static_cast 的其它规则。
 */
#include "check.h"
#include <type_traits>

namespace without_explicit {
struct Money {
    int value;
    Money(int input) : value(input) {}
};

static int read(Money object)
{
    return object.value;
}
} // namespace without_explicit

namespace with_explicit {
struct Money {
    int value;
    explicit Money(int input) : value(input) {} // 唯一旋钮：多了 explicit
};

static int read(Money object)
{
    return object.value;
}
} // namespace with_explicit

/* 编译期证据：is_convertible 问的是“能不能隐式转”，is_constructible 问的是“能不能显式造”。
 * 这四条只要有一条不成立，本文件就编译失败，所以它们和运行期断言一样是证据。 */
static_assert(std::is_convertible_v<int, without_explicit::Money>,
              "without explicit: int implicitly converts to Money");
static_assert(!std::is_convertible_v<int, with_explicit::Money>,
              "with explicit: the implicit path is closed");
static_assert(std::is_constructible_v<with_explicit::Money, int>,
              "with explicit: explicit construction still works");
static_assert(std::is_constructible_v<without_explicit::Money, int>,
              "control: both types can be constructed from int");

int main()
{
    /* A 组：没有 explicit，int 可以直接当 Money 用。 */
    FD_CHECK(without_explicit::read(7) == 7,
             "without explicit: a bare int is accepted where Money is expected");
    without_explicit::Money implicitly_built = 7; // 复制初始化，右边是 int
    FD_CHECK(implicitly_built.value == 7, "without explicit: Money object = 7 compiles and builds a Money");

    /* B 组：加了 explicit，必须自己写出类型；类型本身照常可用。 */
    FD_CHECK(with_explicit::read(with_explicit::Money(7)) == 7,
             "with explicit: the same call works once the conversion is written out");
    with_explicit::Money explicitly_built(7); // 直接初始化，不是 = 7
    FD_CHECK(explicitly_built.value == 7, "with explicit: direct initialization still works");

    /* 改一处（会编译失败，改完记下诊断再改回来）：
     *   with_explicit::read(7);
     *   with_explicit::Money boxed = 7;
     */
    return 0;
}
