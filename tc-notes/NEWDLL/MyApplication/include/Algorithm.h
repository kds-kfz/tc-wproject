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


#include<iostream>
#include<io.h>
#include<string>
#include<vector>

#include <cstring>
#include <atlstr.h>
using namespace std;

//这两个头文件注意先后顺序
//动态库隐式链接
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

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 套 接 字 处 理 类 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//这是socket通信结构体，服务端于客户端公用
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
	string errmsg;			//错误消息
	char type[4];			//消息类型
	char buff[BUFF_SIZE];	//缓存内容
	Total_msg *sfd;			//服务端数据结构
	Total_msg *cfd;			//客户端数据结构
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

	/**********     SocketServe 服务端     **********/
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
		//closesocket（一般不会立即关闭而经历TIME_WAIT的过程）后想继续重用该socket
		BOOL bReuseaddr = TRUE;
		setsockopt(sfd->fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&bReuseaddr, sizeof(BOOL));

		sfd->addr.sin_family = AF_INET;
		sfd->addr.sin_port = htons(atoi(port));
		//如果服务器是单网卡的，则就是这块网卡的ip地址；如果是多网卡，则是其中的任意一块的ip地址
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
		//从连接队列中取第一个连接进行通信
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
			//测试请求类型
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
		errmsg = "[" + std::string(type) + "]，该服务尚未实现!";
		return FALSE;
	}

	BOOL SocketServeSetup(){
		//8.5秒,有些系统未能精确到微秒
		//struct timeval tv = {3, 0};
		struct timeval tv;
		tv.tv_sec = 8;
		tv.tv_usec = 500000;
		#if 0
		//在send(), recv()过程有时由于网络等原因，收发不能如期进行，可设置收发时限
		//接受时限, 服务器接收请求，阻塞等待接收，当接收到半网络断开，超时后则需要重新接收
		//一般不用设置，否则客户端超时，则 accept 处于非阻塞状态
		if(setsockopt(sfd->fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(tv)) == -1){
			errmsg = "Setup Socket SO_RCVTIMEO Timeout Failed! Error:[" + std::string(strerror(errno)) + "]!";
			return FALSE;
		}
		#endif
		//发送时限，服务器发送应答，数据过长，或者网络问题，应答超时,则重新发
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
		
	//循环读取
	int SocketServeRead();

	/**********     SocketClient 客户端     **********/
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
			//测试请求类型
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
		errmsg = "[" + std::string(type) + "], 该请求尚未实现!";
		return FALSE;
	}

	int SocketClientRead();

	//
};

//extern "C" _declspec(dllexport) void __stdcall show();

DLL_FAPI void show();

#endif
