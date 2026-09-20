/*
 * 本文件只证明：要改调用方的指针变量，必须传入那份指针的地址（T**）。
 * 不证明：真正的二维数组、函数指针、所有权图。
 */
#include "check.h"
#include <stdlib.h>

static void set_int(int *out, int value)
{
    *out = value;
}

static void set_ptr(int **out, int *target)
{
    *out = target;
}

static int give_heap(int **out, int value)
{
    int *p = malloc(sizeof(*p));
    if (p == NULL) {
        return 0;
    }
    *p = value;
    *out = p;
    return 1;
}

int main(void)
{
    int x = 0;
    set_int(&x, 7);
    FD_CHECK(x == 7, "int* writes the caller's int");

    int storage = 99;
    int *p = NULL;
    set_ptr(&p, &storage);
    FD_CHECK(p == &storage, "int** writes the caller's pointer variable");
    FD_CHECK(*p == 99, "that pointer now refers to storage");

    int *heap = NULL;
    FD_CHECK(give_heap(&heap, 123) == 1, "allocator reports success");
    FD_CHECK(heap != NULL, "caller received a non-NULL heap pointer");
    FD_CHECK(*heap == 123, "heap cell holds the written value");
    free(heap);
    heap = NULL;
    FD_CHECK(heap == NULL, "caller nulls the pointer after free");
    return 0;
}
