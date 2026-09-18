//
// Created by even on 2026/9/18.
//
#include "arena.h"
#include <stdio.h>
#include <string.h>

int main(void) {

    Arena *a = arena_create(4096);
    char *p1 = arena_alloc(a, 16);
    char *p2 = arena_alloc(a, 8);
    printf("p1=%p p2=%p 差 %td 字节\n",
           (void *)p1, (void *)p2, p2 - p1);
    /* 差应 >= 16；两个都应非 NULL */
    memset(p1, 0xAA, 16);   /* 能写，说明不是账本地址 */
    arena_reset(a);
    char *p3 = arena_alloc(a, 16);
    printf("reset 后 p3=%p  (应与 p1 相同)\n", (void *)p3);
    char *p4 = arena_alloc(a, 10000);
    printf("过大请求 p4=%p  (应是 NULL)\n", (void *)p4);
    arena_destroy(a);
    a = NULL;

    printf("capacity : %zu, offset: %zu\n", arena_capacity(a), arena_offset(a));
    size_t capacity = arena_capacity(a);
    capacity = 100; // 这里不会改a的capacity，因为arena_capacity(a)返回的是值，不是引用，所以最后这里打印的还是0，不是100
    printf("capacity : %zu, offset: %zu\n", arena_capacity(a), arena_offset(a));
    return 0;

}
