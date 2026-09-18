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

# D02 练习
add_executable(c_arena
        arena/arena.c
        arena/main.c)
target_include_directories(c_arena PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/arena)
