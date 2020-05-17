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

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ŀ ¼ �� �� �� �� ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//��̬��Ա��ʼ��
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
 
	//_findfirst ������ָ�����ļ�����ƥ��ĵ�һ��ʵ�������ɹ��򷵻ص�һ��ʵ���ľ�������򷵻�-1L
	//assign ��ԭ�ַ�����գ�Ȼ�����µ�ֵ���滻
	if ((hFile = _findfirst(pathName.GetBuffer(), &fileInfo)) == -1) {
		return;
	}
	do {
		if (fileInfo.attrib&_A_SUBDIR){
			CString fname = _T(fileInfo.name);
			if (fname != ".." && fname != ".") {
				//��Ŀ¼��ݹ����
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
				//ƥ��ָ���ļ�
				CString temp = _T(fileInfo.name);
				if(temp.Right(strlen(fmt)) == _T(fmt))
				{
					objFile += _T(fileInfo.name);
					AllfilesVec.push_back(objFile);
				}
			}
		}
	} while (_findnext(hFile, &fileInfo) == 0);
	//_findnext ���ɹ�����0�����򷵻�-1
	//_findclose �ɹ�����0��ʧ�ܷ���-1
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
	cout<<"��ʾ��̬���õ�������"<<endl;
}
*/

DLL_FAPI void show()
{
	cout<<"��ʾ��̬���õ�������"<<endl;
}
