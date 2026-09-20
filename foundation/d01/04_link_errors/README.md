# D01 · 链接错误（只手工跑，不进默认 build）

这两个命令**故意失败**，用来亲眼看 `undefined symbol` 和 `duplicate symbol`。
不要把它们写成 CMake target：默认 `cmake --build` / `ctest` 必须全绿。

工作目录：本 README 所在目录。`main.c` 还依赖 `foundation/common/check.h`。

下面两组命令已在 macOS arm64 / AppleClang 16 上实测。

## 1. undefined symbol（用了符号，却没给定义）

```bash
clang -std=c17 -Wall -Wextra -Wpedantic \
  -I../03_multifile -I../../common \
  ../03_multifile/main.c \
  -o /tmp/fd_d01_undef
```

只编译了 `main.c`，没有 `calc.c`。链接器在 `main` 里看到 `calc_add` / `calc_mul`，别处找不到定义。

本机实测报错（关键行）：

```
Undefined symbols for architecture arm64:
  "_calc_add", referenced from:
      _main in main-….o
  "_calc_mul", referenced from:
      _main in main-….o
ld: symbol(s) not found for architecture arm64
```

**不会**生成 `/tmp/fd_d01_undef`。

对照：补上定义就过——这是默认 build 里 `fd_d01_03_multifile` 在做的事：

```bash
clang -std=c17 -Wall -Wextra -Wpedantic \
  -I../03_multifile -I../../common \
  ../03_multifile/main.c ../03_multifile/calc.c \
  -o /tmp/fd_d01_ok && /tmp/fd_d01_ok
```

## 2. duplicate symbol（同一个函数定义了两次）

必须先分别编译出两个带同一组 `T` 符号的 `.o`，再和 `main.o` 一起链。
直接 `clang calc.c calc.c` 在本机只会先报缺 `main`，看不到重复定义。

```bash
clang -std=c17 -c ../03_multifile/main.c -I../03_multifile -I../../common -o /tmp/fd_main.o
clang -std=c17 -c ../03_multifile/calc.c -o /tmp/fd_calc1.o
clang -std=c17 -c ../03_multifile/calc.c -o /tmp/fd_calc2.o
clang /tmp/fd_main.o /tmp/fd_calc1.o /tmp/fd_calc2.o -o /tmp/fd_d01_dup
```

本机实测报错（关键行）：

```
duplicate symbol '_calc_add' in:
    /private/tmp/fd_calc2.o
    /private/tmp/fd_calc1.o
duplicate symbol '_calc_mul' in:
    /private/tmp/fd_calc2.o
    /private/tmp/fd_calc1.o
ld: 2 duplicate symbols
```

## 不要做的事

- 不要为了「看错误」去读未初始化内存、解引用空指针，或把失败 target 挂进默认 `ALL`。
- `nm` 看符号（`T` = 本文件已定义，`U` = 等链接器去找）是安全的，可以自行对 `calc.c -c` 后的 `.o` 使用。
