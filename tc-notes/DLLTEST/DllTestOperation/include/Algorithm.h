//Algorithm.h
/****************************************************************************************************
__declspec��Microsoft VC��ר�õĹؼ��֣��������һЩ���Կ��ԶԱ�׼C/C++�������䡣
__declspec�ؼ���Ӧ�ó�����������ǰ�档
__declspec(dllexport)����Windows�еĶ�̬���У����������������ࡢ����ȹ�������ã�ʡ�Ը���.def�ļ���
�����������������Ϊ����������������������ã���Ϊ��̬��Ķ���ӿں�������ȡ�
.def�ļ�(ģ�鶨���ļ�)�ǰ���һ��������������DLL���Ե�Module�����ı��ļ���
.def�ļ���__declspec(dllexport)���ǽ��������ŵ��뵽Ӧ�ó�����DLL����������
������ṩ__declspec(dllexport)����DLL��������DLL��Ҫ�ṩ.def�ļ���
__declspec(dllimport)����Windows�У��ӱ�Ķ�̬�����������뺯�����ࡢ����ȹ�����̬���exe�ļ�ʹ�á�
������Ҫʹ��DLL�еĺ���ʱ����������Ҫ��ʾ�ص��뺯�������������Զ���ɡ�
��ʹ��__declspec(dllimport)Ҳ����ȷ������룬��ʹ��__declspec(dllimport)ʹ�������������ɸ��õĴ��롣
������֮�����ܹ����ɸ��õĴ��룬����Ϊ������ȷ�������Ƿ������DLL�У�
��ʹ�ñ��������������������Ѱַ����Ĵ��룬����Щ����ͨ��������ڿ�DLL�߽�ĺ��������С�
����һ�����뺯������˵��������Ǵӱ��DLL���롣һ������ʹ��ĳ��DLL��exe�С�
****************************************************************************************************/
#ifndef ALGORITHM_H_
#define ALGORITHM_H_

//������ʹ�ö�̬�����
#define DLL_FUNCTION
//�෽����ʹ�ö�̬�����
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

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ŀ ¼ �� �� �� ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
class DLL_CAPI DirFile
{
	private:
		//�洢�ļ�������Ϣ�Ľṹ��
		//static struct _finddata_t fileInfo;
		struct _finddata_t fileInfo;
		/*
		struct _finddata_t
        {
             unsigned attrib;//�ļ����ԵĴ洢λ��
			 _A_ARCH���浵���� _A_HIDDEN�����أ���_A_NORMAL����������_A_RDONLY��ֻ������_A_SUBDIR���ļ��У���_A_SYSTEM��ϵͳ��
             time_t time_create;//long int�������1970��1��1��0ʱ0��0�뵽����ʱ�̵�����
             time_t time_access;//�ļ����һ�α����ʵ�ʱ��
             time_t time_write;//�ļ����һ�α��޸ĵ�ʱ��
             _fsize_t size;//�ļ��Ĵ�С���ֽ�����ʾ��
             char name[_MAX_FNAME];//�ļ����ļ���
        }
		*/

		vector<CString> CurfilesVec;//��ŵ�ǰĿ¼�µ��ļ�
		vector<CString> AllfilesVec;//��ŵ�ǰĿ¼�µ������ļ�
	public:
		DirFile();
		~DirFile();

		//��ȡĿ¼��ָ����ʽ�������ļ�·����vector����
		void ReadDirAllFiles(CString path, const char *fmt = "*");

		//��ʾ�ļ���������
		void Show();
};

//extern "C" _declspec(dllexport) void __stdcall show();

DLL_FAPI void show();

#endif


