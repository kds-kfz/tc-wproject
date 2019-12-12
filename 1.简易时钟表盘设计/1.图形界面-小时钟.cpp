// 3.一个简单的打字程序.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "3.一个简单的打字程序.h"
#include<string>

//窗口函数的函数原型
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

//测试: 定时器窗口
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//小时钟定时器窗口
LRESULT CALLBACK WndProc1(HWND, UINT, WPARAM, LPARAM);


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	char szClassName[] = "MainWClass";

	WNDCLASSEX wndclass;
	
	//用描述主窗口的参数填充 WNDCLASSEX 结构
	wndclass.cbSize = sizeof(wndclass);			//结构体大小
	wndclass.style = CS_HREDRAW|CS_VREDRAW;		//从这个窗口派生的窗口具有的风格
	//wndclass.lpfnWndProc = MainWndProc;			//窗口函数指针
	wndclass.lpfnWndProc = WndProc1;
	wndclass.cbClsExtra = 0;					//没有额外的类内存
	wndclass.cbWndExtra = 0;					//没有额外的窗口内存
	wndclass.hInstance = hInstance;				//实例句柄
	wndclass.hIcon = ::LoadIcon(hInstance, (LPSTR)IDI_TYPER);				//使用预定义图标
	wndclass.hCursor = ::LoadCursor(NULL, IDC_ARROW);				//使用预定义的光标
	wndclass.hbrBackground = (HBRUSH)::GetStockObject(COLOR_3DFACE);	//使用白色背景画刷
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

