
#include "EngineHead.h"
#include "texture.h"
#include "Sector.h"
#include "Camera.h"
#include "Light.h"
#include "gFont.h"
#include "gTimer.h"

/* STRCUT *****************************************************************************/
typedef struct
{
	bool isBlend;						//是否支持混合模式
	bool isTexture;					//材质模式还是线条显示
}SECTORSTYLE;

typedef struct
{
	GLdouble obRad;				//观察半径
	GLdouble obAUp;				//观察仰角单位是度(0-360)
	GLdouble obAOffset;		//观察偏角
	GLdouble obDx;				//观察位置的X坐标
	GLdouble obDy;				//观察位置的Y坐标
	GLdouble obDz;				//观察位置的Z坐标
}OBSERVER;

static char t[256]={"sss"};

/* CLASS *******************************************************************************/
class Control
{
	public:
		int version;
		OBSERVER	obServer;
		SECTORSTYLE sectorStyle;
		Sector *sector;
		Camera *camera;
		gFont	*gfont;
		gTimer *gtimer;
	public://渲染环境控制
		void UpdateRenderEnvironment();
	public://摄像机控制
		void InitialobServer();//初始化摄像机位置，这个函数是以当前类的初始值来初始化摄像机
		void MoveobServer(GLdouble up,GLdouble offset,bool isAuto = true);
		void TwistobServer(GLdouble destX,GLdouble destY,GLdouble destZ,bool isAuto = true);
		void PullobServer(GLdouble distance,bool isAuto = true);
		void DrawSector();
	public://文本控制
		void fontOutput(float x,float y,char* str);
	public:
		Control();
		~Control();
};

/* MEMBER FUNCTION ***************************************************************/
Control::Control()
{
	//初始化基本的参数
	this->version  = ENGINEVERSION;
	this->sector	= new Sector();
	this->camera	= new Camera();
	this->gfont		= new gFont();
	this->gtimer	= new gTimer();
	this->sectorStyle.isBlend		=  false;
	this->sectorStyle.isTexture	=  false;
	this->obServer.obRad			=  10.0;
	this->obServer.obAUp			=	90.0;
	this->obServer.obAOffset	=	45.0;
	this->obServer.obDx				=	0.0;
	this->obServer.obDy				=	0.0;
	this->obServer.obDz				=	0.0;

	//初始化控制的对象
	this->InitialobServer();//摄像机初始化
};

Control::~Control()
{
	if(sector){
		delete sector;
		sector = NULL;
	}
	if(camera){
		delete camera;
		camera = NULL;
	}
	if(gfont){
		delete gfont;
		gfont = NULL;
	}
	if(gtimer){
		delete gtimer;
		gtimer = NULL;
	}
};
//渲染控制部分**********************************************************************************************************>>>>>>>>>>>>
void Control::UpdateRenderEnvironment()
{
	if(this->sectorStyle.isBlend){
		glDisable(GL_DEPTH_TEST);
		glColor4f(1.0f,1.0f,1.0f,1.0f);								// 全亮度， 50% Alpha 混合
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);		// 基于源象素alpha通道值的半透明混合函数
		glEnable(GL_BLEND);											//混合启动
	}else{
		glEnable(GL_DEPTH_TEST);								// 启用深度测试(启用的情况下，混合失效——即不透明状态)
	};
	if(this->sectorStyle.isTexture){glEnable(GL_TEXTURE_2D);								// 启用纹理映射
	}else{glDisable(GL_TEXTURE_2D);}
};

//摄像机操作部分*******************************************************************************************************>>>>>>>>>>>>
void Control::InitialobServer()
{
	this->camera->InitialCameraPostion();
	this->camera->setCameraDirection(this->obServer.obDx, this->obServer.obDy, this->obServer.obDz);
	this->camera->setCameraPostion(0.0,0.0,this->obServer.obRad);
	this->camera->RollCameraPostion(this->obServer.obRad,this->obServer.obAUp,this->obServer.obAOffset);
};

//移动摄像机位置，圆球内进行
void Control::MoveobServer(GLdouble up,GLdouble offset,bool isAuto)
{
	obServer.obAUp		=	isAuto?obServer.obAUp+up:up;
	obServer.obAOffset	=	isAuto?obServer.obAOffset+offset:offset;
	if(abs((GLint)obServer.obAUp)%180 == 0){
		this->camera->upVertex.y *= -1; obServer.obAUp+=up;
		this->camera->setCameraUpside(0.0,this->camera->upVertex.y,0.0);
	}//经过180度后，摄像机进入镜面内，一切都反了，所以要把摄像机反向一下
	if(abs(obServer.obAOffset)>360.0)		obServer.obAOffset = offset;
	this->camera->RollCameraPostion(obServer.obRad,obServer.obAUp,obServer.obAOffset);	
	sprintf(t,"eye--->x:%4.2f y:%4.2f z:%4.2f look--->x:%4.2f y:%4.2f z:%4.2f  deg:%f",
		this->camera->eyePostion.x,this->camera->eyePostion.y,this->camera->eyePostion.z,
		this->camera->lookPostion.x,this->camera->lookPostion.y,this->camera->lookPostion.z,
		this->obServer.obAUp);

};

