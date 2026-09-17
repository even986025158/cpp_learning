//
// Created by even on 2022/3/7.
//

#ifndef CPP_LEARNING_STUDENT_H
#define CPP_LEARNING_STUDENT_H

#include <string>
/**
 * 类:
 * 语法:
 * class 类名{
 * 访问权限:
 * 属性
 * 行为
 * }
 *
 * 访问权限 : [同java]，默认 private
 *
 *  与struct区别:
 *  类: 默认 private
 *  struct:默认 public
 *
 * 构造函数:
 *
 * 调用:
 * 类名 对象名; //默认构造,不要括号
 * 类名 对象名(参数列表);  //
 * 类名 对象名 = 类名(参数列表);
 * 类名 对象名 = 值;  // => 类名 对象名 = 类名(值)
 *
 * 析构函数:
 * ～类名() //编译器自动生成，对象销毁时调用
 *
 * 静态成员方法:
 * static 修饰的方法 [只能访问静态变量]
 * 调用:
 * 对象.方法()
 * 类名::方法()
 *
 */

using namespace std;

class Student {
private:
    string name;
    int age;


};


#endif //CPP_LEARNING_STUDENT_H
