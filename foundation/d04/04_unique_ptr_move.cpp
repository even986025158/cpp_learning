/*
 * 本文件只证明：unique_ptr 独占所有权；std::move 之后源变成 nullptr，目标接管对象。
 * 不证明：shared_ptr、自定义删除器、在 move 后解引用源指针。
 */
#include "check.h"
#include <memory>

int main()
{
    auto a = std::make_unique<int>(5);
    int *raw = a.get();
    FD_CHECK(a != nullptr, "make_unique owns an int");
    FD_CHECK(*a == 5, "owned value is 5");

    auto b = std::move(a);
    FD_CHECK(a == nullptr, "move leaves the source empty");
    FD_CHECK(b != nullptr, "destination now owns the object");
    FD_CHECK(b.get() == raw, "same heap object, ownership transferred");
    FD_CHECK(*b == 5, "value is unchanged by the move");
    return 0;
}
