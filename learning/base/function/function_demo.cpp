//
// Created by even on 2022/3/2.
//
#include "simple_function.h" //""表示自定义头文件



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


int add(int,int);

int multi(int num1,int num2){
    return num1*num2;
}

int add(int a = 10,int b =20);

int add(int a,int b){
    return a+b;
}

//int add(int a=2,int b=3){ //错误
//    return a+b;
//}


int swapInner(int num1,int num2){
    cout << "swapInner : "<< endl;
    cout << "num1 : " << num1 << endl;
    cout << "num2 : " << num2 << endl;
    int temp = num1;
    num1 = num2;
    num2 = temp;
    cout << "after swap : "<< endl;
    cout << "num1 : " << num1 << endl;
    cout << "num2 : " << num2 << endl;
}

int swapPoint(int *p1,int *p2){
    cout << "swapPoint : "<< endl;
    cout << "num1 : " << *p1 << endl;
    cout << "num2 : " << *p2 << endl;
    int temp = *p1;
    *p1 = *p2;
    *p2 = temp;
    cout << "after swap : "<< endl;
    cout << "num1 : " << *p1 << endl;
    cout << "num2 : " << *p2 << endl;
}


//引用传递： 引用传递会修改实参的值
int swap(int &num1,int &num2){
    cout << "swapInner : "<< endl;
    cout << "num1 : " << num1 << endl;
    cout << "num2 : " << num2 << endl;
    int temp = num1;
    num1 = num2;
    num2 = temp;
    cout << "after swap : "<< endl;
    cout << "num1 : " << num1 << endl;
    cout << "num2 : " << num2 << endl;
}

int main(){
    int num1 = 1;
    int num2 = 2;

    cout << add(num1,num2) << endl;

    cout << multi(num1,num2) << endl;

    swapInner(num1,num2);
//    swap(num1,num2);//引用传递会修改实参的值
    cout << "main : "<< endl;
    cout << "num1 : " << num1 << endl; //1
    cout << "num2 : " << num2 << endl; //2


    cout << "max : " << max(num1,num2) << endl; //2

    int * p1 = &num1;
    int * p2 = &num2;
    swapPoint(p1,p2);
    cout << "num1 : " << num1 << endl; //2
    cout << "num2 : " << num2 << endl; //1



}

int add(int num1,int num2){
    return num1+num2;
}

int max(int num1,int num2){
    return num1 > num2 ? num1 : num2;
}