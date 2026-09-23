/*
 * 本文件只证明：vector 的元素连续存放。
 * 不证明：自动扩容后的地址失效、allocator、lambda、模板。
 */
#include "check.h"
#include <vector>

int main()
{
    std::vector<int> xs = {1, 2, 3, 4};
    FD_CHECK(xs.size() == 4, "vector size is 4");
    FD_CHECK(xs.data() + 3 == &xs[3], "elements are contiguous");
    return 0;
}
