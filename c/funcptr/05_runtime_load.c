/* ============================================================
 * 第 5 课：★ 这才是 PG 绕不过去的那堵墙
 * ============================================================
 *
 * 前四课你可能一直有个疑问（这个疑问是对的）：
 *
 *   "不就是加法吗？编译时写死 a + b 不就完了，
 *    绕一大圈函数指针图什么？"
 *
 * 答案：如果 PG 只需要执行它自己那些内建函数，那确实【不需要】这套东西。
 *
 * 真正逼出这套机制的是这个需求：
 *
 *     CREATE FUNCTION my_func(int, int) RETURNS int
 *     AS '/path/to/my_lib.so', 'plugin_add' LANGUAGE C;
 *
 * 用户在**数据库已经跑起来之后**，往里加一个新函数。
 * 那个 .so 是今天下午才编译出来的，而 postgres 这个二进制是去年编译的。
 *
 * 去年编译 postgres 时，链接器不可能解析一个今天才存在的符号。
 * 【这是静态链接在物理上做不到的事，不是设计偏好问题。】
 *
 * 这一课就亲手做一遍这件事。
 *
 * ------------------------------------------------------------
 * 怎么跑（两步）：
 *
 *   cd <本文件所在目录>
 *   # 1. 先编译主程序（注意 macOS 不需要 -ldl，Linux 需要）
 *   cc -std=c11 -Wall -o /tmp/load 05_runtime_load.c
 *   # 2. 再编译那个"事后出现"的库
 *   cc -std=c11 -shared -fPIC -o /tmp/myadd.so plugin/myadd.c
 *   # 3. 运行
 *   /tmp/load /tmp/myadd.so
 *
 * ★ 请特意留意第 1 步和第 2 步的先后：
 *   主程序先编译完，库后编译，主程序照样能调到它。
 * ------------------------------------------------------------
 */
#include <stdio.h>
#include <stdint.h>
#include <dlfcn.h>

/* 主程序里只有这个类型约定，没有 plugin_add 的任何声明或实现。
 * 你可以全文搜索 "plugin_add" —— 除了下面那个字符串，一处都没有。 */
typedef int64_t (*PluginFn)(int64_t, int64_t);

int main(int argc, char **argv)
{
    const char *libpath = (argc > 1) ? argv[1] : "/tmp/myadd.so";

    printf("=== 5.1 装载一个编译时并不存在的库 ===\n");
    printf("  要装载：%s\n", libpath);

    /* dlopen：在运行时把一个动态库映射进本进程的地址空间。
     * ★ PG 里对应 fmgr.c:388 的 load_external_function()。 */
    void *handle = dlopen(libpath, RTLD_NOW);
    if (handle == NULL) {
        printf("\n  ✗ 装载失败：%s\n", dlerror());
        printf("\n  多半是还没编译那个库。先执行：\n");
        printf("    cc -std=c11 -shared -fPIC -o /tmp/myadd.so plugin/myadd.c\n");
        return 1;
    }
    printf("  ✓ 装载成功\n");

    printf("\n=== 5.2 按【名字字符串】找到函数的编号 ===\n");

    /* 注意参数是个字符串。
     * 这意味着函数名可以来自配置文件、来自用户输入、
     * ★ 在 PG 里来自系统表 pg_proc.prosrc 这一列。 */
    const char *funcname = "plugin_add";

    /* dlsym 返回 void*，要转成函数指针才能调用。
     * 这个转换在标准 C 里是灰色地带，POSIX 明确允许，
     * 所以下面用一次中转来避免编译器告警。 */
    void *sym = dlsym(handle, funcname);
    if (sym == NULL) {
        printf("  ✗ 找不到符号 %s：%s\n", funcname, dlerror());
        dlclose(handle);
        return 1;
    }

    PluginFn fn;
    *(void **) (&fn) = sym;     /* POSIX 推荐的转换写法 */

    printf("  用名字 \"%s\" 查到了编号 %p\n", funcname, sym);

    printf("\n=== 5.3 调用它 ===\n");
    printf("  fn(20, 22) = %lld\n", (long long) fn(20, 22));

    printf("\n=== 5.4 换个名字，同一行代码调到另一个实现 ===\n");
    sym = dlsym(handle, "plugin_mul");
    if (sym != NULL) {
        *(void **) (&fn) = sym;
        printf("  换成 \"plugin_mul\" 后， fn(20, 22) = %lld\n", (long long) fn(20, 22));
    }

    dlclose(handle);

    printf("\n============================================================\n");
    printf("★ 现在回头看这个事实：\n");
    printf("  本文件从头到尾【没有】plugin_add 的声明，也没和它链接，\n");
    printf("  编译本文件时那个 .so 甚至可以还不存在 —— 但它被调用了。\n");
    printf("\n  静态写死做不到这件事。函数指针 + dlopen 是 C 里唯一的办法。\n");
    printf("  ★ PG 的 fmgr 机制，内核就是这三步：\n");
    printf("     1. 从系统表 pg_proc 读出「库路径 + 函数名」\n");
    printf("     2. dlopen + dlsym 拿到编号，存进 FmgrInfo.fn_addr\n");
    printf("     3. 执行器调用 fn_addr(fcinfo)\n");
    printf("\n  而 PG 做了一个额外的决定（下一步回任务卡看）：\n");
    printf("  既然外部函数必须走这条路，那就让【内建函数也走同一条路】，\n");
    printf("  连 1+2 的加法也不例外 —— 这样执行器只需要一套代码。\n");
    printf("============================================================\n");

    return 0;
}
