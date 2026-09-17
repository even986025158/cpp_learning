//
// Created by even on 2022/3/4.
//
#include <iostream>

/**
 * 结构体: 自定义数据类型
 * 语法:
 *  struct 结构体名{
 *      成员列表
 *  }
 * 使用: (注:C++中使用时struct可以省略)
 * 1.struct 结构体名 变量名;
 * 2.struct 结构体名 变量名 = {...};
 * 3.创建结构体时带一个变量[不推荐]
 *
 * 属性访问: 变量名.属性名
 *
 * 结构体数组:
 * struct 结构体名 数组名[数组个数] = {{...},{...},{...},...}
 *
 * 结构体指针:
 * Student * p = &s1;
 * 访问时: p -> 属性名
 *
 */
using namespace std;

struct Student{
    string name;
    int age;
    int score;
}s3;

int main(){

    struct Student s1;
    s1.name = "张三";
    s1.age = 18;
    s1.score = 80;

    cout << s1.name << "的年龄是: " << s1.age << ";分数是: " << s1.score << endl;

    Student s2 = {"李四",19,80};

    s3.name = "王五";
    s3.age = 20;
    s3.score = 80;


    Student students[3] = {
            {"张三",19,80},
            {"李四",19,80},
            {"王五",19,80},
    };

    Student * p = &s1;
    cout << p -> name << "的年龄是: " << p -> age << ";分数是: " << p -> score << endl;

}