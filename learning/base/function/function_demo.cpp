//
// Created by even on 2022/3/2.
//
#include <iostream> // <>用于相对于编译器的include 路径，一般用于系统库或者外部库；然后对于C语言的扩展库一般 .h 结尾，而没有扩展名的一般是C++的扩展库，只是一种约定与规范
#include "simple_function.h" //""表示从当前文件相对路径进行查找，一般自定义头文件使用

/**
 * 函数: 一般在.h文件中写函数声明，在源文件中引入.h文件，并进行函数的定义
 *
 * 声明: 为了告诉编译器，函数存在，可以使用
 * 返回值类型 函数名(参数类型列表);  //注:如果在使用前定义则可以不需要声明，但是在使用后定义，则需要声明;[声明可以有多次，定义只能一次]
 *
 * 定义:  //参数可以带有默认值即: (参数=默认值);但是从第一个有默认值的参数则后面的参数都必须要有默认值；声明与实现只能有一个有默认参数
 * 返回值类型 函数名(参数列表){
 *  函数体;
 *  返回值;
 * }
 *
 * 值传递:如果传的是值则不会影响实参
 *
 * 地址传递:参数为指针，函数中会修改指针参数的值
 *
 * 引用传递:相当于形式参数是实参的别名，所以操作的相当于就是实参
 *
 */

int add(int a = 10,int b =20);

int add(int a,int b){
    return a+b;
}

int max(int num1,int num2){
    return num1 > num2 ? num1 : num2;
}

long multi(int num1,int num2){
    return num1 * num2;
}
//int add(int a=2,int b=3){ //错误
//    return a+b;
//}


int swapInner(int num1,int num2){
    int temp = num1;
    num1 = num2;
    num2 = temp;
}

int swapPoint(int *p1,int *p2){
    int temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}


//引用传递： 引用传递会修改实参的值
// &num1 本质其实就是 外部参数的别名，所以可以修改外部参数值
int swap(int &num1,int &num2){
    int temp = num1;
    num1 = num2;
    num2 = temp;
}

int main(){
    int num1 = 1;
    int num2 = 2;

    std::cout << "add: " <<  add(num1,num2) << std::endl;
    std::cout << "max: " << max(num1,num2) << std::endl; //2
    std::cout << "multi: " << multi(num1,num2) << std::endl;

    // 值传递，只是将引用的值复制给参数，然后参数用的是新地址，地址上的数据是引用数据的复制，所以在内部修改的也是参数的数据，不是外部引用的数据
    swapInner(num1,num2);
    std::cout << "swapInner : "<< std::endl;
    std::cout << "num1 : " << num1 << std::endl; //1
    std::cout << "num2 : " << num2 << std::endl; //2

    // 传的是指针，所以实际操作的也是指针，函数中交换的是指向的地址，所以外部看也修改了
    int * p1 = &num1;
    int * p2 = &num2;
    swapPoint(p1,p2);
    std::cout << "swapPoint : "<< std::endl;
    std::cout << "num1 : " << num1 << std::endl; //2
    std::cout << "num2 : " << num2 << std::endl; //1

    // 引用传递: 实际就是把当前参数起了个别名作为参数，所以函数中本质操作的就是这个地址上的数据，所以会进行交换
    std::cout << "swap : "<< std::endl;
    swap(num1,num2);//引用传递会修改实参的值
    std::cout << "num1 : " << num1 << std::endl; //1
    std::cout << "num2 : " << num2 << std::endl; //2


}