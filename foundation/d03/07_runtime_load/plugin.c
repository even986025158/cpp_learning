/*
 * 本文件只证明：一个独立编译的动态库能导出按名字查找的函数。
 * 不证明：与主程序静态链接（CMake 故意不把本文件链进 main）。
 */
#ifdef _WIN32
#define FD_EXPORT __declspec(dllexport)
#else
#define FD_EXPORT
#endif

FD_EXPORT int plugin_add(int a, int b)
{
    return a + b;
}

FD_EXPORT int plugin_mul(int a, int b)
{
    return a * b;
}
