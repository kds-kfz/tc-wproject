// 3.һ���򵥵Ĵ��ֳ���.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "3.һ���򵥵Ĵ��ֳ���.h"
#include<string>

//���ں����ĺ���ԭ��
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

//����: ��ʱ������
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//Сʱ�Ӷ�ʱ������
LRESULT CALLBACK WndProc1(HWND, UINT, WPARAM, LPARAM);


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	char szClassName[] = "MainWClass";

	WNDCLASSEX wndclass;
	
	//�����������ڵĲ������ WNDCLASSEX �ṹ
	wndclass.cbSize = sizeof(wndclass);			//�ṹ���С
	wndclass.style = CS_HREDRAW|CS_VREDRAW;		//��������������Ĵ��ھ��еķ��
	//wndclass.lpfnWndProc = MainWndProc;			//���ں���ָ��
	wndclass.lpfnWndProc = WndProc1;
	wndclass.cbClsExtra = 0;					//û�ж�������ڴ�
	wndclass.cbWndExtra = 0;					//û�ж���Ĵ����ڴ�
	wndclass.hInstance = hInstance;				//ʵ�����
	wndclass.hIcon = ::LoadIcon(hInstance, (LPSTR)IDI_TYPER);				//ʹ��Ԥ����ͼ��
	wndclass.hCursor = ::LoadCursor(NULL, IDC_ARROW);				//ʹ��Ԥ����Ĺ��
	wndclass.hbrBackground = (HBRUSH)::GetStockObject(COLOR_3DFACE);	//ʹ�ð�ɫ������ˢ
	wndclass.lpszMenuName = (LPSTR)IDR_TYPER;//��ָ���˵�
	wndclass.lpszClassName = szClassName;//�����������
	wndclass.hIconSm = NULL;//û�����Сͼ��

	//ע�����������
	::RegisterClassExA(&wndclass);

	//����������
	HWND hwnd = ::CreateWindowEx(
		0,//��չ��ʽ
		szClassName,		//����
		"���ز��Կͻ���",	//����
		WS_OVERLAPPEDWINDOW,//���ڷ��
		CW_USEDEFAULT,		//��ʼ x ����
		CW_USEDEFAULT,		//��ʼ y ����
		CW_USEDEFAULT,		//���
		CW_USEDEFAULT,		//�߶�
		NULL,				//�����ھ��
		NULL,				//�˵����
		hInstance,			//����ʵ�����
		NULL				//�û�����
		);

	if(hwnd == NULL){
		::MessageBoxA(NULL, "�������ڴ���!", "error", MB_OK);
		return -1;
	}

	//��ʾ���ڣ�ˢ�´��ڿͻ���
	::ShowWindow(hwnd, nCmdShow);
	::UpdateWindow(hwnd);

	//����Ϣ������ȡ����Ϣ���������ں�������ֱ�� GetMessage ���� FALSE��������Ϣѭ��
	MSG msg;
	while(::GetMessageA(&msg, NULL, 0, 0)){
		//�Ѽ������뷭���Ϊ�ɴ��ݵ���Ϣ
		::TranslateMessage(&msg);

		//����Ϣת������Ӧ�Ĵ��ں��� MainWndProc
		::DispatchMessageA(&msg);
	}

	//��getmessage����false �ǳ������,��ֵ���� PostQuitMessage
	return msg.wParam;

}

#define IDT_TIMER1 1
//��ʱ��
LRESULT __stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM IParam){
	static int nNum;//����
	static int bSetTimer;//ָʾ�Ƿ�װ�˶�ʱ��
	char szText[56];
	PAINTSTRUCT ps;
	HDC hdc;
	switch(message){
	case WM_CREATE:
		bSetTimer = FALSE;
		break;
	case WM_PAINT:
		hdc = ::BeginPaint(hWnd, &ps);
		wsprintf(szText, "����: %d", nNum);
		::TextOutA(hdc, 10, 10, szText, strlen(szText));
		::EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:
		if(wParam == IDT_TIMER1){
			hdc = ::GetDC(hWnd);
			wsprintf(szText, "����: %d", nNum++);
			::TextOutA(hdc, 10, 10, szText, strlen(szText));

			//��һ��"�"������
			::MessageBeep(MB_OK);
		}
		break;
	case WM_LBUTTONDOWN:
		if(bSetTimer){
			//����һ���Ѿ���װ�Ķ�ʱ��
			::KillTimer(hWnd, IDT_TIMER1);
			bSetTimer = FALSE;
		}else{
			//��װһ��ʱ��������250ms�Ķ�ʱ��
			if(::SetTimer(hWnd, IDT_TIMER1, 250, NULL) == NULL){
				::MessageBoxA(hWnd, "��װ��ʱ��ʧ��", "03Timer", MB_OK);
			}else{
				bSetTimer = TRUE;
			}
		}
		break;
	case WM_CLOSE:
		if(bSetTimer){
			::KillTimer(hWnd, IDT_TIMER1);
		}
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
	}
	return ::DefWindowProcA(hWnd, message, wParam, IParam);
}

