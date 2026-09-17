/* 知识点 2: 数组退化为指针 (array decay)
 *
 * 核心心法: 数组名一旦作为函数参数传递,就"退化"成指向首元素的指针,
 *          长度信息彻底丢失。所以 C 里传数组必须同时传长度。
 */
#include <stdio.h>

/* 看起来像数组,编译器实际当成 int* 处理 */
void f_looks_like_array(int arr[10])
{
    // sizeof(arr) 这里的 arr 是 int * 指针，不是数组了，所以sizeof(arr) 结果为 8，即指针大小
    printf("  函数内 sizeof(arr)      = %zu  <- 指针大小!\n", sizeof(arr));
    printf("  函数内 sizeof(arr)/sizeof(arr[0]) = %zu  <- 算出来是错的\n",
           sizeof(arr) / sizeof(arr[0]));
}

/* 正确写法: 显式传长度 */
void f_correct(int *arr, size_t n)
{
    int sum = 0;
    for (size_t i = 0; i < n; i++)
        sum += arr[i];
    printf("  正确遍历 n=%zu, sum = %d\n", n, sum);
}

/* 唯一不退化的办法: 传数组的指针 int (*)[10] */
void f_no_decay(int (*arr)[10])
{
    printf("  传 int(*)[10]: sizeof(*arr) = %zu  <- 长度保住了\n", sizeof(*arr));
    printf("  元素个数 = %zu\n", sizeof(*arr) / sizeof((*arr)[0]));
}

int main(void)
{
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    printf("=== 2.1 函数外 vs 函数内 ===\n");
    printf("  函数外 sizeof(arr)      = %zu  <- 10 * 4 = 40,真实总字节\n", sizeof(arr));
    printf("  函数外 元素个数         = %zu\n", sizeof(arr) / sizeof(arr[0]));
    f_looks_like_array(arr);

    printf("\n=== 2.2 正确做法: 带上长度 ===\n");
    f_correct(arr, sizeof(arr) / sizeof(arr[0]));

    printf("\n=== 2.3 不退化的特殊写法 ===\n");
    f_no_decay(&arr);

    printf("\n=== 2.4 退化的等价性证明 ===\n");
    printf("  arr        = %p\n", (void *) arr);
    printf("  &arr[0]    = %p   <- 数组名 == 首元素地址\n", (void *) &arr[0]);
    printf("  &arr       = %p   <- 值相同,但类型是 int(*)[10]\n", (void *) &arr);
    printf("  arr + 1    = %p   <- +4 字节 (步长 sizeof(int))\n", (void *) (arr + 1));
    printf("  &arr + 1   = %p   <- +40 字节 (步长 sizeof(int[10]))\n", (void *) (&arr + 1));

    printf("\n=== 2.5 字符串同理 ===\n");
    char s1[] = "hello";        /* 数组: 6 字节,含 '\\0' */
    const char *s2 = "hello";   /* 指针: 指向只读区 */
    printf("  char s1[] = \"hello\"  -> sizeof = %zu (含结尾 \\0)\n", sizeof(s1));
    printf("  const char *s2       -> sizeof = %zu (指针大小)\n", sizeof(s2));
    return 0;
}
