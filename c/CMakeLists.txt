# C 语言学习实验（30 天内核攻坚 · D02 起）
#
# 作用：让 CLion 知道 c/ 下每个 .c 该怎么编译。
#      不加入 CMake 的文件，CLion 的代码分析器拿不到编译参数（不知道 SDK 在哪、
#      用哪个标准），就会误报 "'stdio.h' file not found"。
#
# 每个 .c 都是一个独立的、含 main 的小程序，所以一个文件对应一个 target。
# 下面用 foreach 自动生成，新增实验文件后只要 Reload CMake Project 即可，不用改这里。

# C11：任务卡统一要求（clang -std=c11）
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 严格警告：任务卡说「警告本身也属于学习结果，不要直接忽略」
# 02_array_decay 会故意触发 -Wsizeof-array-argument，那是实验目的，不是错误
add_compile_options(-Wall -Wextra -Wpedantic -g -gdwarf-4)

# 把某个子目录下所有 .c 各自编成一个可执行文件
# target 名 = 前缀 + 文件名，例如 c_warmup_00_hello、c_basics_01_pointer_out
function(add_c_experiments subdir prefix)
    file(GLOB sources "${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/*.c")
    foreach(src ${sources})
        get_filename_component(name ${src} NAME_WE)
        add_executable(${prefix}_${name} ${src})
    endforeach()
endfunction()

# 00 章 · 零基础预备：内存、指针、函数
add_c_experiments(warmup c_warmup)

# 02 章 · 四个必须：out 参数 / 数组退化 / struct 布局 / 柔性数组
add_c_experiments(basics c_basics)

# D03 插入补课：宏与预处理（读 ereport 之前）
add_c_experiments(macros c_macros)

# D03 插入补课：函数指针（读 PG_FUNCTION_ARGS / fmgr 之前必须先过这一组）
#
# 01 函数也有地址 -> 02 存地址的变量 -> 03 函数指针表(手工 vtable)
#   -> 04 统一签名(逼出 PG_FUNCTION_ARGS) -> 05 dlopen 运行时装载(fmgr 的真正动因)
add_c_experiments(funcptr c_funcptr)

# 05_runtime_load 要 dlopen 一个【单独编译】的动态库。
# 这个库故意不和主程序链接 —— 整节课的要点就是"主程序编译时它还不存在"。
# 放在 funcptr/plugin/ 子目录下，上面的 GLOB 是非递归的，不会把它当成一个 main 程序。
add_library(c_funcptr_plugin MODULE funcptr/plugin/myadd.c)
set_target_properties(c_funcptr_plugin PROPERTIES
        OUTPUT_NAME myadd
        PREFIX ""             # 不要 CMake 默认加的 lib 前缀，保持和任务卡里的 myadd.so 一致
        SUFFIX ".so"          # macOS 默认给 MODULE 加 .so，这里显式写死，和任务卡命令一致
        POSITION_INDEPENDENT_CODE ON)

# D02 练习
add_executable(c_arena
        arena/arena.c
        arena/main.c)
target_include_directories(c_arena PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/arena)

# D03 · PG 宏展开探针
#
# probe.c 没有 main，也不链接 PG 的任何库，它只是用来被预处理器展开看的。
# 但只要不进 CMake，CLion 就拿不到 -I 参数，postgres.h 会整片报红（坑 #7），
# ⌥⌘= 展开宏也用不了。所以这里建一个 OBJECT 库：只编译、不产可执行文件。
#
# PG 的头文件依赖 configure 生成的 pg_config.h，所以 PG 必须先 ./configure 过。
# 没 configure 时跳过这个 target，不要让整个 c/ 工程构建失败。
set(PG_SOURCE_DIR "/Users/even/Desktop/even/code/cpp/postgres"
        CACHE PATH "PostgreSQL 源码根目录（需已执行过 ./configure）")

if (EXISTS "${PG_SOURCE_DIR}/src/include/pg_config.h")
    add_library(c_pg_macro_probe OBJECT pg-macro/probe.c)
    target_include_directories(c_pg_macro_probe PRIVATE "${PG_SOURCE_DIR}/src/include")
    # PG 自己的代码风格触发不少警告，这里不是学习目标，关掉以免淹没真正的报错
    target_compile_options(c_pg_macro_probe PRIVATE -w)
else ()
    message(STATUS "跳过 c_pg_macro_probe：未找到 ${PG_SOURCE_DIR}/src/include/pg_config.h，请先在 PG 源码目录执行 ./configure")
endif ()
