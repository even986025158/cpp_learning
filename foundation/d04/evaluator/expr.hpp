/*
 * 本文件只证明：Expr 树用 unique_ptr 表示所有权，基类需要 virtual 析构，Add/Literal 用 override。
 * 不证明：parser、完整运算符集合。Multiply 只留 TODO，正常 build 不得依赖它。
 */
#pragma once

#include <memory>

class Expr {
public:
    virtual ~Expr();
    virtual int evaluate() const = 0;
    virtual const char *kind() const = 0;
};

class Literal : public Expr {
public:
    explicit Literal(int value);
    ~Literal() override;
    int evaluate() const override;
    const char *kind() const override;

private:
    int value_;
};

class Add : public Expr {
public:
    Add(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right);
    ~Add() override;
    int evaluate() const override;
    const char *kind() const override;

private:
    std::unique_ptr<Expr> left_;
    std::unique_ptr<Expr> right_;
};

// TODO(learner): 增加 Multiply，模仿 Add：
//   class Multiply : public Expr {
//       two unique_ptr<Expr> children;
//       int evaluate() const override { return left * right; }
//   };
// 写完整再接入 main。不要在这里留一个半成品 class —— 未定义的虚函数会让链接失败。