//����ʼ�յ����
void DrawClockFace(HDC hdc){
	const int Sqsize = 20;
	static POINT pt[]={
		0,450,		//12��
		225,390,		//1��
		390,255,		//2��
		450,0,		//3��
		390,-225,	//4��
		225,-390,	//5��
		0,-450,		//6��
		-225,-390,	//7��
		-390,-255,	//8��
		-450,0,		//9��
		-390,225,	//10��
		-255,390		//11��
	};

	//ѡ��һ����ˢ
	::SelectObject(hdc, ::GetStockObject(BLACK_BRUSH));

	//��12��Բ
	for(int i = 0; i < 12; i++){
		::Ellipse(hdc, pt[i].x - Sqsize, pt[i].y + Sqsize,
			pt[i].x + Sqsize, pt[i].y - Sqsize);
	}
}

//���õѿ�������
void SetIsotropic(HDC hdc, int cx, int cy){
	//��������ϵ��ӳ�䷽ʽ���û��Զ��壬x��y���߼���λ��ͬ
	::SetMapMode(hdc, MM_ISOTROPIC);
	//��������ϵ�߼���λ
	::SetWindowExtEx(hdc, 1000, 1000, NULL);
	//����x��y�᷽�������ϵ������Χ
	::SetViewportExtEx(hdc, cx, -cy, NULL);
	//����ԭ������
	::SetViewportOrgEx(hdc, cx/2, cy/2, NULL);
}

//��һ���߳�Ϊ100�������Σ��ڲ��Ժ�ɫ��ˢ���
void OnPaint(HWND hwnd){
	RECT rt;
	::GetClientRect(hwnd, &rt);
	int xcent = rt.right/2;
	int ycent = rt.bottom/2;

	PAINTSTRUCT ps;
	HDC hdc = ::BeginPaint(hwnd, &ps);

	//����һ����ɫ��ˢ�����豸
	HBRUSH hbrush = ::CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH holdbrush = (HBRUSH)::SelectObject(hdc, hbrush);

	//��xcent, ycentΪ���ģ���һ���߳�Ϊ100��������
	::Rectangle(hdc, xcent - 50, ycent + 50, xcent +50, ycent - 50);
	::SelectObject(hdc, holdbrush);
	::DeleteObject(hbrush);
	::EndPaint(hwnd, &ps);
}

//��һ����Բ
void OnPaint2(HWND hwnd){
	PAINTSTRUCT ps;
	HDC hdc = ::BeginPaint(hwnd, &ps); 
	
	//��ȡ�ͻ����Ĵ�С
	RECT rt;
	::GetClientRect(hwnd, &rt);
	int cx = rt.right;
	int cy = rt.bottom;

	//���ÿͻ������߼���СΪ 500 �� 500��ԭ��Ϊ (0, 0)
	//��������ӳ�䷽ʽ,�û��Զ���Ϊ
	::SetMapMode(hdc, MM_ANISOTROPIC);
	//�����߼���λ
	::SetWindowExtEx(hdc, 500, 500, NULL);
	//�������귽�򣬺�����ϵ�ķ�Χ
	::SetViewportExtEx(hdc, cx, cy, NULL);
	//����ԭ������
	::SetViewportOrgEx(hdc, 0, 0, NULL);

	//�������ͻ���Ϊ�߽续һ����Բ
	::Ellipse(hdc, 0, 0, 500, 500);
	::EndPaint(hwnd,&ps);

}

