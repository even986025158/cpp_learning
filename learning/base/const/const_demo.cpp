#include <iostream>

/**
 * 常量: 赋值后不可修改
 * 语法:
 * 1. 宏常量:#define 常量名 常量值  (通常定义在文件上方)
 * 2. const 数据类型 常量名 = 常量初始值
 *
 */
#define DAYS_OF_WEEK 7

int main(){

    std::cout << "一周有" << DAYS_OF_WEEK << "天" << std::endl;
    const int MONTH_OF_YEAR = 12;
    std::cout << "一年有" << MONTH_OF_YEAR << "月" << std::endl;
}