#include <iostream> // #开头语句,编译器会对其进行预处理
                    // #include <iostream>表示进行预处理，将iostream文件的内容添加到当前文件中
                    //iostream为C++输入输出库;如使用cin与cout
                    //#include <头文件> ;扩展名为.h是C语言头文件，没有扩展名为C++头文件，c开头没有扩展名的为C转C++头文件
int main()  //函数头:程序连接的接口,告诉程序如何调用该函数(函数名、参数列表、返回类型)
{           //函数体:该函数具体做什么
    //std为iostream库的命名空间，如果cout，endl都是属于std命名空间的
    //如果没有using namespace std;要使用则需要std::cout;编译的时候会进行绑定
    //对于非.h扩展名的头文件，需要使用using namespace来导入命名空间
	using namespace std;

	cout << "welcome to C++ world!"; //A << B 表示将B发送到A;cout为预定义对象
	cout << endl; //endl表示重启一行;相当与\n
//    cout << "\n";
    cout << "Let's go !";
	return 0;
}
