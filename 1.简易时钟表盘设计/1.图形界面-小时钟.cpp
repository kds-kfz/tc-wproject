// 3.һ���򵥵Ĵ��ֳ���.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "3.һ���򵥵Ĵ��ֳ���.h"
#include<string>

//���ں����ĺ���ԭ��
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	char szClassName[] = "MainWClass";

	WNDCLASSEX wndclass;
	
	//�����������ڵĲ������ WNDCLASSEX �ṹ
	wndclass.cbSize = sizeof(wndclass);			//�ṹ���С
	wndclass.style = CS_HREDRAW|CS_VREDRAW;		//��������������Ĵ��ھ��еķ��
	wndclass.lpfnWndProc = MainWndProc;			//���ں���ָ��
	wndclass.cbClsExtra = 0;					//û�ж�������ڴ�
	wndclass.cbWndExtra = 0;					//û�ж���Ĵ����ڴ�
	wndclass.hInstance = hInstance;				//ʵ�����
	wndclass.hIcon = ::LoadIcon(hInstance, (LPSTR)IDI_TYPER);				//ʹ��Ԥ����ͼ��
	wndclass.hCursor = ::LoadCursor(NULL, IDC_ARROW);				//ʹ��Ԥ����Ĺ��
	wndclass.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);	//ʹ�ð�ɫ������ˢ
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
				OnPaint(hwnd);
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