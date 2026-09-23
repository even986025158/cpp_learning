/*
 * 本文件只证明：决定“有没有覆盖成功”的是签名是否一致，不是有没有写 override。
 * 三组基类完全相同，派生类每次相对上一组只改一个地方：
 *   baseline        : int evaluate() const override   正确写法
 *   without_override: int evaluate() const            相对 baseline 只去掉 override
 *   missing_const   : int evaluate()                  相对 without_override 只去掉尾随 const
 * （missing_const 不能直接挂在 baseline 下面：去掉 const 还留着 override 会编译失败，
 *   那一组见 evidence_abstract_override.sh 的 mismatch_with_override。）
 * 不证明：= 0 拦住实例化、override 写错时的编译诊断（都是编译失败，见同一个脚本）。
 */
#include "check.h"

namespace baseline {
struct Base {
    virtual int evaluate() const { return 0; }
};

struct Derived : Base {
    int evaluate() const override { return 5; }
};
} // namespace baseline

namespace without_override {
struct Base {
    virtual int evaluate() const { return 0; }
};

struct Derived : Base {
    int evaluate() const { return 5; } // 唯一旋钮：去掉 override
};
} // namespace without_override

namespace missing_const {
struct Base {
    virtual int evaluate() const { return 0; }
};

struct Derived : Base {
    int evaluate() { return 5; } // 唯一旋钮：在上一组基础上去掉尾随 const
};
} // namespace missing_const

int main()
{
    baseline::Derived baseline_object;
    const baseline::Base &baseline_base_name = baseline_object;
    FD_CHECK(baseline_object.evaluate() == 5, "baseline: derived name gives 5");
    FD_CHECK(baseline_base_name.evaluate() == 5, "baseline: base name dispatches to the derived function");

    // 派生类在实现基类函数时没有写override，还是可以覆盖对应的函数，调用时调用的还是派生类的函数，
    // 写override只是让编译器可以在编译器检查出 对应函数 覆写的 签名是否对了，不对会提前报错
    without_override::Derived without_override_object;
    const without_override::Base &without_override_base_name = without_override_object;
    FD_CHECK(without_override_object.evaluate() == 5, "without override: derived name gives 5");
    FD_CHECK(without_override_base_name.evaluate() == 5,
             "without override: same as baseline, the keyword changes no dispatch");

    // 派生类在实现基类函数时没有写const，不可以覆盖对应的函数，调用时调用的还是基类的函数；
    // 编译期会先通过基类查找对应的派生类有没有覆写对应的函数，没有的话就编译为基类对应的函数。有的话编译成派生类对应的函数。
    missing_const::Derived missing_const_object;
    const missing_const::Base &missing_const_base_name = missing_const_object;
    FD_CHECK(missing_const_object.evaluate() == 5, "missing const: derived name still gives 5");
    FD_CHECK(missing_const_base_name.evaluate() == 0,
             "missing const: signature differs, so the base function is still in use");
    return 0;
}