//ʹ��GDI������ʱ�롢���롢����
void DrawHand(HDC hdc, int nLength, int nWidth, int nDegrees, COLORREF clrColor){
	//���Ƕ�nDegreesת��Ϊ���ȣ�2 * 3.1415926 / 360 = 0.0174533
	double nReadians = (double)nDegrees * 0.0174533;

	//��������
	POINT pt[2];
	pt[0].x = (int)(nLength * sin(nReadians));
	pt[0].y = (int)(nLength * cos(nReadians));
	pt[1].x = - pt[0].x/5;
	pt[1].y = - pt[0].y/5;

	//�������ʣ���ѡ��DC�ṹ��
	HPEN hPen = ::CreatePen(PS_SOLID, nWidth, clrColor);
	HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);

	//����
	::MoveToEx(hdc, pt[0].x, pt[0].y, NULL);
	::LineTo(hdc, pt[1].x, pt[1].y);

	::SelectObject(hdc, hOldPen);
	::DeleteObject(hPen);
}

//��һ��windows֪ͨʱ��
static int s_nPreHour;		//Сʱ
static int s_nPreMinute;	//����
static int s_nPreSecond;	//��

//���ڿͻ����Ĵ�С
static int s_cxClient;
static int s_cyClient;

//�Ƿ�λ�����
static BOOL s_bTopMost;

//ʱ�Ӷ�ʱ�����
#define IDT_CLOCK 1

LRESULT CALLBACK WndProc1(HWND hwnd, UINT message, WPARAM wParam, LPARAM IParam){
	switch(message){
	case WM_CREATE:
		{
			SYSTEMTIME time;
			//����ʱ��
			::GetLocalTime(&time);
			s_nPreHour = time.wHour%12;
			s_nPreMinute = time.wMinute;
			s_nPreSecond = time.wSecond;
			//������ʱ��
			::SetTimer(hwnd, IDT_CLOCK, 1000, NULL);
			return 0;
		}
	case WM_CLOSE:
		{
			::KillTimer(hwnd, IDT_CLOCK);
			::DestroyWindow(hwnd);
			return 0;
		}
	case WM_DESTROY:
		{
			::PostQuitMessage(0);
			return 0;
		}
	case WM_SIZE:
		{
			s_cxClient = LOWORD(IParam);
			s_cyClient = HIWORD(IParam);
			return 0;
		}
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = ::BeginPaint(hwnd, &ps);

			//��������ϵ
			SetIsotropic(hdc, s_cxClient, s_cyClient);

			//����ʱ�����
			DrawClockFace(hdc);

			//����ָ��
			//����1Сʱ��ʱ����30��(360/12)������1����ʱ����0.5��(30/60)
			DrawHand(hdc, 200, 8, s_nPreHour * 30 + s_nPreMinute/2, RGB(0, 0, 0));
			//����1���ӣ�������6��(360/6)
			DrawHand(hdc, 400, 6, s_nPreMinute * 6, RGB(0, 0, 0));
			//����1���ӣ�������6��(360/6)
			DrawHand(hdc, 400, 6, s_nPreSecond * 6, RGB(0, 0, 0));
			::EndPaint(hwnd, &ps);
			return 0;
		}
	case WM_TIMER:
		{
			//������ڴ�����С��״̬����ʲôҲ����
			if(::IsIconic(hwnd)){
				return 0;
			}
			//��ȡϵͳʱ��
			SYSTEMTIME time;
			::GetLocalTime(&time);

			//��������ϵ
			HDC hdc = ::GetDC(hwnd);
			//��������ϵ
			SetIsotropic(hdc, s_cxClient, s_cyClient);

			//��COLOR_3DFACE Ϊ�����Ϳ��Բ���ָ����
			//��Ϊ���ڱ�����ɫ�� COLOR_3DFACE
			COLORREF crfColor = ::GetSysColor(COLOR_3DFACE);

			//������Ӹı�Ļ��Ͳ���ʱ��ͷ���
			if(time.wMinute != s_nPreMinute){
				//����ʱ��ͷ���
				
				DrawHand(hdc, 200, 8, s_nPreHour * 30 + s_nPreMinute / 2, crfColor);
				DrawHand(hdc, 400, 6, s_nPreMinute * 6, crfColor);
				s_nPreHour = time.wHour;
				s_nPreMinute = time.wMinute;
				
			}
			
			//�������ı�Ļ��Ͳ������룬Ȼ���ػ�������
			if(time.wSecond != s_nPreSecond){
				//��������
				DrawHand(hdc, 400, 1, s_nPreSecond * 6, crfColor);

				//�ػ�������
				DrawHand(hdc, 400, 1, time.wSecond * 6, RGB(0, 0, 0));
				DrawHand(hdc, 200, 8, time.wHour * 30 + time.wMinute / 2, RGB(0, 0, 0));
				DrawHand(hdc, 400, 6, time.wMinute * 6, RGB(0, 0, 0));
				s_nPreSecond = time.wSecond;
				return 0;
			
			}
		}
	}
	return ::DefWindowProcA(hwnd, message, wParam, IParam);
}

