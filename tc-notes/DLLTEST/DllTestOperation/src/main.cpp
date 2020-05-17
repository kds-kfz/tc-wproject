//main.cpp
#include "Algorithm.h"
#include <memory>
#include <iostream>

#include <atlstr.h>

//TEST_MODE
//1：静态链接 2：动态链接隐式调用 3：动态链接显示调用
#define TEST_MODE 3

//动态链接隐式调用
//使用隐式动态链接需要指明库所在的位置（本例中在当前文件夹下），若改变路径，则需重写路径，静态变异也会使可执行文件的体积变大
#if TEST_MODE == 2
#pragma comment(lib,"..\\lib\\DllDemon.lib")   //这里要导入生成的库文件
#else if TEST_MODE == 3
//函数指针
typedef void(* pFUN)();
#endif

/*
一、静态连接库的调用
（1）将.h文件加入工程中，在调用此库的.cpp文件中包含此头文件。
（2）将.lib文件放到一个固定文件夹，在工程属性/VC++目录/库目录中添加此目录。
（3）在工程属性/链接器/输入/附加依赖库中添加此库的文件名（.lib），或者在调用此库的.cpp文件中添加#pragma comment(lib, “.lib”);

二、动态链接库的调用
隐式调用：
（1）.lib和.h的使用如上静态链接库的调用。
（2）将.dll文件放到源程序所在目录或者任意位置但是这样在调用时要写绝对路径
显示调用：
（1）.lib和.h文件不用。
（2）将.dll文件放到源程序所在目录或者任意位置但是这样在调用时要写绝对路径
*/

/*
静态链接、动态链接隐式调用、动态链接显示调用 的区别
*/

/*
静态链接库包括两个文件：
（1）.h头文件，包含静态链接库中说明输出的类或符号原型或数据结构。应用程序调用静态链接库时，需要将该文件包含入应用程序的源文件中。
（2）.lib文件，放到固定位置，在应用程序中添加库目录，在附加依赖项中进行添加。
动态链接库包括三个文件：
（1）.h头文件，包含动态链接库中说明输出的类或符号原型或数据结构的.h文件。应用程序调用动态链接库时，
需要将该文件包含入应用程序的源文件中。
（2）.lib文件，是动态链接库在编译、链接成功之后生成的文件，作用是当其他应用程序调用.dll时，
需要将该文件引入应用程序，否则产生错误。如果不想用.lib文件或者没有.lib文件，可以用WIN32 API函数LoadLibrary、GetProcAddress装载。
（3）.dll文件，真正的可执行文件，开发成功后的应用程序在发布时，只需要有.exe文件和.dll文件，并不需要.lib文件和.h头文件。
2. .lib文件和.dll文件区别
（1）.lib是编译时用到的，.dll是运行时用到的。如果要完成源代码的编译，只需要静态链接库；如果要使动态链接的程序运行起来，只需要动态链接库。
（2）如果有.dll文件，那么.lib一般是一些索引信息，记录了.dll中函数的入口和位置，.dll中是函数的具体内容；
如果只有.lib文件，那么这个.lib文件是静态编译出来的，索引和实现都在其中。使用静态编译的.lib文件，
在运行程序时不需要再挂动态库，缺点是导致应用程序比较大，而且失去了动态库的灵活性，发布新版本时要发布新的应用程序才行。
（3）动态链接的情况下，有两个文件：一个是.lib文件，一个是.dll文件。
.lib包含被.dll导出的函数名称和位置，.dll包含实际的函数和数据，应用程序使用.lib文件链接到.dll文件。
在应用程序的可执行文件中，存放的不是被调用的函数代码，而是.dll中相应函数代码的地址，从而节省了内存资源。
.dll和.lib文件必须随应用程序一起发行，否则应用程序会产生错误。
如果不想用.lib文件或者没有.lib文件，可以用WIN32 API函数LoadLibrary、GetProcAddress装载
*/



int main(int argc, char *argv[])
{
#if 0
	//测试 McsfAlgorithm 类
    std::shared_ptr<McsfAlgorithm> myOpertion(new McsfAlgorithm());
    int testNum1 = myOpertion->Add(12,23);
    int testNum2 = myOpertion->Sub(90,30);
    std::cout<<"testNum1 = "<<testNum1<<std::endl;
    std::cout<<"testNum2 = "<<testNum2<<std::endl;

	//测试 DirFile 类
	std::shared_ptr<DirFile> Opertion(new DirFile());
	CString path("C:\\Users\\yinling.wu\\Desktop\\DLLTEST");
	Opertion->ReadDirAllFiles(path, "cpp");
	Opertion->Show();
#endif
	
#if TEST_MODE == 1 || TEST_MODE == 2
	show();
#else if TEST_MODE == 3
	//显式调用动态库
	//使用对象，测试 DirFile 类
	CString path("C:\\Users\\yinling.wu\\Desktop\\DLLTEST");
	DirFile myDirFile;
	myDirFile.ReadDirAllFiles(path, "cpp");
	myDirFile.Show();

	//使用智能指针，测试 DirFile 类
	std::shared_ptr<DirFile> Opertion(new DirFile());
	Opertion->ReadDirAllFiles(path, "cpp");
	Opertion->Show();

	HMODULE hDLL = LoadLibrary("DllDemon.dll");
	if(hDLL != NULL)
	{
		pFUN p  = (pFUN)GetProcAddress(hDLL,"show");
		if(p != NULL)
		{
			(*p)();
		}
		else
		{
			cout<<"not Find Function show"<<endl;
		}
		FreeLibrary(hDLL);
	}
	else
	{
		cout<<"no DllDemon.dll"<<endl;
	}
#endif

	//ofstream fout("out.txt",ios::binary|ios::app);
	//fout.close();
	//C:\Users\yinling.wu\AppData\Local\Temp\VC++
	//表示直接调用DOS命令Pause, 输入任意字符程序继续
	system("pause");
    return 0;
}