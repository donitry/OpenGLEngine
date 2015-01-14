// GameEngine.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "GameEngine.h"
#include "raster.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE hInst;															// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];							// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
RASTER	raster;																// �豸����

GdiplusStartupInput m_gdiplusStartupInput;   //gdi+
ULONG_PTR m_pGdiToken;						 //gdi+	
// �˴���ģ���а����ĺ�����ǰ������:
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
	//glDrawBuffer(GL_BACK_RIGHT);// ���һ���ģʽ
	//glDrawBuffer(GL_BACK_LEFT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// �����Ļ����Ȼ���
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

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GAMEENGINE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
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
		// �����˵�ѡ��:
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
		// TODO: �ڴ���������ͼ����...
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

// �����ڡ������Ϣ�������
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
	//glLoadIdentity();																					// ����ģ�͹۲����
	//glTranslatef(0.0f,0.0f, -10.0f);
	//glRotatef(xrot,1.0f,0.0f,0.0f);																//  ��X����ת�ı���	
	//glRotatef(yrot,0.0f,1.0f,0.0f);																//  ��Y����ת�ı���	
	//glRotatef(rquad,0.0f,0.0f,1.0f);																//  ��Z����ת
	glBindTexture(GL_TEXTURE_2D, TextureName[2]);
	glBegin(GL_QUADS);																			//  ����������
		// ǰ����
		glNormal3f( 0.0f, 0.0f, 1.0f);					// ����ָ��۲���
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	
		// �����
		glNormal3f( 0.0f, 0.0f,-1.0f);					// ���߱���۲���
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	
		// ����
		glNormal3f( 0.0f, 1.0f, 0.0f);					// ��������
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	
		// ����
		glNormal3f( 0.0f,-1.0f, 0.0f);					// ���߳���
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	
		// �Ҳ���
		glNormal3f( 1.0f, 0.0f, 0.0f);					// ���߳���
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	
		// �����
		glNormal3f(-1.0f, 0.0f, 0.0f);					// ���߳���
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);

	glEnd();																									// �����λ��ƽ���
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
		glNormal3f( 0.0f, 1.0f, 0.0f);					// ��������
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	
	//glEnd();

	//glLoadIdentity();														// ����ͶӰ����
	//gluLookAt(1.0,1.0,a,1.0,1.0,0.0,b,1.0,0.0);
	//a += 0.0001;
	//camera->setCameraPostion(1.0,1.0,b);
	b += 0.001;

	
		glVertex3f( 1.0f, 1.0f, 0.0f);	
		glVertex3f( 1.0f, 0.0f, 0.0f);	
		glVertex3f( 0.0f, 0.0f, 0.0f);	

		glVertex3f( 0.0f, 0.0f, 0.0f);			// �����ε��϶��� (ǰ����)
		glVertex3f( 0.0f, 1.0f, 0.0f);			// �����ε����¶��� (ǰ����)
		glVertex3f( 1.0f, 1.0f, 0.0f);			// �����ε����¶��� (ǰ����)

		glVertex3f( 0.0f, 0.0f, 0.0f);	
		glVertex3f(-1.0f, 0.0f, 0.0f);	
		glVertex3f( 0.0f, 1.0f, 0.0f);	

		glVertex3f(-1.0f, 0.0f, 0.0f);	
		glVertex3f(-1.0f, 1.0f, 0.0f);	
		glVertex3f( 0.0f, 1.0f, 0.0f);	
	*/
		
};

