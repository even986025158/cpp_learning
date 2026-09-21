# foundation：阶段0（D01–D05）可运行、可调试演示。
# 可被顶层 cpp_learning 纳入，也可本目录独立 cmake。
# 所有 target 使用 fd_ 前缀，避免和 learning/、c/、math/ 重名。

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    cmake_minimum_required(VERSION 3.20)
    project(foundation LANGUAGES C CXX)
    set(CMAKE_C_STANDARD 17)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    enable_testing()
endif ()

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(FOUNDATION_ENABLE_SANITIZERS "为 foundation target 打开 ASan+UBSan" OFF)

function(fd_apply_common tgt)
    target_compile_options(${tgt} PRIVATE -Wall -Wextra -Wpedantic)
    target_include_directories(${tgt} PRIVATE "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/common")
    if (FOUNDATION_ENABLE_SANITIZERS)
        target_compile_options(${tgt} PRIVATE -fsanitize=address,undefined)
        target_link_options(${tgt} PRIVATE -fsanitize=address,undefined)
    endif ()
endfunction()

function(fd_add_c_demo tgt src)
    add_executable(${tgt} ${src})
    fd_apply_common(${tgt})
    add_test(NAME ${tgt} COMMAND ${tgt})
endfunction()

function(fd_add_cxx_demo tgt src)
    add_executable(${tgt} ${src})
    fd_apply_common(${tgt})
    add_test(NAME ${tgt} COMMAND ${tgt})
endfunction()

# ----- D01 -----
fd_add_c_demo(fd_d01_01_values_and_functions d01/01_values_and_functions.c)
fd_add_c_demo(fd_d01_02_compile_stages d01/02_compile_stages.c)

add_executable(fd_d01_03_multifile
        d01/03_multifile/main.c
        d01/03_multifile/calc.c)
