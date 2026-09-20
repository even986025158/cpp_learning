/*
 * 本文件只证明：对齐后的 bump 分配、容量不足不改游标、reset 只拨回起点、destroy 归还整块。
 * 不证明：reset 后继续解引用旧指针（那是调用方纪律，本文件不会这么做）。
 */
#include "arena.h"

#include <stdint.h>
#include <stdlib.h>

struct Arena {
    size_t capacity;
    size_t used;
    unsigned char *mem;
};

static size_t arena_align_up(size_t n, size_t align)
{
    return (n + (align - 1)) & ~(align - 1);
}

static int arena_fits(size_t used, size_t size, size_t capacity)
{
    if (size > capacity) {
        return 0;
    }
    return used <= capacity - size;
}

Arena *arena_create(size_t capacity)
{
    if (capacity == 0) {
        return NULL;
    }
    Arena *a = malloc(sizeof(*a));
    if (a == NULL) {
        return NULL;
    }
    a->mem = malloc(capacity);
    if (a->mem == NULL) {
        free(a);
        return NULL;
    }
    a->capacity = capacity;
    a->used = 0;
    return a;
}

void *arena_alloc(Arena *a, size_t size)
{
    if (a == NULL || size == 0) {
        return NULL;
    }
    size_t align = _Alignof(max_align_t);
    size_t start = arena_align_up(a->used, align);
    if (!arena_fits(start, size, a->capacity)) {
        return NULL;
    }
    a->used = start + size;
    return a->mem + start;
}

void arena_reset(Arena *a)
{
    if (a == NULL) {
        return;
    }
    a->used = 0;
}

void arena_destroy(Arena *a)
{
    if (a == NULL) {
        return;
    }
    free(a->mem);
    a->mem = NULL;
    a->capacity = 0;
    a->used = 0;
    free(a);
}

size_t arena_capacity(const Arena *a)
{
    return a == NULL ? 0 : a->capacity;
}

size_t arena_used(const Arena *a)
{
    return a == NULL ? 0 : a->used;
}
