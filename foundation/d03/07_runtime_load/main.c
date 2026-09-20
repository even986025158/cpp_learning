/*
 * 本文件只证明：dlopen/dlsym 能在运行时按路径和名字找到从未链接过的函数。
 * 不证明：插件热更新、符号版本、把 plugin.c 静态链进本程序。
 */
#include "check.h"

#include <dlfcn.h>
#include <string.h>

typedef int (*PluginFn)(int, int);

static PluginFn load_fn(void *handle, const char *name)
{
    void *sym = dlsym(handle, name);
    PluginFn fn = NULL;
    if (sym == NULL) {
        return NULL;
    }
    memcpy(&fn, &sym, sizeof(fn));
    return fn;
}

int main(int argc, char **argv)
{
    FD_CHECK(argc >= 2, "plugin path is passed as argv[1]");
    const char *path = argv[1];

    void *handle = dlopen(path, RTLD_NOW);
    FD_CHECK(handle != NULL, "dlopen succeeds on the CMake-provided path");

    PluginFn add = load_fn(handle, "plugin_add");
    PluginFn mul = load_fn(handle, "plugin_mul");
    FD_CHECK(add != NULL, "dlsym finds plugin_add");
    FD_CHECK(mul != NULL, "dlsym finds plugin_mul");
    FD_CHECK(add != mul, "the two symbols are different functions");
    FD_CHECK(add(20, 22) == 42, "plugin_add(20, 22) == 42");
    FD_CHECK(mul(6, 7) == 42, "plugin_mul(6, 7) == 42");

    dlclose(handle);
    return 0;
}
