/*
 * 本文件只证明：lambda 的 [&] 引用捕获可以修改外部变量。
 * 不证明：按值捕获、生命周期、std::function 的类型擦除。
 */
#include "check.h"
#include <algorithm>
#include <vector>

int main()
{
    const std::vector<int> values = {1, 2, 3, 4};
    int sum = 0;
    std::for_each(values.begin(), values.end(), [&](int value) { sum += value; });
    FD_CHECK(sum == 10, "[&] lambda modifies the external sum");
    return 0;
}
