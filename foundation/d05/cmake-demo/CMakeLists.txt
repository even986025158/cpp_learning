# 静态库 + 可执行文件 + ctest；可独立配置，也可被 foundation 纳入。
# 可选：-DFD_CMAKE_DEMO_SANITIZER=ON 或继承 FOUNDATION_ENABLE_SANITIZERS。
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    cmake_minimum_required(VERSION 3.20)
    project(fd_cmake_demo_proj LANGUAGES CXX)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    enable_testing()
endif ()

option(FD_CMAKE_DEMO_SANITIZER "为本嵌套工程打开 ASan+UBSan" OFF)

add_library(fd_cmake_demo_math STATIC add.cpp)
target_include_directories(fd_cmake_demo_math PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(fd_cmake_demo_app main.cpp)
target_link_libraries(fd_cmake_demo_app PRIVATE fd_cmake_demo_math)

if (COMMAND fd_apply_common)
    fd_apply_common(fd_cmake_demo_math)
    fd_apply_common(fd_cmake_demo_app)
else ()
    target_compile_options(fd_cmake_demo_math PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(fd_cmake_demo_app PRIVATE -Wall -Wextra -Wpedantic)
endif ()

if (FD_CMAKE_DEMO_SANITIZER OR FOUNDATION_ENABLE_SANITIZERS)
    foreach (tgt IN ITEMS fd_cmake_demo_math fd_cmake_demo_app)
        target_compile_options(${tgt} PRIVATE -fsanitize=address,undefined)
        target_link_options(${tgt} PRIVATE -fsanitize=address,undefined)
    endforeach ()
endif ()

add_test(NAME fd_cmake_demo_app COMMAND fd_cmake_demo_app)