#define IDT_TIMER1 1
//定时器
LRESULT __stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM IParam){
	static int nNum;//计数
	static int bSetTimer;//指示是否安装了定时器
	char szText[56];
	PAINTSTRUCT ps;
	HDC hdc;
	switch(message){
	case WM_CREATE:
		bSetTimer = FALSE;
		break;
	case WM_PAINT:
		hdc = ::BeginPaint(hWnd, &ps);
		wsprintf(szText, "计数: %d", nNum);
		::TextOutA(hdc, 10, 10, szText, strlen(szText));
		::EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:
		if(wParam == IDT_TIMER1){
			hdc = ::GetDC(hWnd);
			wsprintf(szText, "计数: %d", nNum++);
			::TextOutA(hdc, 10, 10, szText, strlen(szText));

			//发一声"嘟"的声音
			::MessageBeep(MB_OK);
		}
		break;
	case WM_LBUTTONDOWN:
		if(bSetTimer){
			//撤销一个已经安装的定时器
			::KillTimer(hWnd, IDT_TIMER1);
			bSetTimer = FALSE;
		}else{
			//安装一个时间周期是250ms的定时器
			if(::SetTimer(hWnd, IDT_TIMER1, 250, NULL) == NULL){
				::MessageBoxA(hWnd, "安装定时器失败", "03Timer", MB_OK);
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

//绘制始终的外观
void DrawClockFace(HDC hdc){
	const int Sqsize = 20;
	static POINT pt[]={
		0,450,		//12点
		225,390,		//1点
		390,255,		//2点
		450,0,		//3点
		390,-225,	//4点
		225,-390,	//5点
		0,-450,		//6点
		-225,-390,	//7点
		-390,-255,	//8点
		-450,0,		//9点
		-390,225,	//10点
		-255,390		//11点
	};

	//选择一个画刷
	::SelectObject(hdc, ::GetStockObject(BLACK_BRUSH));

	//画12个圆
	for(int i = 0; i < 12; i++){
		::Ellipse(hdc, pt[i].x - Sqsize, pt[i].y + Sqsize,
			pt[i].x + Sqsize, pt[i].y - Sqsize);
	}
}

//设置笛卡尔坐标
void SetIsotropic(HDC hdc, int cx, int cy){
	//设置坐标系的映射方式，用户自定义，x、y轴逻辑单位相同
	::SetMapMode(hdc, MM_ISOTROPIC);
	//设置坐标系逻辑单位
	::SetWindowExtEx(hdc, 1000, 1000, NULL);
	//设置x、y轴方向和坐标系包含范围
	::SetViewportExtEx(hdc, cx, -cy, NULL);
	//设置原点坐标
	::SetViewportOrgEx(hdc, cx/2, cy/2, NULL);
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

//画一个椭圆
void OnPaint2(HWND hwnd){
	PAINTSTRUCT ps;
	HDC hdc = ::BeginPaint(hwnd, &ps); 
	
	//获取客户区的大小
	RECT rt;
	::GetClientRect(hwnd, &rt);
	int cx = rt.right;
	int cy = rt.bottom;

	//设置客户区的逻辑大小为 500 乘 500，原点为 (0, 0)
	//设置坐标映射方式,用户自定义为
	::SetMapMode(hdc, MM_ANISOTROPIC);
	//设置逻辑单位
	::SetWindowExtEx(hdc, 500, 500, NULL);
	//设置坐标方向，和坐标系的范围
	::SetViewportExtEx(hdc, cx, cy, NULL);
	//设置原点坐标
	::SetViewportOrgEx(hdc, 0, 0, NULL);

	//以整个客户区为边界画一个椭圆
	::Ellipse(hdc, 0, 0, 500, 500);
	::EndPaint(hwnd,&ps);

}

//使用GDI函数画时针、分针、秒针
void DrawHand(HDC hdc, int nLength, int nWidth, int nDegrees, COLORREF clrColor){
	//将角度nDegrees转化为弧度，2 * 3.1415926 / 360 = 0.0174533
	double nReadians = (double)nDegrees * 0.0174533;

	//计算坐标
	POINT pt[2];
	pt[0].x = (int)(nLength * sin(nReadians));
	pt[0].y = (int)(nLength * cos(nReadians));
	pt[1].x = - pt[0].x/5;
	pt[1].y = - pt[0].y/5;

	//创建画笔，并选入DC结构体
	HPEN hPen = ::CreatePen(PS_SOLID, nWidth, clrColor);
	HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);

	//划线
	::MoveToEx(hdc, pt[0].x, pt[0].y, NULL);
	::LineTo(hdc, pt[1].x, pt[1].y);

	::SelectObject(hdc, hOldPen);
	::DeleteObject(hPen);
}

//上一次windows通知时间
static int s_nPreHour;		//小时
static int s_nPreMinute;	//分钟
static int s_nPreSecond;	//秒

//窗口客户区的大小
static int s_cxClient;
static int s_cyClient;

//是否位于最顶层
static BOOL s_bTopMost;

//时钟定时器编号
#define IDT_CLOCK 1

LRESULT CALLBACK WndProc1(HWND hwnd, UINT message, WPARAM wParam, LPARAM IParam){
	switch(message){
	case WM_CREATE:
		{
			SYSTEMTIME time;
			//设置时间
			::GetLocalTime(&time);
			s_nPreHour = time.wHour%12;
			s_nPreMinute = time.wMinute;
			s_nPreSecond = time.wSecond;
			//创建定时器
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

			//设置坐标系
			SetIsotropic(hdc, s_cxClient, s_cyClient);

			//绘制时钟外观
			DrawClockFace(hdc);

			//绘制指针
			//经过1小时，时针走30度(360/12)，经过1分钟时针走0.5度(30/60)
			DrawHand(hdc, 200, 8, s_nPreHour * 30 + s_nPreMinute/2, RGB(0, 0, 0));
			//经过1分钟，分针走6度(360/6)
			DrawHand(hdc, 400, 6, s_nPreMinute * 6, RGB(0, 0, 0));
			//经过1秒钟，秒针走6度(360/6)
			DrawHand(hdc, 400, 6, s_nPreSecond * 6, RGB(0, 0, 0));
			::EndPaint(hwnd, &ps);
			return 0;
		}
	case WM_TIMER:
		{
			//如果窗口处于最小化状态，就什么也不做
			if(::IsIconic(hwnd)){
				return 0;
			}
			//获取系统时间
			SYSTEMTIME time;
			::GetLocalTime(&time);

			//建立坐标系
			HDC hdc = ::GetDC(hwnd);
			//设置坐标系
			SetIsotropic(hdc, s_cxClient, s_cyClient);

			//以COLOR_3DFACE 为背景就可以擦除指针了
			//因为窗口背景颜色是 COLOR_3DFACE
			COLORREF crfColor = ::GetSysColor(COLOR_3DFACE);

			//如果分钟改变的话就擦除时针和分针
			if(time.wMinute != s_nPreMinute){
				//擦除时针和分针
				
				DrawHand(hdc, 200, 8, s_nPreHour * 30 + s_nPreMinute / 2, crfColor);
				DrawHand(hdc, 400, 6, s_nPreMinute * 6, crfColor);
				s_nPreHour = time.wHour;
				s_nPreMinute = time.wMinute;
				
			}
			
			//如果秒针改变的话就擦除秒针，然后重画所有针
			if(time.wSecond != s_nPreSecond){
				//擦除秒针
				DrawHand(hdc, 400, 1, s_nPreSecond * 6, crfColor);

				//重画所有针
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
				//OnPaint(hwnd);//画方形
				//OnPaint2(hwnd);//画椭圆


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