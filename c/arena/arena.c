//
// Created by even on 2026/9/18.
//
#include "arena.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

typedef struct Arena {
    size_t  capacity;  /* 这块一共多少字节，就是 create 时的 block_size */
    size_t  offset;    /* 已经用到哪了：下一个空闲字节相对 base 的下标 */
    char pad;      /* 故意插在 FAM 前：base 的地址不再碰巧是 8 的倍数 */
    char base[]; /* 那一大块的起点，malloc(block_size) 得来的 */
} Arena;

Arena *arena_create(size_t block_size) {
    // 需要分配 sizeof(Arena) + block_size，sizeof(Arena) 是给 capacity 和 offset 用的
    Arena * arena = malloc(sizeof(Arena) + block_size);
    if (arena == NULL) {
        return NULL;
    }
    arena ->capacity = block_size;
    // offset 初始化时offset应该是第一个对齐位
    size_t align = _Alignof(max_align_t);
    uintptr_t addr = (uintptr_t) arena->base; //获取base的首地址
    uintptr_t aligned = (addr + align - 1) / align * align; //获取 base 第一次对齐的首地址
    arena ->offset= (size_t)(aligned - addr);
    // char base[] 放在结构体最后是柔性数组，结构体会把最后一个字段之后到这个结构体的内存空间分配给它，所以这里不用对char base[] 进行内存分配操作
    /*
     * capacity 8字节
     * offset 8字节
     * pad 1字节
     * base 剩下的字节，即 block_size ;由于 align = 8，所以base首字节没有对齐，所以offset初始化时直接对齐，而不是从0开始
     */
    return arena;
}

void  *arena_alloc(Arena *a, size_t size) {
    // 需要先对齐，再分配； align 这台机器为8
    size_t align = _Alignof(max_align_t);
    // 假设现在 offset 是 1，则要补齐7，其实 就是 aligned = offset + (align - offset % align) % align;
    // 最后再 % align 是因为如果offset正好在align倍数上时，没有% align 就会多跳align
    // (addr + align - 1) / align 会去掉余数，可以得到在第几格，然后 * align 就得到了补齐后的首位了
    uintptr_t addr = (uintptr_t)(a->base + a->offset);
    uintptr_t aligned = (addr + align - 1) / align * align;
    if ((char *) (aligned + size) > (a->base + a ->capacity)) {
        return NULL;
    }
    // offset = 分配空间的末位 - base的起始位
    a ->offset = (size_t)((aligned + size) - (uintptr_t)a->base);
    //返回的是分配的首位，a ->base 表示 base的首位，aligned 表示补齐后的首位，即分配首位，分配首位 + offset 等于分配的内存
    return (void *)aligned;
}

void  arena_reset(Arena *arena) {
    // offset 初始化时offset应该是第一个对齐位
    size_t align = _Alignof(max_align_t);
    uintptr_t addr = (uintptr_t) arena->base; //获取base的首地址
    uintptr_t aligned = (addr + align - 1) / align * align; //获取 base 第一次对齐的首地址
    arena ->offset= (size_t)(aligned - addr);
}

void  arena_destroy(Arena *a) {
    free(a);
}

size_t arena_capacity(Arena *a) {
    if (a == NULL) {
        return 0;
    }
    return a->capacity;
}

size_t arena_offset(Arena *a) {
    if (a == NULL) {
        return 0;
    }
    return a->offset;
}

