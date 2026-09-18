/*
 * D03 · PG 宏展开探针
 *
 * 这个文件不是用来「运行」的（它没有 main，也不链接）。
 * 它只用来做一件事：把 PG 的几个高频宏喂给预处理器，看它们展开成什么普通 C。
 *
 * 怎么用（在这个目录下执行）：
 *
 *     export PG=/Users/even/Desktop/even/code/cpp/postgres
 *     gcc -E -I $PG/src/include probe.c -o probe.i
 *     sed -n '/^void demo/,$p' probe.i
 *
 * -E = 只做预处理，不编译。所以即使没有 main、没链接 PG 的库，也能跑通。
 *
 * 看完回答三个问题（不看答案先自己说）：
 *   1. foreach 展开后是一个 for 循环。它的「初始化 / 条件 / 步进」三段各做了什么？
 *   2. lfirst(lc) 展开成了什么？为什么说 PG 的 List 其实是数组不是链表？
 *   3. IsA(n, Var) 展开成了什么？为什么这要求 NodeTag 必须是 struct 的第一个字段？
 */

#include "postgres.h"
#include "nodes/pg_list.h"

void		demo(List *l);

void
demo(List *l)
{
	ListCell   *lc;

	/* ① 遍历宏：PG 源码里出现频率最高的东西 */
	foreach(lc, l)
	{
		/* ② 取元素：lfirst 把 ListCell 里的指针取出来 */
		Node	   *n = (Node *) lfirst(lc);

		/* ③ 手工 instanceof：判断这个 Node 的实际类型 */
		if (IsA(n, Var))
			elog(NOTICE, "这是一个 Var 节点");
	}
}

/*
 * 加练（做完上面三题再放开注释重跑一次）：
 * 把下面这段注释解开，看 PG_FUNCTION_ARGS / PG_GETARG_INT32 / PG_RETURN_INT32
 * 展开成什么 —— 这就是 SQL 里的 `a + b` 实际执行的函数长什么样。
 *
 * Datum
 * my_add(PG_FUNCTION_ARGS)
 * {
 *     int32 a = PG_GETARG_INT32(0);
 *     int32 b = PG_GETARG_INT32(1);
 *     PG_RETURN_INT32(a + b);
 * }
 */
