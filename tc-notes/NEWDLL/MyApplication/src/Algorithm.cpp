//Algorithm.cpp
#include "Algorithm.h"
//#include <winsock2.h>
//#pragma comment(lib, "ws2_32")
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

int Socket::SocketServeRead()
{
	memset(buff, 0, sizeof(buff));
	int count = 0;
    //有可能接收的请求字节数超长，故先检验包体长度是否合法
    if((count = read(cfd->fd, buff, 8)) != 8){
        cout<<"Recv Header Error, break!\n";
        return -1;
    }
    //这里要校验前8字节是否都为纯数字
    unsigned long pkg_len = atol(buff);
    cout<<"收到请求包体长度:"<<pkg_len<<endl;
    if(pkg_len <= 0 || pkg_len > REQ_MAX_LEN){
        cout<<"The Length oF Reqmsg Is Illigle Max "<<REQ_MAX_LEN<<endl;
        return -2;
    }
    memset(buff, 0, sizeof(buff));
    int nbyte = 3;
    unsigned int tpkg_len = pkg_len;

	//每次最大读取 1024 字节
    char *pbuf = buff;
    while(nbyte){
        char TempBuff[1024] = {0};
        count = read(cfd->fd, TempBuff, nbyte);
        if(count < 0){
            if(errno == EINTR){
                count = 0;
                errmsg = "SocketServeRead Read EINTR!";
                return -3;
            }else{
                char temp[12] = {0};
                sprintf(temp, "%d", count);
                errmsg = "Recv Type is None Or Error:[" + string(strerror(errno)) + "][" + string(temp) + "]!";
                return -4;
            }
        }else if(count == 0){
            errmsg = SocketShowAccept(false);                
            return 1;
            //break;
        }/*
        else{
            errmsg = "SocketServeRead Read Error!";
            return -5;
        }*/
        if(nbyte == 3 && TempBuff[2] == '#'){
            strcpy(this->type, TempBuff);
            nbyte = tpkg_len > sizeof(TempBuff) ? sizeof(TempBuff) : tpkg_len;
        }else{
            tpkg_len -= count;
            nbyte = tpkg_len > sizeof(TempBuff) ? sizeof(TempBuff) : tpkg_len;
            memcpy(pbuf, TempBuff, count);
            pbuf += count;
        }
    }
}

int Socket::SocketClientRead()
{
	int count = 0;
    memset(buff, 0, sizeof(buff));

    //有可能接收的请求字节数超长，故先检验包体长度是否合法
    if((count = read(cfd->fd, buff, 8)) != 8){
        cout<<"Recv Header Error, break!\n";
        return -1;
    }
    //这里要校验前8字节是否都为纯数字
    unsigned long pkg_len = atol(buff);
    if(pkg_len <= 0 || pkg_len > REQ_MAX_LEN){
        cout<<"The Length oF Reqmsg Is Illigle Max "<<REQ_MAX_LEN<<endl;
        return -2;
    }

    memset(this->type, 0, sizeof(this->type));
    memset(buff, 0, sizeof(buff));
    int nbyte = 3;
    //每次最大读取 1024 字节
    //char *pbuf = buff;
    unsigned int tpkg_len = pkg_len;
    cout<<"收到应答包体长度:"<<pkg_len<<endl;

	char *pbuf = buff;
	while(nbyte){
        char TempBuff[1024] = {0};
        count = read(cfd->fd, TempBuff, nbyte);
        if(count < 0){
            if(errno == EINTR){
                count = 0;
                errmsg = "SocketServeRead Read EINTR!";
                return -3;
            }else{
                char temp[12] = {0};
                sprintf(temp, "%d", count);
                errmsg = "Recv Type is None Or Error:[" + string(strerror(errno)) + "][" + string(temp) + "]!";
                return -4;
            }
        }else if(count == 0){
            errmsg = "SocketClientRead the EOF or Read 0 Byte!";
            //return 1;
            break;
        }/*
        else if(count == -1){
            errmsg = "SocketClientRead Error:[" + string(strerror(errno)) + "]!";
        }*/
        
        if(nbyte == 3 && TempBuff[2] == '#'){
            strcpy(this->type, TempBuff);
            //第一次读取消息内容，默认读取整个包长
            //如果包长大于缓存大小，则每次最多接收缓存大小这么长的字节
            //及时实际包长很长，没此最大获取 sizeof(TempBuff) 个字节，直到获取完毕
            nbyte = tpkg_len > sizeof(TempBuff) ? sizeof(TempBuff) : tpkg_len;
        }else{
            tpkg_len -= count;
            nbyte = tpkg_len > sizeof(TempBuff) ? sizeof(TempBuff) : tpkg_len;
            memcpy(pbuf, TempBuff, count);
            pbuf += count;
        }
    }

	return 1;
}

DLL_FAPI void show()
{
	cout<<"显示动态调用单个函数"<<endl;
}
