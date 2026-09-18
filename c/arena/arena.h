//
// Created by even on 2026/9/18.
//
#include <stddef.h>

typedef struct Arena Arena;

Arena *arena_create(size_t block_size);
void  *arena_alloc(Arena *a, size_t size);   // 只移动指针，不真分配
void  arena_reset(Arena *a);                 // 整批释放，不还给操作系统，只把 offset 拨回 0
void  arena_destroy(Arena *a);             // 真正 free 这一大块
size_t arena_capacity(Arena *a);
size_t arena_offset(Arena *a);


