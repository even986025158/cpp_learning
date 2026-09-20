#Cmake文件作用:用于不同环境的C++代码编译，来实现Write once, run everywhere

#Cmake 语法:
#   注释:#
#   命令:命令名称(参数1 参数2 参数3 ...)

# Cmake文件使用:在当前目录执行cmake . ;完成后执行make便会得到对应的可执行文件;之后便可以执行可执行文件来运行程序

# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.20)
# 项目信息
project(cpp_learning)

set(CMAKE_CXX_STANDARD 17)

# 加入一个配置头文件，用于处理 CMake 对源码的设置
configure_file (
        "${PROJECT_SOURCE_DIR}/config.h.in"
        "${PROJECT_BINARY_DIR}/config.h"
)

# 是否使用自己的 MathFunctions 库
option (USE_MYMATH
        "Use provided math implementation" ON)

# 是否加入 MathFunctions 库
if (USE_MYMATH)
    include_directories ("${PROJECT_SOURCE_DIR}/math")
    add_subdirectory (math)
    set (EXTRA_LIBS ${EXTRA_LIBS} MathFunctions)
endif (USE_MYMATH)

# 添加 math 子目录;当执行到这里时，会先到子目录下找到CmakeList.txt执行;math/CmakeList.txt中生成了链接库，即将MathFunctions.cpp加入到MathFunctions链接库中;后面需要配合target_link_libraries命令使用
#add_subdirectory(math)
add_subdirectory(learning/hello_world)
add_subdirectory(learning/base/var)
add_subdirectory(learning/base/const)
add_subdirectory(learning/base/data_type)
add_subdirectory(learning/base/str)
add_subdirectory(learning/base/arr)
add_subdirectory(learning/base/function)
add_subdirectory(learning/base/point)
add_subdirectory(learning/base/struct)
add_subdirectory(learning/object)

# C 语言学习实验（30 天内核攻坚 D02 起）；不加进来 CLion 会误报 stdio.h not found
add_subdirectory(c)

# 阶段0 可运行演示（D01–D05）。target 一律 fd_ 前缀，不和上面的旧 target 重名。
# enable_testing 必须在顶层调用，ctest 才会收录子目录里的 add_test。
enable_testing()
add_subdirectory(foundation)


# 指定生成目标: add_executable(可执行文件名 源文件1 源文件2)[需要将Cmake文件放在源文件同级目录];表示将源文件编译为可执行文件名;即将main.cpp编译为hello_world_main
# 所以可执行文件名不能重名，源文件也不能重名
# 这些源文件有且只有一个main方法，且不存在冲突(即假设两个文件合并到一个文件，不存在同方法名等情况)
#这样是不行的，Cmake会将main.cpp cout_demo.cpp两个源文件合并生成为hello_world_main,但是由于两个源文件中有相同的方法(main)，所以无法进行编译
#add_executable(hello_world_main main.cpp cout_demo.cpp)
add_executable(hello_world_main main.cpp)
#这样是可以的，因为math_demo.cpp MathFunctions.cpp不冲突
#此时会将math_demo.cpp MathFunctions.cpp合为一个文件进行编译，编译时会将头文件中的声明信息加入到该文件中。所以虽然MathFunctions.cpp与math_demo math_demo.cpp中没有power方法的声明，但是在扫描头文件时会将MathFunctions.h中power方法的声明加入
#add_executable(math_demo math_demo.cpp math/MathFunctions.cpp)
add_executable(math_demo math_demo.cpp)

# 添加链接库;表示链接库中的文件加入math_demo来生成可执行文件[说明:由于.h文件是通过准确文件名来导入的，所以不存在找不到的问题.而.cpp文件是没有在main文件中导入的，所以需要进行链接]
target_link_libraries(math_demo MathFunctions)




