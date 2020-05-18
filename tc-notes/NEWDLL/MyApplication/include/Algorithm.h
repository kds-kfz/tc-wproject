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


#include<iostream>
#include<io.h>
#include<string>
#include<vector>

#include <cstring>
#include <atlstr.h>
using namespace std;

//������ͷ�ļ�ע���Ⱥ�˳��
//��̬����ʽ����
#include <winsock2.h>
#include<windows.h>
#pragma comment(lib,"ws2_32.lib")

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

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ �� �� �� �� �� �� ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//����socketͨ�Žṹ�壬������ڿͻ��˹���
#define BUFF_SIZE (1024 * 8)
#define REQ_MAX_LEN (1024 * 7)
typedef struct{
    SOCKET fd;
    //struct sockaddr_in addr;
	SOCKADDR_IN addr;
    //char ip_buf[64];
}Total_msg;

class DLL_CAPI Socket
{
private:
	string errmsg;			//������Ϣ
	char type[4];			//��Ϣ����
	char buff[BUFF_SIZE];	//��������
	Total_msg *sfd;			//��������ݽṹ
	Total_msg *cfd;			//�ͻ������ݽṹ
public:
	Socket()
	{
		sfd = new Total_msg;
		cfd = new Total_msg;
		memset(this->type, 0, sizeof(this->type));
		//memset(this->buff, 0, sizeof(this->buff));
		sfd->fd = 0;
		cfd->fd = 0;
		errmsg.clear();
	}
	
	char *SocketBuff(){
		return buff;
	}

	char *SocketClientBuff(){
        return buff;
    }
	
	string SocketErrmsg(){
        return errmsg;
    }

    char *SocketType(){
        return this->type;
    }

	~Socket()
	{
		Close();
	}

	void Close()
	{
		if(!SocketServeClose())
		{
			cout<<errmsg<<endl;
		}
		if(!SocketClientClose())
		{
			cout<<errmsg<<endl;
		}
		delete []sfd;
		delete []cfd;
	}

	BOOL SocketServeClose(){
		if(sfd->fd < 0){
			errmsg = "The Serve Sfd was Shuted!";
			return FALSE;
		}
		int ret = closesocket(sfd->fd);
		if(ret == 0){
			return TRUE;
		}else if(ret == -1){
			errmsg = "SocketServeClose Error:[" + string(strerror(ret)) + "]!";
			return FALSE;
		}
		return TRUE;
	}

	BOOL SocketClientClose(){
		if(cfd->fd < 0){
			errmsg = "The Serve Sfd was Shuted!";
			return FALSE;
		}
        
		int ret = closesocket(cfd->fd);
		if(ret == 0){
			return TRUE;
		}else if(ret == -1){
			errmsg = "SocketClientClose Error:[" + string(strerror(ret)) + "]!";
			return FALSE;
		}
		return TRUE;
	}

	/**********     SocketServe �����     **********/
	BOOL SocketServeBuild(){
		WSADATA wsaData;
		WORD sockVersion = MAKEWORD(2, 2);
		int err = WSAStartup(sockVersion, &wsaData);
		if(err != 0)
		{
			cout<<"WSAStartup faild"<<endl;
			return FALSE;
		}

		sfd->fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
		if(sfd->fd == -1){
			cout<<"SocketServeBuild Fail!\n";
			return FALSE;
		}
		return TRUE;
	}
	
	BOOL SocketServeBind(const char *port, const char *addr){
		//closesocket��һ�㲻�������رն�����TIME_WAIT�Ĺ��̣�����������ø�socket
		BOOL bReuseaddr = TRUE;
		setsockopt(sfd->fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&bReuseaddr, sizeof(BOOL));

		sfd->addr.sin_family = AF_INET;
		sfd->addr.sin_port = htons(atoi(port));
		//����������ǵ������ģ���������������ip��ַ������Ƕ��������������е�����һ���ip��ַ
		sfd->addr.sin_addr.S_un.S_addr= htonl(INADDR_ANY);
		//sfd->addr.sin_addr.s_addr = inet_addr(addr);
		int ret = bind(sfd->fd, (struct sockaddr *)&sfd->addr, sizeof(sfd->addr));
		if(ret == -1){
			cout<<"SocketServeBind Fail!\n";
			return FALSE;
		}
		return TRUE;
	}
	
	BOOL SocketServeListen(int backlog){
		int ret = listen(sfd->fd, backlog);
		if(ret == -1){
			cout<<"SocketServeListen Fail!\n";
			return FALSE;
		}
		return TRUE;
	}

	BOOL SocketServeAccept(){
		//socklen_t c_len = sizeof(sockaddr_in);
		int c_len = sizeof(sockaddr_in);
		//cfd->fd = accept(sfd->fd, (struct sockaddr *)&cfd->addr, &c_len);
		//�����Ӷ�����ȡ��һ�����ӽ���ͨ��
		cfd->fd = accept(sfd->fd, NULL, NULL);
		getpeername(cfd->fd,(SOCKADDR *)&cfd->addr, &c_len);
		if(cfd->fd == -1){
			errmsg = "SocketServeAccept Fail!";
			return FALSE;
		}
		return TRUE;
    }
	
	string SocketShowAccept(BOOL flag = TRUE){
		char msg[512] = {};
		if(flag){
			sprintf(msg, "client ip[%s], port[%d] Has been Accessed!",
				inet_ntoa(cfd->addr.sin_addr), ntohs(cfd->addr.sin_port));
		}else{
			sprintf(msg, "client ip[%s], port[%d] Has been Disconnecting!",
				inet_ntoa(cfd->addr.sin_addr), ntohs(cfd->addr.sin_port));
		}
		return string(msg);
	}

