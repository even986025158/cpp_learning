#!/usr/bin/env bash
# L2 编译诊断。每组相对自己的基线只改一个旋钮；失败本身是证据，所以不进默认绿测。
set -u

expect_error() {
    local name="$1"
    local src="$2"
    echo "== $name（预期编译失败）=="
    if c++ -std=c++17 -fsyntax-only -x c++ - <<<"$src" 2>/tmp/fd_d04_"$name".err; then
        echo "UNEXPECTED: compiled"
        return 1
    fi
    cat /tmp/fd_d04_"$name".err
}

expect_ok() {
    local name="$1"
    local src="$2"
    echo "== $name（预期编译通过）=="
    if c++ -std=c++17 -fsyntax-only -x c++ - <<<"$src" 2>/tmp/fd_d04_"$name".err; then
        echo "ok: compiled"
    else
        echo "UNEXPECTED: failed"
        cat /tmp/fd_d04_"$name".err
        return 1
    fi
}

# ---- 组 1：= 0。基线可实例化，加上 = 0 之后不可以。唯一旋钮是 = 0。----
expect_ok pure_virtual_baseline '
struct Expr {
    virtual ~Expr() = default;
    virtual int evaluate() const { return 0; }
};
int main() { Expr object; }
'

expect_error pure_virtual_blocks_construction '
struct Expr {
    virtual ~Expr() = default;
    virtual int evaluate() const = 0;
};
int main() { Expr object; }
'

# ---- 组 2：签名不一致时，唯一旋钮是 override 关键字。----
expect_ok mismatch_without_override '
struct Base {
    virtual int evaluate() const { return 0; }
};
struct Derived : Base {
    int evaluate() { return 5; }
};
int main() { Derived object; }
'

expect_error mismatch_with_override '
struct Base {
    virtual int evaluate() const { return 0; }
};
struct Derived : Base {
    int evaluate() override { return 5; }
};
int main() { Derived object; }
'

# ---- 组 3：override 是否强制要求尾随 const。唯一旋钮是基类虚函数带不带 const。----
expect_ok override_on_non_const_virtual '
struct Base {
    virtual int evaluate() { return 0; }
};
struct Derived : Base {
    int evaluate() override { return 5; }
};
int main() { Derived object; }
'
