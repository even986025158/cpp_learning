# C++17 内存查询演示；可独立配置，也可被 foundation 纳入。
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    cmake_minimum_required(VERSION 3.20)
    project(fd_mini_query_proj LANGUAGES CXX)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    enable_testing()
endif ()

add_executable(fd_mini_query main.cpp)

if (COMMAND fd_apply_common)
    fd_apply_common(fd_mini_query)
else ()
    target_compile_options(fd_mini_query PRIVATE -Wall -Wextra -Wpedantic)
    if (FOUNDATION_ENABLE_SANITIZERS)
        target_compile_options(fd_mini_query PRIVATE -fsanitize=address,undefined)
        target_link_options(fd_mini_query PRIVATE -fsanitize=address,undefined)
    endif ()
endif ()

add_test(NAME fd_mini_query COMMAND fd_mini_query)
