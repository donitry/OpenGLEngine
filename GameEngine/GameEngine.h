
#include "EngineHead.h"
#include "resource.h"
#include "Control.h"


/* Constants *****************************************************************************************************/
HWND	Window;
DEVMODE	old_resolution;
DEVMODE	new_resolution;

Control	*g_pControl = NULL;															// 总控制器

void fnControlSetUp(){
	g_pControl = new Control();
}
void fnControlClearUp(){
	if(g_pControl){delete g_pControl;g_pControl = NULL;}
};

/* SetGLDefaults *****************************************************************************************************/
void fnSetGLDefaults()
{
	RECT rect;
	GetClientRect(Window,&rect);
	int width = (rect.right - rect.left);
	int height = (rect.bottom - rect.top);
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glShadeModel(GL_SMOOTH);								// 启用阴影平滑
	
	glPolygonMode(GL_FRONT,GL_LINE);  
    glPolygonMode(GL_BACK,GL_FILL); 
	glFrontFace(GL_CW);												// 设置顺时针方向为正面 

	/*接下来的三行必须做的是关于depth buffer(深度缓存)的。
	将深度缓存设想为屏幕后面的层。深度缓存不断的对物体进入屏幕内部有多深进行跟踪。
	我们本节的程序其实没有真正使用深度缓存，但几乎所有在屏幕上显示3D场景OpenGL程序都使用深度缓存。
	它的排序决定那个物体先画。这样您就不会将一个圆形后面的正方形画到圆形上来。
	深度缓存是OpenGL十分重要的部分。*/
	glClearDepth(1.0f);													// 设置深度缓存
	glDepthFunc(GL_LEQUAL);									// 所作深度测试的类型
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// 告诉系统对透视进行修正

	glEnable(GL_STEREO);										//立体驱动+3D液晶快门

	glMatrixMode(GL_PROJECTION);						// 选择投影矩阵
	glLoadIdentity();														// 重置投影矩阵

	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,1000.0f); // 设置视口的大小
};	

/* SetControlDefaults ******************************************************************************************************/
//启动总控制设备,并初始化包括材质在内的一切
void fnSetControlDefault()
{
 	fnControlSetUp();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);						// 黑色背景
	g_pControl->sectorStyle.isTexture = true;
	g_pControl->sectorStyle.isBlend    = true;
	if(g_pControl) g_pControl->UpdateRenderEnvironment();
	TestInitialSector(g_pControl,10,10,1.0f);
	TestUpdateobServer(g_pControl);
};