fd_apply_common(fd_d01_03_multifile)
target_include_directories(fd_d01_03_multifile PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/d01/03_multifile")
add_test(NAME fd_d01_03_multifile COMMAND fd_d01_03_multifile)
# 04_link_errors 故意失败，只留 README，不进默认 build。

# ----- D02 -----
fd_add_c_demo(fd_d02_01_address_pointer d02/01_address_pointer.c)
fd_add_c_demo(fd_d02_02_array_string d02/02_array_string.c)
fd_add_c_demo(fd_d02_03_pointer_to_pointer d02/03_pointer_to_pointer.c)
fd_add_c_demo(fd_d02_04_struct_layout d02/04_struct_layout.c)
fd_add_c_demo(fd_d02_05_tagged_node d02/05_tagged_node.c)
fd_add_c_demo(fd_d02_06_heap_lifetime d02/06_heap_lifetime.c)

add_executable(fd_d02_arena
        d02/arena/arena.c
        d02/arena/main.c)
fd_apply_common(fd_d02_arena)
target_include_directories(fd_d02_arena PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/d02/arena")
add_test(NAME fd_d02_arena COMMAND fd_d02_arena)

# ----- D03 -----
fd_add_c_demo(fd_d03_01_object_macro d03/01_object_macro.c)
fd_add_c_demo(fd_d03_02_function_macro d03/02_function_macro.c)
fd_add_c_demo(fd_d03_03_safe_multistatement_macro d03/03_safe_multistatement_macro.c)
fd_add_c_demo(fd_d03_04_function_pointer d03/04_function_pointer.c)
fd_add_c_demo(fd_d03_05_callback_table d03/05_callback_table.c)
fd_add_c_demo(fd_d03_06_uniform_signature d03/06_uniform_signature.c)
fd_add_c_demo(fd_d03_08_tagged_node_list d03/08_tagged_node_list.c)

add_library(fd_d03_plugin MODULE d03/07_runtime_load/plugin.c)
set_target_properties(fd_d03_plugin PROPERTIES
        PREFIX ""
        POSITION_INDEPENDENT_CODE ON)
fd_apply_common(fd_d03_plugin)

add_executable(fd_d03_07_runtime_load d03/07_runtime_load/main.c)
fd_apply_common(fd_d03_07_runtime_load)
target_link_libraries(fd_d03_07_runtime_load PRIVATE ${CMAKE_DL_LIBS})
# 主程序不链接 plugin；路径由参数 / 生成器表达式传入。
add_dependencies(fd_d03_07_runtime_load fd_d03_plugin)
add_test(NAME fd_d03_07_runtime_load
        COMMAND fd_d03_07_runtime_load "$<TARGET_FILE:fd_d03_plugin>")

# PG 探针：缺 pg_config.h 时跳过，不破坏默认 build。
set(FD_PG_SOURCE_DIR "/Users/even/Desktop/even/code/cpp/postgres"
        CACHE PATH "PostgreSQL 源码根目录（需已 ./configure）")
if (EXISTS "${FD_PG_SOURCE_DIR}/src/include/pg_config.h")
    add_library(fd_d03_pg_probe OBJECT EXCLUDE_FROM_ALL d03/pg_probe/probe.c)
    target_include_directories(fd_d03_pg_probe PRIVATE "${FD_PG_SOURCE_DIR}/src/include")
    target_compile_options(fd_d03_pg_probe PRIVATE -w)
    message(STATUS "foundation: 可选 target fd_d03_pg_probe 已登记（EXCLUDE_FROM_ALL）")
else ()
    message(STATUS "foundation: 跳过 fd_d03_pg_probe（未找到 pg_config.h）")
endif ()

# ----- D04 -----
fd_add_c_demo(fd_d04_00_c_no_ctor d04/00_ctor_dtor/01_c_no_ctor.c)
fd_add_cxx_demo(fd_d04_00_constructor d04/00_ctor_dtor/02_constructor.cpp)
fd_add_cxx_demo(fd_d04_00_destructor d04/00_ctor_dtor/03_destructor.cpp)
fd_add_cxx_demo(fd_d04_01_reference_const d04/01_reference_const.cpp)
fd_add_cxx_demo(fd_d04_02_lifetime_raii d04/02_lifetime_raii.cpp)
fd_add_cxx_demo(fd_d04_03_virtual_dispatch d04/03_virtual_dispatch.cpp)
fd_add_cxx_demo(fd_d04_04_unique_ptr_move d04/04_unique_ptr_move.cpp)
fd_add_cxx_demo(fd_d04_05_vector_lambda_template d04/05_vector_lambda_template.cpp)
fd_add_cxx_demo(fd_d04_06_exception_assert d04/06_exception_assert.cpp)

add_subdirectory(d04/evaluator)

# ----- D05：嵌套工程既可独立配置，也可被本文件纳入 -----
add_subdirectory(d05/cmake-demo)
add_subdirectory(d05/mini-query)

add_custom_target(fd_all
        DEPENDS
        fd_d01_01_values_and_functions
        fd_d01_02_compile_stages
        fd_d01_03_multifile
        fd_d02_01_address_pointer
        fd_d02_02_array_string
        fd_d02_03_pointer_to_pointer
        fd_d02_04_struct_layout
        fd_d02_05_tagged_node
        fd_d02_06_heap_lifetime
        fd_d02_arena
        fd_d03_01_object_macro
        fd_d03_02_function_macro
        fd_d03_03_safe_multistatement_macro
        fd_d03_04_function_pointer
        fd_d03_05_callback_table
        fd_d03_06_uniform_signature
        fd_d03_07_runtime_load
        fd_d03_plugin
        fd_d03_08_tagged_node_list
        fd_d04_00_c_no_ctor
        fd_d04_00_constructor
        fd_d04_00_destructor
        fd_d04_01_reference_const
        fd_d04_02_lifetime_raii
        fd_d04_03_virtual_dispatch
        fd_d04_04_unique_ptr_move
        fd_d04_05_vector_lambda_template
        fd_d04_06_exception_assert
        fd_d04_evaluator
        fd_cmake_demo_app
        fd_mini_query)