	BOOL SocketServeSend(const char *type, void *data){
		if(!strcmp(type, "01#")){
			//������������
			char *pbuf = buff;
			memset(pbuf, 0, sizeof(buff));
			char ResHead[11] = {0};
			//%lu : long unsigned int 
			sprintf(ResHead, "%8lu01#", strlen((char *)data));
			memcpy(pbuf, ResHead, sizeof(ResHead));
			memcpy(pbuf + 11, (char *)data, strlen((char *)data));

			cout<<"serve send data:"<<buff<<endl;
			int ret = write(cfd->fd, buff, strlen(buff));
			//int ret = send(cfd->fd, &(buff), sizeof(Msg_buff), 0);
			if(ret < 0){
			errmsg = "Server Write To Client Fail!";
			return FALSE;
			}
			return TRUE;
		}
		errmsg = "[" + std::string(type) + "]���÷�����δʵ��!";
		return FALSE;
	}

	BOOL SocketServeSetup(){
		//8.5��,��Щϵͳδ�ܾ�ȷ��΢��
		//struct timeval tv = {3, 0};
		struct timeval tv;
		tv.tv_sec = 8;
		tv.tv_usec = 500000;
		#if 0
		//��send(), recv()������ʱ���������ԭ���շ��������ڽ��У��������շ�ʱ��
		//����ʱ��, �������������������ȴ����գ������յ�������Ͽ�����ʱ������Ҫ���½���
		//һ�㲻�����ã�����ͻ��˳�ʱ���� accept ���ڷ�����״̬
		if(setsockopt(sfd->fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(tv)) == -1){
			errmsg = "Setup Socket SO_RCVTIMEO Timeout Failed! Error:[" + std::string(strerror(errno)) + "]!";
			return FALSE;
		}
		#endif
		//����ʱ�ޣ�����������Ӧ�����ݹ����������������⣬Ӧ��ʱ,�����·�
		if(setsockopt(sfd->fd, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(tv)) == -1){
			errmsg = "Setup Socket SO_SNDTIMEO Timeout Failed! Error:[" + std::string(strerror(errno)) + "]!";
			return FALSE;
		}
		return TRUE;
	}

	BOOL SocketServeInit(const char *addr = "127.0.0.0", const char *port = "8080", int backlog = 128){
		return SocketServeBuild() == FALSE ? FALSE
			: SocketServeBind(port, addr) == FALSE ? FALSE
			: SocketServeListen(backlog) == FALSE ? FALSE
			: SocketServeSetup() == FALSE ? FALSE
			: TRUE;
	}
		
	//ѭ����ȡ
	int SocketServeRead();

	/**********     SocketClient �ͻ���     **********/
	BOOL SocketClientBuild(){
		WSADATA wsaData;
		WORD sockVersion = MAKEWORD(2, 2);
		int err = WSAStartup(sockVersion, &wsaData);
		if(err != 0)
		{
			cout<<"WSAStartup faild"<<endl;
			return FALSE;
		}
		//IPPROTO_TCP
		cfd->fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(cfd->fd == -1){
			errmsg = "SocketClientBuild Fail!";
			return FALSE;
		}
		return TRUE;
	}

	BOOL SocketClientConnect(const char *addr, const char *port){
		//memset(&cfd->addr, 0, sizeof(cfd->addr));
		cfd->addr.sin_family = AF_INET;
		cfd->addr.sin_port = htons(atoi(port));
		cfd->addr.sin_addr.S_un.S_addr = inet_addr(addr);
		//inet_pton(AF_INET, addr, &cfd->addr.sin_addr.s_addr);
		//int ret = connect(cfd->fd, (struct sockaddr *)(&(cfd->addr)),sizeof(cfd->addr));
		int ret = connect(cfd->fd, (LPSOCKADDR)(&(cfd->addr)),sizeof(cfd->addr));
		if(ret == -1){
			errmsg = "SocketClientConnect Fail!";
			return FALSE;
		}else if(ret != 0){
			errmsg = "SocketClientConnect Error:[" + std::string(strerror(ret)) + "]!";
			return FALSE;
		}
		return TRUE;
    }

	BOOL SocketClientInit(const char *addr = "127.0.0.1", const char *port = "8080"){
		return SocketClientBuild() == FALSE ? FALSE
			: SocketClientConnect(addr, port) == FALSE ? FALSE
			: TRUE;
    }

	BOOL SocketClientSend(const char *type, void *data){
		if(!strcmp(type, "01#")){
			//������������
			char *pbuf = buff;
			memset(pbuf, 0, sizeof(buff));
			char ResHead[11] = {0};
			sprintf(ResHead, "%8lu01#", strlen((char *)data));
			memcpy(pbuf, ResHead, sizeof(ResHead));
			memcpy(pbuf + 11, (char *)data, strlen((char *)data));

			cout<<"clint send data: "<<buff<<endl;
			int ret = write(cfd->fd, buff, strlen(buff));
			//int ret = send(cfd->fd, &(buff), sizeof(Msg_buff), 0);
			if(ret < 0){
				errmsg = "Server Write To Client Fail!";
				return FALSE;
			}
			return TRUE;
		}
		errmsg = "[" + std::string(type) + "], ��������δʵ��!";
		return FALSE;
	}

	int SocketClientRead();

	//
};

//extern "C" _declspec(dllexport) void __stdcall show();

DLL_FAPI void show();

#endif
