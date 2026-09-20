/*
 * 本文件只证明：Add 递归求值子树；析构时 unique_ptr 子节点自动释放。
 * 不证明：Multiply（见 expr.hpp 的 TODO）、异常安全的复杂树改写。
 */
#include "expr.hpp"

#include <cstdio>
#include <utility>

Expr::~Expr() = default;

Literal::Literal(int value) : value_(value) {}

Literal::~Literal()
{
    std::printf("dtor %s\n", kind());
}

int Literal::evaluate() const
{
    return value_;
}

const char *Literal::kind() const
{
    return "Literal";
}

Add::Add(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right)
    : left_(std::move(left)), right_(std::move(right))
{
}

Add::~Add()
{
    std::printf("dtor %s\n", kind());
}

int Add::evaluate() const
{
    return left_->evaluate() + right_->evaluate();
}

const char *Add::kind() const
{
    return "Add";
}
