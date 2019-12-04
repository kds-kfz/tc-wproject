// 3.一个简单的打字程序.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "3.一个简单的打字程序.h"
#include<string>

//窗口函数的函数原型
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	char szClassName[] = "MainWClass";

	WNDCLASSEX wndclass;
	
	//用描述主窗口的参数填充 WNDCLASSEX 结构
	wndclass.cbSize = sizeof(wndclass);			//结构体大小
	wndclass.style = CS_HREDRAW|CS_VREDRAW;		//从这个窗口派生的窗口具有的风格
	wndclass.lpfnWndProc = MainWndProc;			//窗口函数指针
	wndclass.cbClsExtra = 0;					//没有额外的类内存
	wndclass.cbWndExtra = 0;					//没有额外的窗口内存
	wndclass.hInstance = hInstance;				//实例句柄
	wndclass.hIcon = ::LoadIcon(hInstance, (LPSTR)IDI_TYPER);				//使用预定义图标
	wndclass.hCursor = ::LoadCursor(NULL, IDC_ARROW);				//使用预定义的光标
	wndclass.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);	//使用白色背景画刷
	wndclass.lpszMenuName = (LPSTR)IDR_TYPER;//不指定菜单
	wndclass.lpszClassName = szClassName;//窗口类的名称
	wndclass.hIconSm = NULL;//没有类的小图标

	//注册这个窗口类
	::RegisterClassExA(&wndclass);

	//创建主窗口
	HWND hwnd = ::CreateWindowEx(
		0,//扩展样式
		szClassName,		//类名
		"挂载测试客户端",	//标题
		WS_OVERLAPPEDWINDOW,//窗口风格
		CW_USEDEFAULT,		//初始 x 坐标
		CW_USEDEFAULT,		//初始 y 坐标
		CW_USEDEFAULT,		//宽度
		CW_USEDEFAULT,		//高度
		NULL,				//父窗口句柄
		NULL,				//菜单句柄
		hInstance,			//程序实例句柄
		NULL				//用户数据
		);

	if(hwnd == NULL){
		::MessageBoxA(NULL, "创建窗口错误!", "error", MB_OK);
		return -1;
	}

	//显示窗口，刷新窗口客户区
	::ShowWindow(hwnd, nCmdShow);
	::UpdateWindow(hwnd);

	//从信息队列中取出消息，交给窗口函数处理，直到 GetMessage 返回 FALSE，结束消息循环
	MSG msg;
	while(::GetMessageA(&msg, NULL, 0, 0)){
		//把键盘输入翻译成为可传递的消息
		::TranslateMessage(&msg);

		//将消息转发到相应的窗口函数 MainWndProc
		::DispatchMessageA(&msg);
	}

	//当getmessage返回false 是程序结束,把值传给 PostQuitMessage
	return msg.wParam;

}

//画一个边长为100的正方形，内部以红色画刷填充
void OnPaint(HWND hwnd){
	RECT rt;
	::GetClientRect(hwnd, &rt);
	int xcent = rt.right/2;
	int ycent = rt.bottom/2;

	PAINTSTRUCT ps;
	HDC hdc = ::BeginPaint(hwnd, &ps);

	//创建一个单色画刷加入设备
	HBRUSH hbrush = ::CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH holdbrush = (HBRUSH)::SelectObject(hdc, hbrush);

	//以xcent, ycent为中心，画一个边长为100的正方形
	::Rectangle(hdc, xcent - 50, ycent + 50, xcent +50, ycent - 50);
	::SelectObject(hdc, holdbrush);
	::DeleteObject(hbrush);
	::EndPaint(hwnd, &ps);
}

//窗口函数
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM IParam){
	static std::string str;
	char szText[] = "最简单的窗口程序";
	switch(message){
		case WM_CREATE:
		{
			//设置窗口的标题
			::SetWindowTextA(hwnd, "最简陋的大字程序");
			return 0;
		}
		case WM_PAINT:
		{
#if 0
			char szText[] = "你好，世界!";
			HDC hdc;
			PAINTSTRUCT ps;

			//是无效的客户区变得有效，并取得设备环境句柄
			hdc = ::BeginPaint(hwnd, &ps);
			//显示文字6
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
			//获取客户区大小
			::GetClientRect(hwnd, &rt);

			//自定义画笔
			HPEN hpen = ::CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
			HPEN holdpen = (HPEN)::SelectObject(hdc, hpen);
			//::SelectObject(hdc, holdpen);

			//划列
			for(x = 0; x < rt.right - rt.left; x += 50){
				::MoveToEx(hdc, x, 0, NULL);
				::LineTo(hdc, x, rt.bottom - rt.top);
			}
			//画行
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
			//保存 ansi 码
			str += char(wParam);
			//是整个客户区无效
			::InvalidateRect(hwnd, NULL, 0);
			return 0;
		}
		case WM_DESTROY://正在销毁窗口
			//向消息队列投递一个WM_QUIT消息，促使 GetMessage 函数返回0，结束消息循环
			::PostQuitMessage(0);
			return 0;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
			case ID_FILE_EXIT:
				//向hwnd指定的窗口发送一个 WM_CLOSE 消息
				//并不进入消息队列等待GetMessage函数取出,而是直接传给了窗口函数
				::SendMessageA(hwnd, WM_CLOSE, 0, 0);
				break;
			}
			return 0;
		case WM_CLOSE:
			//关闭当前工作
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
				char szText[] = "大家好";
				HDC hdc = ::GetWindowDC(hwnd);
				::TextOutA(hdc, 10, 10, szText, strlen(szText));
				::ReleaseDC(hwnd, hdc);
				return 0;
			}
	}

	//将我们不处理的消息交给系统做默认处理
	return ::DefWindowProcA(hwnd, message, wParam, IParam);

}