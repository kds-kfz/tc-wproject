//Algorithm.h
/****************************************************************************************************
__declspec是Microsoft VC中专用的关键字，它配合着一些属性可以对标准C/C++进行扩充。
__declspec关键字应该出现在声明的前面。
__declspec(dllexport)用于Windows中的动态库中，声明导出函数、类、对象等供外面调用，省略给出.def文件。
即将函数、类等声明为导出函数，供其它程序调用，作为动态库的对外接口函数、类等。
.def文件(模块定义文件)是包含一个或多个描述各种DLL属性的Module语句的文本文件。
.def文件或__declspec(dllexport)都是将公共符号导入到应用程序或从DLL导出函数。
如果不提供__declspec(dllexport)导出DLL函数，则DLL需要提供.def文件。
__declspec(dllimport)用于Windows中，从别的动态库中声明导入函数、类、对象等供本动态库或exe文件使用。
当你需要使用DLL中的函数时，往往不需要显示地导入函数，编译器可自动完成。
不使用__declspec(dllimport)也能正确编译代码，但使用__declspec(dllimport)使编译器可以生成更好的代码。
编译器之所以能够生成更好的代码，是因为它可以确定函数是否存在于DLL中，
这使得编译器可以生成跳过间接寻址级别的代码，而这些代码通常会出现在跨DLL边界的函数调用中。
声明一个导入函数，是说这个函数是从别的DLL导入。一般用于使用某个DLL的exe中。
****************************************************************************************************/
#ifndef ALGORITHM_H_
#define ALGORITHM_H_

//函数，使用动态库编译
#define DLL_FUNCTION
//类方法，使用动态库编译
#define DLL_CLASS

#ifdef DLL_FUNCTION  
#define DLL_FAPI extern "C" __declspec(dllexport)
#else  
#define DLL_FAPI extern "C" __declspec(dllimport)
#endif

#ifdef DLL_CLASS  
#define DLL_CAPI __declspec(dllexport)
#else  
#define DLL_CAPI __declspec(dllimport)
#endif

#include<windows.h>

#include<iostream>
#include<io.h>
#include<string>
#include<vector>

#include <cstring>
#include <atlstr.h>
using namespace std;



//class _declspec(dllexport) McsfAlgorithm
class DLL_CAPI McsfAlgorithm
{
public:
    McsfAlgorithm();
    ~McsfAlgorithm();
    int Add(int, int);
    int Sub(int, int);
};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 目 录 处 理 类 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
class DLL_CAPI DirFile
{
	private:
		//存储文件各种信息的结构体
		//static struct _finddata_t fileInfo;
		struct _finddata_t fileInfo;
		/*
		struct _finddata_t
        {
             unsigned attrib;//文件属性的存储位置
			 _A_ARCH（存档）、 _A_HIDDEN（隐藏）、_A_NORMAL（正常）、_A_RDONLY（只读）、_A_SUBDIR（文件夹）、_A_SYSTEM（系统）
             time_t time_create;//long int、保存从1970年1月1日0时0分0秒到现在时刻的秒数
             time_t time_access;//文件最后一次被访问的时间
             time_t time_write;//文件最后一次被修改的时间
             _fsize_t size;//文件的大小（字节数表示）
             char name[_MAX_FNAME];//文件的文件名
        }
		*/

		vector<CString> CurfilesVec;//存放当前目录下的文件
		vector<CString> AllfilesVec;//存放当前目录下的所有文件
	public:
		DirFile();
		~DirFile();

		//读取目录下指定格式的所有文件路径到vector容器
		void ReadDirAllFiles(CString path, const char *fmt = "*");

		//显示文件容器内容
		void Show();
};

//extern "C" _declspec(dllexport) void __stdcall show();

DLL_FAPI void show();

#endif


