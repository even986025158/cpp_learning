/* ============================================================
 * 第 4 课：想把"所有函数"塞进一张表，签名就必须统一
 * ============================================================
 *
 * 第 3 课那张表能成立，是因为三个函数**长得一模一样**：
 * 都是 int(int, int)。
 *
 * 可是真实数据库里的函数五花八门：
 *   int4pl(int, int) -> int          两个整数
 *   abs(int) -> int                  一个参数
 *   substring(text,int,int) -> text  三个参数，还是别的类型
 *   now() -> timestamp               零个参数
 *
 * 这一课就撞一次那堵墙，然后看 C 语言里唯一的出路是什么。
 * 撞完你就明白 PG_FUNCTION_ARGS 为什么长那样了 —— 它不是设计得花哨，是被逼的。
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* ---------- 先看看墙有多硬 ---------- */

int  my_add(int a, int b)        { return a + b; }
int  my_abs(int a)               { return a < 0 ? -a : a; }

/* 试试把它们放进同一个数组：
 *
 *     int (*table[2])(int, int) = { my_add, my_abs };
 *
 * 编译器会直接报错（类型不兼容）。把上面这行取消注释试一次，
 * 亲眼看到报错比记住结论管用。
 *
 * C 是静态类型语言：数组里每个元素的类型必须完全一致。
 * 参数个数不同 = 类型不同 = 塞不进同一个数组。
 */

/* ---------- 出路：把"参数"打包成一个东西 ---------- */

/* 既然参数个数不能变，那就让所有函数都只收【一个】参数：
 * 一个装着"实际参数们"的盒子。
 *
 * ★ PG 就是这么干的。这个盒子在 PG 里叫 FunctionCallInfo，
 *   而 PG_FUNCTION_ARGS 这个宏，展开后就是 "FunctionCallInfo fcinfo" 这句参数声明。
 */
#define MAX_ARGS 4

typedef struct {
    int nargs;              /* 实际传了几个 */
    int64_t args[MAX_ARGS]; /* 值都统一存成 64 位整数 —— PG 里这叫 Datum */
} CallInfo;

/* 现在所有函数签名都一样了：吃一个 CallInfo*，还一个 int64_t。
 * ★ 对照 PG：typedef Datum (*PGFunction)(FunctionCallInfo fcinfo);
 *   一模一样的思路。 */
typedef int64_t (*UniformFn)(CallInfo *);

/* 为了写起来不那么难看，也定义两个取参数的宏。
 * ★ 对照 PG 的 PG_GETARG_INT32(n) / PG_RETURN_INT32(x)。 */
#define GETARG_INT(ci, n)  ((int) (ci)->args[n])
#define RETURN_INT(x)      return (int64_t) (x)

/* 同样两个函数，改写成统一签名的样子 */
int64_t u_add(CallInfo *fcinfo)
{
    int a = GETARG_INT(fcinfo, 0);
    int b = GETARG_INT(fcinfo, 1);
    RETURN_INT(a + b);
}

int64_t u_abs(CallInfo *fcinfo)
{
    int a = GETARG_INT(fcinfo, 0);
    RETURN_INT(a < 0 ? -a : a);
}

int64_t u_now(CallInfo *fcinfo)
{
    (void) fcinfo;              /* 零参数，但签名照样得收这个盒子 */
    RETURN_INT(1758000000);
}

/* 一张表，装下参数个数完全不同的三个函数 */
typedef struct {
    const char *name;
    UniformFn   fn;
    int         expected_nargs;
} Entry;

int main(void)
{
    Entry catalog[] = {
        { "add", u_add, 2 },
        { "abs", u_abs, 1 },
        { "now", u_now, 0 },
    };
    const int n = (int) (sizeof(catalog) / sizeof(catalog[0]));

    printf("=== 4.1 参数个数不同的函数，现在进了同一张表 ===\n");

    /* 三次调用，写法完全一样 */
    CallInfo ci;

    ci.nargs = 2; ci.args[0] = 10; ci.args[1] = 3;
    printf("  add(10, 3) = %lld\n", (long long) catalog[0].fn(&ci));

    ci.nargs = 1; ci.args[0] = -42;
    printf("  abs(-42)   = %lld\n", (long long) catalog[1].fn(&ci));

    ci.nargs = 0;
    printf("  now()      = %lld\n", (long long) catalog[2].fn(&ci));

    printf("\n  ★ 三次调用点的代码形状完全相同：catalog[i].fn(&ci)\n");
    printf("     参数个数的差异被藏进了盒子里。\n");

    printf("\n=== 4.2 按名字查表再调用（完整的一次「动态分发」）===\n");

    const char *wanted = "abs";
    UniformFn   picked = NULL;

    for (int i = 0; i < n; i++) {
        if (strcmp(catalog[i].name, wanted) == 0) { picked = catalog[i].fn; break; }
    }
    if (picked != NULL) {
        ci.nargs = 1; ci.args[0] = -7;
        printf("  查到 \"%s\"，调用结果 = %lld\n", wanted, (long long) picked(&ci));
    }

    printf("\n=== 4.3 ⚠️ 代价：类型检查没了 ===\n");
    /* 这是这套机制最危险的地方，PG 完全一样。 */
    ci.nargs = 1; ci.args[0] = 100;   /* 注意：没动 args[1]，它还留着上面那次调用的值 */
    printf("  故意用 1 个参数去调 add（它要 2 个）：\n");
    printf("    结果 = %lld   <- 不报错，不崩溃\n", (long long) u_add(&ci));
    printf("    100 + ? = 上面那个数。args[1] 里是【上一次调用残留的值】，\n");
    printf("    add 照读不误，因为没人告诉它这次只传了 1 个参数。\n");

    printf("\n  编译器帮不了你：签名是合法的，它无从知道 add 需要两个参数。\n");
    printf("  ★ PG 里一模一样 —— 取错参数类型/个数，读到垃圾数据，静默出错。\n");
    printf("    PG 只能在运行时靠 pg_proc 里记的 pronargs 自己核对。\n");

    printf("\n【记住这一句】统一签名不是为了好看，是为了让所有函数能进同一张表。\n");
    printf("代价是把类型检查从编译期丢掉了，得自己在运行时补。\n");

    return 0;
}
