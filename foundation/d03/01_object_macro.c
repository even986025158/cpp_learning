/*
 * 本文件只证明：对象宏是编译前的文本替换，替换后就是普通常量/类型名。
 * 不证明：函数宏副作用、多语句宏、条件编译的全部分支。
 */
#include "check.h"

#define BUCKET_COUNT 8
#define NODE_TAG_INT 1

int main(void)
{
    int buckets[BUCKET_COUNT];
    FD_CHECK(sizeof(buckets) / sizeof(buckets[0]) == 8, "BUCKET_COUNT became 8");
    FD_CHECK(NODE_TAG_INT == 1, "tag macro is the integer 1");
#ifndef BUCKET_COUNT
    FD_CHECK(0, "BUCKET_COUNT should be defined");
#endif
    printf("note: clang -E -P this file to see the macros disappear\n");
    return 0;
}
