//Algorithm.cpp
#include "Algorithm.h"

McsfAlgorithm::McsfAlgorithm()
{

}

McsfAlgorithm::~McsfAlgorithm()
{

}

int McsfAlgorithm::Add(int firstNum, int secondNum)
{
    return firstNum + secondNum;
}

int McsfAlgorithm::Sub(int firseNum, int secondNum)
{
    return firseNum - secondNum;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 目 录 处 理 函 数 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//静态成员初始化
//struct _finddata_t DirFile::fileInfo = {0,0,0,0,'\0'};

DirFile::DirFile()
{
	CurfilesVec.clear();
	AllfilesVec.clear();
}

DirFile::~DirFile(){}
void DirFile::ReadDirAllFiles(CString path, const char *fmt)
{
	long hFile = 0;

	CString pathName = path + _T("\\*");
 
	//_findfirst 搜索与指定的文件名称匹配的第一个实例，若成功则返回第一个实例的句柄，否则返回-1L
	//assign 将原字符串清空，然后赋予新的值作替换
	if ((hFile = _findfirst(pathName.GetBuffer(), &fileInfo)) == -1) {
		return;
	}
	do {
		if (fileInfo.attrib&_A_SUBDIR){
			CString fname = _T(fileInfo.name);
			if (fname != ".." && fname != ".") {
				//是目录则递归调用
				ReadDirAllFiles(path + _T("\\") + fname, fmt);
			} 
		} else {
			CString objFile(path);
			objFile += _T("\\");
			if(strcmp(fmt, "*") == 0)
			{
				objFile += _T(fileInfo.name);
				AllfilesVec.push_back(objFile);
			}
			else
			{
				//匹配指定文件
				CString temp = _T(fileInfo.name);
				if(temp.Right(strlen(fmt)) == _T(fmt))
				{
					objFile += _T(fileInfo.name);
					AllfilesVec.push_back(objFile);
				}
			}
		}
	} while (_findnext(hFile, &fileInfo) == 0);
	//_findnext 若成功返回0，否则返回-1
	//_findclose 成功返回0，失败返回-1
	_findclose(hFile);
	return;
}

void DirFile::Show()
{
	if(!CurfilesVec.empty())
	{
		vector<CString>::iterator it = CurfilesVec.begin();
		for(; it != CurfilesVec.end(); it++)
		{
			cout<<*it<<endl;
		}
	}
	if(!AllfilesVec.empty())
	{
		vector<CString>::iterator it = AllfilesVec.begin();
		for(; it != AllfilesVec.end(); it++)
		{
			cout<<*it<<endl;
		}
	}
}

/*
void __stdcall show()
{
	cout<<"显示动态调用单个函数"<<endl;
}
*/

DLL_FAPI void show()
{
	cout<<"显示动态调用单个函数"<<endl;
}
