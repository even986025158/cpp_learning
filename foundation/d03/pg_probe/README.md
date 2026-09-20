# D03 · PG 宏探针（可选，不进默认测试）

`probe.c` 没有 `main`，也不链接 PostgreSQL 库。它只给预处理器看。

默认 build **不会**因为缺 PG 头文件失败：CMake 仅在

`$FD_PG_SOURCE_DIR/src/include/pg_config.h`

存在时才生成可选 OBJECT target `fd_d03_pg_probe`。

## 手动展开

```bash
export PG=/Users/even/Desktop/even/code/cpp/postgres
clang -std=c17 -E -I "$PG/src/include" probe.c -o /tmp/fd_pg_probe.i
sed -n '/^void demo/,$p' /tmp/fd_pg_probe.i
```

`-E` 只做预处理。没有 `pg_config.h` 时这里会失败——那是预期，不影响 `foundation` 的 ctest。
