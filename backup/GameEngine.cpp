// GameEngine.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "GameEngine.h"
#include "raster.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;															// 当前实例
TCHAR szTitle[MAX_LOADSTRING];							// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
RASTER	raster;																// 设备控制

GdiplusStartupInput m_gdiplusStartupInput;   //gdi+
ULONG_PTR m_pGdiToken;						 //gdi+	
// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

/* TEST FUNCTION ********************************************************************************************/
void drawQuads(void);
void drawTringles(void);

GLdouble xrot = 0.0;
/* Render ********************************************************************************************************/
void Render()
{
	//glDrawBuffer(GL_BACK_RIGHT);// 左右缓冲模式
	//glDrawBuffer(GL_BACK_LEFT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// 清除屏幕及深度缓存
	glColor3f(1.0f,1.0f,1.0f);
	glPushMatrix();
		control->InitialobServer();
		char a[24];
		itoa(control->obServer.obAUp,a,10);
		Test(1,2,a);
	glPopMatrix();
	if(control){
			//glRotatef(xrot,1.0f,0.0f,0.0f);
			//control->InitialobServer();
			control->MoveobServer(0.01,0.0);
			control->DrawSector();
	}

	xrot += 0.1;
	drawQuads();
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

	while(true){
		Render();
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage (&msg);
		}
		SwapBuffers (raster.hDC);
	}
	ClearUp();
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

	SetGLDefaults();

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
		control = new Control();
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
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:{
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
				  }break;
	case WM_ERASEBKGND:{
		return 0;
					   }break;
	case WM_RBUTTONDOWN:{
						}break;
	case WM_RBUTTONUP:{
						}break;
	case WM_MOUSEMOVE:{
		if(wParam == MK_RBUTTON){
			break;
		}
						}break;
	case WM_KEYDOWN:{
		switch(wParam){
		case 'W':{
			control->MoveobServer(1.0,0.0);
				}break;
		case 'S':{
			control->MoveobServer(-1.0,0.0);
				}break;
		case 'A':{
			control->MoveobServer(0.0,1.0);
				}break;
		case 'D':{
			control->MoveobServer(0.0,-1.0);
				}break;
		case 'Q':{
			control->TwistobServer(0.01,0.0,0.0);
				}break;
		case 'E':{
			control->TwistobServer(-0.01,0.0,0.0);
				}break;
		case 'R':{
			control->TwistobServer(0.0,-0.01,0.1);
				}break;
		case 'T':{
			control->TwistobServer(0.0,0.01,-0.1);
			control->PullobServer(-0.1);
			control->MoveobServer(1.0,0.0);
				}break;
		case 'Z':{
			control->PullobServer(0.1);
				}break;
		case 'X':{
			control->PullobServer(-0.1);
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

void drawQuads(void)
{
	//glLoadIdentity();																					// 重置模型观察矩阵
	//glTranslatef(0.0f,0.0f, -10.0f);
	//glRotatef(xrot,1.0f,0.0f,0.0f);																//  绕X轴旋转四边形	
	//glRotatef(yrot,0.0f,1.0f,0.0f);																//  绕Y轴旋转四边形	
	//glRotatef(rquad,0.0f,0.0f,1.0f);																//  绕Z轴旋转
	glBindTexture(GL_TEXTURE_2D, TextureName[2]);
	glBegin(GL_QUADS);																			//  绘制正方形
		// 前侧面
		glNormal3f( 0.0f, 0.0f, 1.0f);					// 法线指向观察者
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	
		// 后侧面
		glNormal3f( 0.0f, 0.0f,-1.0f);					// 法线背向观察者
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	
		// 顶面
		glNormal3f( 0.0f, 1.0f, 0.0f);					// 法线向上
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	
		// 底面
		glNormal3f( 0.0f,-1.0f, 0.0f);					// 法线朝下
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	
		// 右侧面
		glNormal3f( 1.0f, 0.0f, 0.0f);					// 法线朝右
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	
		// 左侧面
		glNormal3f(-1.0f, 0.0f, 0.0f);					// 法线朝左
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);

	glEnd();																									// 正方形绘制结束
};

GLdouble a = 10.0;
GLdouble b = 0.001;

void drawTringles(void)
{
	/*
	glColor3f(1.0f,1.0f,1.0f);	
	glBindTexture(GL_TEXTURE_2D, TextureName[3]);
	

	if(sector->tagSector.numtriangles>0)
	{
		for(uLong i=0;i<sector->tagSector.numtriangles;i++)
		{
			//glBegin(GL_TRIANGLES);
			glBegin(GL_LINE_LOOP);
			for(int j = 0;j<3;j++)
			{
				glTexCoord2f(sector->tagSector.triangle[i].vertex[j].u,sector->tagSector.triangle[i].vertex[j].v);
				glVertex3f(sector->tagSector.triangle[i].vertex[j].x,sector->tagSector.triangle[i].vertex[j].y,sector->tagSector.triangle[i].vertex[j].z);
			}
			glEnd();
		}
	}
	//glBegin(GL_LINE_LOOP);
		glNormal3f( 0.0f, 1.0f, 0.0f);					// 法线向上
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	
	//glEnd();

	//glLoadIdentity();														// 重置投影矩阵
	//gluLookAt(1.0,1.0,a,1.0,1.0,0.0,b,1.0,0.0);
	//a += 0.0001;
	//camera->setCameraPostion(1.0,1.0,b);
	b += 0.001;

	
		glVertex3f( 1.0f, 1.0f, 0.0f);	
		glVertex3f( 1.0f, 0.0f, 0.0f);	
		glVertex3f( 0.0f, 0.0f, 0.0f);	

		glVertex3f( 0.0f, 0.0f, 0.0f);			// 三角形的上顶点 (前侧面)
		glVertex3f( 0.0f, 1.0f, 0.0f);			// 三角形的左下顶点 (前侧面)
		glVertex3f( 1.0f, 1.0f, 0.0f);			// 三角形的右下顶点 (前侧面)

		glVertex3f( 0.0f, 0.0f, 0.0f);	
		glVertex3f(-1.0f, 0.0f, 0.0f);	
		glVertex3f( 0.0f, 1.0f, 0.0f);	

		glVertex3f(-1.0f, 0.0f, 0.0f);	
		glVertex3f(-1.0f, 1.0f, 0.0f);	
		glVertex3f( 0.0f, 1.0f, 0.0f);	
	*/
		
};

