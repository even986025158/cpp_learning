cmake_minimum_required(VERSION 3.20)
project(fd_cmake_visibility LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(FD_VISIBILITY_PUBLIC "把库的公共头文件目录传给消费者" ON)

add_library(visibility_math STATIC lib/add.cpp)
target_include_directories(visibility_math PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

if (FD_VISIBILITY_PUBLIC)
    target_include_directories(visibility_math PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
else ()
    # 唯一旋钮：消费者不再继承 include 路径；库自己仍由上面的 PRIVATE 正常编译。
    target_include_directories(visibility_math PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
endif ()

add_executable(visibility_consumer app/main.cpp)
target_link_libraries(visibility_consumer PRIVATE visibility_math)