//���ں���
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM IParam){
	static std::string str;
	char szText[] = "��򵥵Ĵ��ڳ���";
	switch(message){
		case WM_CREATE:
		{
			//���ô��ڵı���
			::SetWindowTextA(hwnd, "���ª�Ĵ��ֳ���");
			return 0;
		}
		case WM_PAINT:
		{
#if 0
			char szText[] = "��ã�����!";
			HDC hdc;
			PAINTSTRUCT ps;

			//����Ч�Ŀͻ��������Ч����ȡ���豸�������
			hdc = ::BeginPaint(hwnd, &ps);
			//��ʾ����6
			//::TextOutA(hdc, 10, 10, str.c_str(), str.length());
			TextOutA(hdc, 10, 10, szText, strlen(szText));
			::EndPaint(hwnd, &ps);
			return 0;
#endif
#if 0
			HDC hdc;
			PAINTSTRUCT ps;
			RECT rt;
			int x, y;

			hdc = ::BeginPaint(hwnd, &ps);
			//��ȡ�ͻ�����С
			::GetClientRect(hwnd, &rt);

			//�Զ��廭��
			HPEN hpen = ::CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
			HPEN holdpen = (HPEN)::SelectObject(hdc, hpen);
			//::SelectObject(hdc, holdpen);

			//����
			for(x = 0; x < rt.right - rt.left; x += 50){
				::MoveToEx(hdc, x, 0, NULL);
				::LineTo(hdc, x, rt.bottom - rt.top);
			}
			//����
			for(y = 0; y < rt.bottom - rt.top; y +=50 ){
				::MoveToEx(hdc, 0, y, NULL);
				::LineTo(hdc, rt.right - rt.left, y);
			}
			
			::DeleteObject(hpen);
#endif
				//OnPaint(hwnd);//������
				//OnPaint2(hwnd);//����Բ


			HDC hdc;
			PAINTSTRUCT ps;
			RECT rt;
			hdc = ::BeginPaint(hwnd, &ps);
			::GetClientRect(hwnd, &rt);
			SetIsotropic(hdc, rt.right - rt.left, rt.bottom - rt.top);
			DrawClockFace(hdc);
			::EndPaint(hwnd, &ps);
		}
		case WM_CHAR:
		{
			//���� ansi ��
			str += char(wParam);
			//�������ͻ�����Ч
			::InvalidateRect(hwnd, NULL, 0);
			return 0;
		}
		case WM_DESTROY://�������ٴ���
			//����Ϣ����Ͷ��һ��WM_QUIT��Ϣ����ʹ GetMessage ��������0��������Ϣѭ��
			::PostQuitMessage(0);
			return 0;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
			case ID_FILE_EXIT:
				//��hwndָ���Ĵ��ڷ���һ�� WM_CLOSE ��Ϣ
				//����������Ϣ���еȴ�GetMessage����ȡ��,����ֱ�Ӵ����˴��ں���
				::SendMessageA(hwnd, WM_CLOSE, 0, 0);
				break;
			}
			return 0;
		case WM_CLOSE:
			//�رյ�ǰ����
			::DestroyWindow(hwnd);
			return 0;
		case WM_LBUTTONDOWN:
			{
#if 0
				char szPoint[56];
				wsprintf(szPoint, "x = %d, y = %d", LOWORD(IParam), HIWORD(IParam));
				str = szPoint;
				if(wParam & MK_SHIFT)
					str += "   shift key is down";
				::InvalidateRect(hwnd, NULL, 1);
				return 0;
#endif
				char szText[] = "��Һ�";
				HDC hdc = ::GetWindowDC(hwnd);
				::TextOutA(hdc, 10, 10, szText, strlen(szText));
				::ReleaseDC(hwnd, hdc);
				return 0;
			}
	}

	//�����ǲ��������Ϣ����ϵͳ��Ĭ�ϴ���
	return ::DefWindowProcA(hwnd, message, wParam, IParam);

}