//扭转摄像机镜头
void Control::TwistobServer(GLdouble destX,GLdouble destY,GLdouble destZ,bool isAuto)
{
	obServer.obDx = isAuto?obServer.obDx+destX:destX;
	obServer.obDy = isAuto?obServer.obDy+destY:destY;
	obServer.obDz = isAuto?obServer.obDz+destZ:destZ;
	this->camera->setCameraDirection(obServer.obDx ,obServer.obDy ,obServer.obDz );
};
//调整摄像机距离物体的距离
void Control::PullobServer(GLdouble distance,bool isAuto)
{
	obServer.obRad = isAuto?obServer.obRad+distance:distance;
	this->camera->RollCameraPostion(obServer.obRad,obServer.obAUp,obServer.obAOffset);	
};

void Control::DrawSector()
{
	this->sector->RenderSector(sectorStyle.isTexture);
};


//文字操作部分*******************************************************************************************************>>>>>>>>>>>>
void Control::fontOutput(float x, float y, char *str)
{
 	glPushMatrix();
 		glMatrixMode(GL_MODELVIEW);						// 选择模型观察矩阵	
		glLoadIdentity();														// 重置投影矩阵
 		gluLookAt(	0.0,0.0,1.0,
 							0.0,0.0,0.0,
 							0.0,1.0,0.0); //暂时只提供正立图像		
		gfont->Printftext(x ,y ,str);
	glPopMatrix();
};


//测试区域，以下函数都是测试用例>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void Test(Control *control,GLdouble x1,GLdouble y1)
{
	GLdouble	winX=0.0,winY=0.0,winZ=0.0;
	GLdouble   modelMatrix[16];
	GLdouble   projMatrix[16];
	GLint			viewport[4];
	GLint			TestArg;
	glGetIntegerv(GL_AUX_BUFFERS  ,&TestArg);
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	gluUnProject(x1, y1, 0, modelMatrix, projMatrix, viewport, &winX, &winZ, &winY);
	
	char a[24],b[24],c[24],d[24]={0};
	itoa(winX,	a,	10);
	itoa(winY,	b,	10);
	itoa(winZ,	c,	10);
	itoa(TestArg,	d,	10);
	control->fontOutput(0.1f,0.10f,a);
	control->fontOutput(0.2f,0.15f,b);
	control->fontOutput(0.3f,0.20f,c);
	control->fontOutput(-1.0f,0.25f,d);
};

float c=0;
void drawQuads(void)
{
	//glLoadIdentity();																					// 重置模型观察矩阵
	//glTranslatef(0.0f,0.0f, -10.0f);
	//glRotatef(xrot,1.0f,0.0f,0.0f);																//  绕X轴旋转四边形	
	//glRotatef(yrot,0.0f,1.0f,0.0f);																//  绕Y轴旋转四边形	
	//glRotatef(rquad,0.0f,0.0f,1.0f);																//  绕Z轴旋转
	//glBindTexture(GL_TEXTURE_2D, TextureName[2]);
	glColor3f(0.5f, 0.7f, 1.0f);
	glPushMatrix();
	//glLoadIdentity();
	//glTranslatef(0.0f,0.0f, c);
	if(c>360) c=0;
	c += 1;
	//glRotatef(c,1,0,0);
	//glRotatef(c,0,1,0);
	//glRotatef(c,0,0,1);
	glBegin(GL_LINES);
	for(int i=-1000;i<=1000;i+=10)
	{
		glVertex3f(0,0,i);
		glVertex3f(10,10,i);

		glVertex3f(0,0,i);
		glVertex3f(-10,10,i);
	}
	glEnd();
	glPopMatrix();
	/*
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
	*/
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

void TestInitialSector(Control *control,GLint width,GLint height,float scale)
{
	InitialTexture();
	glBindTexture(GL_TEXTURE_2D, TextureName[3]);
	control->sector->setSector(width,height,scale);
};

void TestUpdateobServer(Control *control)
{
	control->obServer.obDx		= 0.0;//(GLdouble)10/2;
	control->obServer.obDy		= 0.0;
	control->obServer.obDz		= 0.0;//(GLdouble)10/2;
	control->InitialobServer();
};

void TestRender(Control *control)
{
// 	InitialTexture();
// 	glBindTexture(GL_TEXTURE_2D, TextureName[3]);
// 	control->camera->InitialCameraPostion();
// 	control->sector->setSector(width,height,scale);
// 	control->obServer.obRad 	= 200.0;
// 	control->obServer.obDx		= 0.0;//(GLdouble)width/2;
// 	control->obServer.obDy		= 0.0;
// 	control->obServer.obDz		= 0.0;//(GLdouble)height/2;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// 清除屏幕及深度缓存
	GLint a = control->gtimer->GetEngineFPS(glutGet(GLUT_ELAPSED_TIME));
	char buff[512]={0};sprintf_s(buff,sizeof(buff)-1,"fps:%d",a);
	control->fontOutput(0,0,t);
	//drawQuads();
	control->DrawSector();
	
};