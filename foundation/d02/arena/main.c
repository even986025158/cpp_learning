/*
 * 本文件只证明：arena 分配成功、同块 bump、可写、reset 复用起点、超容量失败且不改 used。
 * 不证明：reset 后读旧指针指向的内容（只比较地址是否相等）。
 */
#include "arena.h"
#include "check.h"

#include <stdint.h>
#include <string.h>

int main(void)
{
    Arena *a = arena_create(4096);
    FD_CHECK(a != NULL, "arena_create(4096)");
    FD_CHECK(arena_capacity(a) == 4096, "capacity is 4096");
    FD_CHECK(arena_used(a) == 0, "used starts at 0");

    unsigned char *p1 = arena_alloc(a, 16);
    unsigned char *p2 = arena_alloc(a, 8);
    FD_CHECK(p1 != NULL && p2 != NULL, "two small allocs succeed");
    FD_CHECK(p2 > p1, "second bump is after the first in the same block");
    FD_CHECK((size_t)(p2 - p1) >= 16, "gap covers the first 16-byte request");
    FD_CHECK(((uintptr_t)p1 % _Alignof(max_align_t)) == 0, "p1 is max-aligned");
    FD_CHECK(((uintptr_t)p2 % _Alignof(max_align_t)) == 0, "p2 is max-aligned");

    memset(p1, 0xAA, 16);
    FD_CHECK(p1[0] == 0xAA && p1[15] == 0xAA, "returned memory is writable");

    size_t used_before_fail = arena_used(a);
    unsigned char *too_big = arena_alloc(a, 10000);
    FD_CHECK(too_big == NULL, "oversize request returns NULL");
    FD_CHECK(arena_used(a) == used_before_fail, "failed alloc does not move used");

    unsigned char *one = arena_alloc(a, 1);
    FD_CHECK(one != NULL, "1-byte alloc after a failure still works");
    unsigned char *aligned_after = arena_alloc(a, 8);
    FD_CHECK(aligned_after != NULL, "alloc after 1-byte request succeeds");
    FD_CHECK(((uintptr_t)aligned_after % _Alignof(max_align_t)) == 0,
             "allocator realigns after a 1-byte bump");

    arena_reset(a);
    FD_CHECK(arena_used(a) == 0, "reset returns used to 0");
    unsigned char *p3 = arena_alloc(a, 16);
    FD_CHECK(p3 == p1, "reset then alloc(16) reuses the first address");
    /* 不读 *p1：reset 后旧指针的值仍可能相等，但内容契约已经作废。 */

    arena_destroy(a);
    FD_CHECK(arena_capacity(NULL) == 0, "queries on NULL are safe");
    return 0;
}
