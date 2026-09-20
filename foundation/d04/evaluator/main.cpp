/*
 * 本文件只证明：手工 unique_ptr 构图、move 后源为空、Add/Literal 求值正确。
 * 不证明：从字符串 parse、乘法和减法（留给 TODO）。
 */
#include "../../common/check.h"
#include "expr.hpp"

#include <memory>
#include <string>

int main()
{
    auto left = std::make_unique<Literal>(2);
    auto right = std::make_unique<Literal>(3);
    FD_CHECK(left != nullptr && right != nullptr, "literals constructed");

    auto add = std::make_unique<Add>(std::move(left), std::move(right));
    FD_CHECK(left == nullptr, "move emptied the left unique_ptr");
    FD_CHECK(right == nullptr, "move emptied the right unique_ptr");
    FD_CHECK(add != nullptr, "Add took ownership");
    FD_CHECK(std::string(add->kind()) == "Add", "root kind is Add");
    FD_CHECK(add->evaluate() == 5, "2 + 3 == 5");

    auto deeper = std::make_unique<Add>(
        std::make_unique<Literal>(1),
        std::make_unique<Add>(std::make_unique<Literal>(2), std::make_unique<Literal>(3)));
    FD_CHECK(deeper->evaluate() == 6, "1 + (2 + 3) == 6");
    return 0;
}
