#!/usr/bin/env bash
# L2 编译诊断：尾随 const 是编译器强制的限制，不是命名约定。
# 基线和 11_trailing_const.cpp 同构；每组相对基线只改一个旋钮。失败本身是证据，不进默认绿测。
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

# ---- 基线：和 11_trailing_const.cpp 一致。const 成员只读，const 对象调用。----
expect_ok trailing_const_baseline '
struct Readable {
    int value = 5;
    int touch() const { return value; }
};
int main() { const Readable object; return object.touch(); }
'

# ---- 组 1：唯一旋钮是函数体，由“读”改成“写”。证明 const 是限制，不是规范。----
expect_error const_member_cannot_write '
struct Readable {
    int value = 5;
    int touch() const { value = 9; return value; }
};
int main() { const Readable object; return object.touch(); }
'

# ---- 组 2：唯一旋钮是去掉尾随 const。证明 const 对象不接受非 const 成员函数。----
expect_error const_object_rejects_non_const_member '
struct Readable {
    int value = 5;
    int touch() { return value; }
};
int main() { const Readable object; return object.touch(); }
'

# ---- 组 3：相对组 1 只把对象从 const 改成非 const。证明组 1 的失败来自函数上的 const，
#      不是来自对象上的 const。----
expect_error non_const_object_still_cannot_write_in_const_member '
struct Readable {
    int value = 5;
    int touch() const { value = 9; return value; }
};
int main() { Readable object; return object.touch(); }
'
