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

int Socket::SocketServeRead()
{
	memset(buff, 0, sizeof(buff));
	int count = 0;
    //�п��ܽ��յ������ֽ������������ȼ�����峤���Ƿ�Ϸ�
    if((count = read(cfd->fd, buff, 8)) != 8){
        cout<<"Recv Header Error, break!\n";
        return -1;
    }
    //����ҪУ��ǰ8�ֽ��Ƿ�Ϊ������
    unsigned long pkg_len = atol(buff);
    cout<<"�յ�������峤��:"<<pkg_len<<endl;
    if(pkg_len <= 0 || pkg_len > REQ_MAX_LEN){
        cout<<"The Length oF Reqmsg Is Illigle Max "<<REQ_MAX_LEN<<endl;
        return -2;
    }
    memset(buff, 0, sizeof(buff));
    int nbyte = 3;
    unsigned int tpkg_len = pkg_len;

	//ÿ������ȡ 1024 �ֽ�
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

    //�п��ܽ��յ������ֽ������������ȼ�����峤���Ƿ�Ϸ�
    if((count = read(cfd->fd, buff, 8)) != 8){
        cout<<"Recv Header Error, break!\n";
        return -1;
    }
    //����ҪУ��ǰ8�ֽ��Ƿ�Ϊ������
    unsigned long pkg_len = atol(buff);
    if(pkg_len <= 0 || pkg_len > REQ_MAX_LEN){
        cout<<"The Length oF Reqmsg Is Illigle Max "<<REQ_MAX_LEN<<endl;
        return -2;
    }

    memset(this->type, 0, sizeof(this->type));
    memset(buff, 0, sizeof(buff));
    int nbyte = 3;
    //ÿ������ȡ 1024 �ֽ�
    //char *pbuf = buff;
    unsigned int tpkg_len = pkg_len;
    cout<<"�յ�Ӧ����峤��:"<<pkg_len<<endl;

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
            //��һ�ζ�ȡ��Ϣ���ݣ�Ĭ�϶�ȡ��������
            //����������ڻ����С����ÿ�������ջ����С��ô�����ֽ�
            //��ʱʵ�ʰ����ܳ���û������ȡ sizeof(TempBuff) ���ֽڣ�ֱ����ȡ���
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
	cout<<"��ʾ��̬���õ�������"<<endl;
}
