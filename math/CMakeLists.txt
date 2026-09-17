# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
#aux_source_directory(. DIR_LIB_SRCS)
#add_executable(math_demo math_demo.cpp)

# 生成链接库
#add_library (MathFunctions ${DIR_LIB_SRCS})
# 也可以用以上方式(需要把上面两部分注释解开)生成链接库;
add_library (MathFunctions MathFunctions.cpp)

