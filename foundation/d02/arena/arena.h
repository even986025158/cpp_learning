/*
 * 本文件只证明：arena 的公开契约（create / alloc / reset / destroy + 容量查询）。
 * 不证明：多块链表、线程安全、与 PG MemoryContext 的完整对应。
 */
#ifndef FOUNDATION_ARENA_H
#define FOUNDATION_ARENA_H

#include <stddef.h>

typedef struct Arena Arena;

Arena *arena_create(size_t capacity);
void *arena_alloc(Arena *a, size_t size);
void arena_reset(Arena *a);
void arena_destroy(Arena *a);
size_t arena_capacity(const Arena *a);
size_t arena_used(const Arena *a);

#endif
