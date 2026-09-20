# 可被 foundation 纳入，也可本目录独立 cmake -S . -B build
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    cmake_minimum_required(VERSION 3.20)
    project(fd_d04_evaluator_proj LANGUAGES CXX)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    enable_testing()
endif ()

add_executable(fd_d04_evaluator expr.cpp main.cpp)
target_include_directories(fd_d04_evaluator PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

if (COMMAND fd_apply_common)
    fd_apply_common(fd_d04_evaluator)
else ()
    target_compile_options(fd_d04_evaluator PRIVATE -Wall -Wextra -Wpedantic)
    if (FOUNDATION_ENABLE_SANITIZERS)
        target_compile_options(fd_d04_evaluator PRIVATE -fsanitize=address,undefined)
        target_link_options(fd_d04_evaluator PRIVATE -fsanitize=address,undefined)
    endif ()
endif ()

add_test(NAME fd_d04_evaluator COMMAND fd_d04_evaluator)
