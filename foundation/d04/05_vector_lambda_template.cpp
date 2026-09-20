/*
 * 本文件只证明：vector 保存连续元素、lambda 能当回调、函数模板按类型实例化。
 * 不证明：SFINAE、完美转发、自定义 allocator。
 */
#include "check.h"
#include <algorithm>
#include <vector>

template <typename T>
T identity(T x)
{
    return x;
}

int main()
{
    FD_CHECK(identity(7) == 7, "identity<int>");
    FD_CHECK(identity(3L) == 3L, "identity<long>");

    std::vector<int> xs = {1, 2, 3, 4};
    FD_CHECK(xs.size() == 4, "vector size is 4");
    FD_CHECK(xs.data() + 3 == &xs[3], "elements are contiguous");

    int sum = 0;
    std::for_each(xs.begin(), xs.end(), [&](int v) { sum += v; });
    FD_CHECK(sum == 10, "lambda accumulated 1+2+3+4");

    auto evens = 0;
    for (int v : xs) {
        evens += (v % 2 == 0) ? 1 : 0;
    }
    FD_CHECK(evens == 2, "two even numbers");
    return 0;
}
