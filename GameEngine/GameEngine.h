
#include "EngineHead.h"
#include "resource.h"
#include "Control.h"


/* Constants *****************************************************************************************************/
HWND	Window;
DEVMODE	old_resolution;
DEVMODE	new_resolution;

Control	*g_pControl = NULL;															// �ܿ�����

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
	glShadeModel(GL_SMOOTH);								// ������Ӱƽ��
	
	glPolygonMode(GL_FRONT,GL_LINE);  
    glPolygonMode(GL_BACK,GL_FILL); 
	glFrontFace(GL_CW);												// ����˳ʱ�뷽��Ϊ���� 

	/*�����������б��������ǹ���depth buffer(��Ȼ���)�ġ�
	����Ȼ�������Ϊ��Ļ����Ĳ㡣��Ȼ��治�ϵĶ����������Ļ�ڲ��ж�����и��١�
	���Ǳ��ڵĳ�����ʵû������ʹ����Ȼ��棬��������������Ļ����ʾ3D����OpenGL����ʹ����Ȼ��档
	������������Ǹ������Ȼ����������Ͳ��Ὣһ��Բ�κ���������λ���Բ��������
	��Ȼ�����OpenGLʮ����Ҫ�Ĳ��֡�*/
	glClearDepth(1.0f);													// ������Ȼ���
	glDepthFunc(GL_LEQUAL);									// ������Ȳ��Ե�����
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// ����ϵͳ��͸�ӽ�������

	glEnable(GL_STEREO);										//��������+3DҺ������

	glMatrixMode(GL_PROJECTION);						// ѡ��ͶӰ����
	glLoadIdentity();														// ����ͶӰ����

	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,1000.0f); // �����ӿڵĴ�С
};	

/* SetControlDefaults ******************************************************************************************************/
//�����ܿ����豸,����ʼ�������������ڵ�һ��
void fnSetControlDefault()
{
 	fnControlSetUp();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);						// ��ɫ����
	g_pControl->sectorStyle.isTexture = true;
	g_pControl->sectorStyle.isBlend    = true;
	if(g_pControl) g_pControl->UpdateRenderEnvironment();
	TestInitialSector(g_pControl,10,10,1.0f);
	TestUpdateobServer(g_pControl);
};
