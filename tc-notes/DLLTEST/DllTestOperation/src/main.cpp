//main.cpp
#include "Algorithm.h"
#include <memory>
#include <iostream>

#include <atlstr.h>

//TEST_MODE
//1����̬���� 2����̬������ʽ���� 3����̬������ʾ����
#define TEST_MODE 3

//��̬������ʽ����
//ʹ����ʽ��̬������Ҫָ�������ڵ�λ�ã��������ڵ�ǰ�ļ����£������ı�·����������д·������̬����Ҳ��ʹ��ִ���ļ���������
#if TEST_MODE == 2
#pragma comment(lib,"..\\lib\\DllDemon.lib")   //����Ҫ�������ɵĿ��ļ�
#else if TEST_MODE == 3
//����ָ��
typedef void(* pFUN)();
#endif

/*
һ����̬���ӿ�ĵ���
��1����.h�ļ����빤���У��ڵ��ô˿��.cpp�ļ��а�����ͷ�ļ���
��2����.lib�ļ��ŵ�һ���̶��ļ��У��ڹ�������/VC++Ŀ¼/��Ŀ¼����Ӵ�Ŀ¼��
��3���ڹ�������/������/����/��������������Ӵ˿���ļ�����.lib���������ڵ��ô˿��.cpp�ļ������#pragma comment(lib, ��.lib��);

������̬���ӿ�ĵ���
��ʽ���ã�
��1��.lib��.h��ʹ�����Ͼ�̬���ӿ�ĵ��á�
��2����.dll�ļ��ŵ�Դ��������Ŀ¼��������λ�õ��������ڵ���ʱҪд����·��
��ʾ���ã�
��1��.lib��.h�ļ����á�
��2����.dll�ļ��ŵ�Դ��������Ŀ¼��������λ�õ��������ڵ���ʱҪд����·��
*/

/*
��̬���ӡ���̬������ʽ���á���̬������ʾ���� ������
*/

/*
��̬���ӿ���������ļ���
��1��.hͷ�ļ���������̬���ӿ���˵�������������ԭ�ͻ����ݽṹ��Ӧ�ó�����þ�̬���ӿ�ʱ����Ҫ�����ļ�������Ӧ�ó����Դ�ļ��С�
��2��.lib�ļ����ŵ��̶�λ�ã���Ӧ�ó�������ӿ�Ŀ¼���ڸ����������н�����ӡ�
��̬���ӿ���������ļ���
��1��.hͷ�ļ���������̬���ӿ���˵�������������ԭ�ͻ����ݽṹ��.h�ļ���Ӧ�ó�����ö�̬���ӿ�ʱ��
��Ҫ�����ļ�������Ӧ�ó����Դ�ļ��С�
��2��.lib�ļ����Ƕ�̬���ӿ��ڱ��롢���ӳɹ�֮�����ɵ��ļ��������ǵ�����Ӧ�ó������.dllʱ��
��Ҫ�����ļ�����Ӧ�ó��򣬷�������������������.lib�ļ�����û��.lib�ļ���������WIN32 API����LoadLibrary��GetProcAddressװ�ء�
��3��.dll�ļ��������Ŀ�ִ���ļ��������ɹ����Ӧ�ó����ڷ���ʱ��ֻ��Ҫ��.exe�ļ���.dll�ļ���������Ҫ.lib�ļ���.hͷ�ļ���
2. .lib�ļ���.dll�ļ�����
��1��.lib�Ǳ���ʱ�õ��ģ�.dll������ʱ�õ��ġ����Ҫ���Դ����ı��룬ֻ��Ҫ��̬���ӿ⣻���Ҫʹ��̬���ӵĳ�������������ֻ��Ҫ��̬���ӿ⡣
��2�������.dll�ļ�����ô.libһ����һЩ������Ϣ����¼��.dll�к�������ں�λ�ã�.dll���Ǻ����ľ������ݣ�
���ֻ��.lib�ļ�����ô���.lib�ļ��Ǿ�̬��������ģ�������ʵ�ֶ������С�ʹ�þ�̬�����.lib�ļ���
�����г���ʱ����Ҫ�ٹҶ�̬�⣬ȱ���ǵ���Ӧ�ó���Ƚϴ󣬶���ʧȥ�˶�̬�������ԣ������°汾ʱҪ�����µ�Ӧ�ó�����С�
��3����̬���ӵ�����£��������ļ���һ����.lib�ļ���һ����.dll�ļ���
.lib������.dll�����ĺ������ƺ�λ�ã�.dll����ʵ�ʵĺ��������ݣ�Ӧ�ó���ʹ��.lib�ļ����ӵ�.dll�ļ���
��Ӧ�ó���Ŀ�ִ���ļ��У���ŵĲ��Ǳ����õĺ������룬����.dll����Ӧ��������ĵ�ַ���Ӷ���ʡ���ڴ���Դ��
.dll��.lib�ļ�������Ӧ�ó���һ���У�����Ӧ�ó�����������
���������.lib�ļ�����û��.lib�ļ���������WIN32 API����LoadLibrary��GetProcAddressװ��
*/



int main(int argc, char *argv[])
{
#if 0
	//���� McsfAlgorithm ��
    std::shared_ptr<McsfAlgorithm> myOpertion(new McsfAlgorithm());
    int testNum1 = myOpertion->Add(12,23);
    int testNum2 = myOpertion->Sub(90,30);
    std::cout<<"testNum1 = "<<testNum1<<std::endl;
    std::cout<<"testNum2 = "<<testNum2<<std::endl;

	//���� DirFile ��
	std::shared_ptr<DirFile> Opertion(new DirFile());
	CString path("C:\\Users\\yinling.wu\\Desktop\\DLLTEST");
	Opertion->ReadDirAllFiles(path, "cpp");
	Opertion->Show();
#endif
	
#if TEST_MODE == 1 || TEST_MODE == 2
	show();
#else if TEST_MODE == 3
	//��ʽ���ö�̬��
	//ʹ�ö��󣬲��� DirFile ��
	CString path("C:\\Users\\yinling.wu\\Desktop\\DLLTEST");
	DirFile myDirFile;
	myDirFile.ReadDirAllFiles(path, "cpp");
	myDirFile.Show();

	//ʹ������ָ�룬���� DirFile ��
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
	//��ʾֱ�ӵ���DOS����Pause, ���������ַ��������
	system("pause");
    return 0;
}