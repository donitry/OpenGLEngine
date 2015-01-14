// GameEngine.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "GameEngine.h"
#include "raster.h"

#define MAX_LOADSTRING 100
#define GAME_FPS				75

// 全局变量:
HINSTANCE hInst;															// 当前实例
TCHAR szTitle[MAX_LOADSTRING];							// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
RASTER	raster;																// 设备控制
HMENU hMainMenu;														// 窗口主菜单

GdiplusStartupInput m_gdiplusStartupInput;   //gdi+
ULONG_PTR m_pGdiToken;						     //gdi+	

GLint  priorTime = 0;															// 用于fps计算

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

/* TEST FUNCTION ********************************************************************************************/
void drawQuads(void);
void drawTringles(void);

GLdouble xrot = 0.0;
POINT currentPoint;
int frame=0,cur_time=0,timebase=0,old_frame=0;
char s[256]={"FPS"};
/* Render ********************************************************************************************************/
//渲染应分为很多部分，架设很多的摄像机
void Render(GLint fps)
{

	//glDrawBuffer(GL_BACK_RIGHT);// 左右缓冲模式
	//glDrawBuffer(GL_BACK_LEFT);
	//glFinish();
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// 清除屏幕及深度缓存

	if(g_pControl){
		//g_pControl->fontOutput(-1.0f,0.35f,t);
		
		if(g_pControl->gtimer->LimitEngineFPS(glutGet(GLUT_ELAPSED_TIME))){
			TestRender(g_pControl);
			SwapBuffers (raster.hDC);
		}
		//Test(g_pControl,(GLdouble)currentPoint.x,(GLdouble)currentPoint.y);
		//control->fontOutput(currentPoint.x, currentPoint.y, "Test");
		//g_pControl->DrawSector();
		//auxWireTeapot(1);
	}
	
}


/* MAIN **********************************************************************************************/


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GAMEENGINE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAMEENGINE));
	fnSetGLDefaults();
	fnSetControlDefault();
	while(true){
		Render(GAME_FPS);
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage (&msg);
		}
	}
	fnControlClearUp();
	raster.Release(Window);
	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAMEENGINE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_GAMEENGINE);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

	hInst = hInstance; // 将实例句柄存储在全局变量中

	Window = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	  CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!Window)
	{
	  return FALSE;
	}

	if (!raster.Init (Window, (unsigned char)new_resolution.dmBitsPerPel))
	{
		MessageBoxA (NULL, "Error: Failed to Initialize OpenGL", "ERROR!", MB_OK);
		return (0);
	}
	
	hMainMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDC_GAMEENGINE));   //装备主菜单

	ShowWindow(Window, nCmdShow);
	UpdateWindow(Window);

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:{
		GdiplusStartup( &m_pGdiToken , &m_gdiplusStartupInput , NULL );//gdi+
		currentPoint.x = 0;
		currentPoint.y = 0;
				   }break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_TEXTURE:
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:{
		hdc = BeginPaint(hWnd, &ps);
		{
			Graphics		gr(hdc);
			SolidBrush		brush(RGB(255,0,255));
			FontFamily		fontFamily(L"Arial");
			Gdiplus::Font   font(&fontFamily, (REAL)12, FontStyleRegular, UnitPixel);
			PointF			pointF(100, 100);

			char *str="strstrstrstrstrstr";
			static WCHAR buffer[256];
			wcsset(buffer,0);
			MultiByteToWideChar(CP_ACP,0,str,(int)strlen(str),buffer,256);  
			gr.DrawString(buffer, -1, &font, pointF, &brush);
			gr.ReleaseHDC(hdc);
		}
		EndPaint(hWnd, &ps);
				  }break;
	case WM_ERASEBKGND:{
		return 1;
					   }break;
	case WM_RBUTTONUP:{
						}break;
	case WM_MOUSEMOVE:{
		if(wParam == MK_RBUTTON){
			break;
		}
		GetCursorPos(&currentPoint);
		ScreenToClient(Window,&currentPoint);
						}break;
	case WM_KEYDOWN:{
		switch(wParam){
		case 'W':{
			g_pControl->MoveobServer(1.0,0.0);
				}break;
		case 'S':{
			g_pControl->MoveobServer(-1.0,0.0);
				}break;
		case 'A':{
			g_pControl->MoveobServer(0.0,1.0);
				}break;
		case 'D':{
			g_pControl->MoveobServer(0.0,-1.0);
				}break;
		case 'Q':{
			g_pControl->TwistobServer(0.01,0.0,0.0);
				}break;
		case 'E':{
			g_pControl->TwistobServer(-0.01,0.0,0.0);
				}break;
		case 'R':{
			g_pControl->TwistobServer(0.0,-0.01,0.1);
				}break;
		case 'T':{
			g_pControl->TwistobServer(0.0,0.01,-0.1);
			g_pControl->PullobServer(-0.1);
			g_pControl->MoveobServer(1.0,0.0);
				}break;
		case 'Z':{
			g_pControl->PullobServer(0.1);
				}break;
		case 'X':{
			g_pControl->PullobServer(-0.1);
				}break;
		default:break;
		}
						}break;
	case WM_DESTROY:
		GdiplusShutdown(m_pGdiToken);//gdi+
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}



