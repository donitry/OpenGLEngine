
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
	bool isBlend;						//�Ƿ�֧�ֻ��ģʽ
	bool isTexture;					//����ģʽ����������ʾ
}SECTORSTYLE;

typedef struct
{
	GLdouble obRad;				//�۲�뾶
	GLdouble obAUp;				//�۲����ǵ�λ�Ƕ�(0-360)
	GLdouble obAOffset;		//�۲�ƫ��
	GLdouble obDx;				//�۲�λ�õ�X����
	GLdouble obDy;				//�۲�λ�õ�Y����
	GLdouble obDz;				//�۲�λ�õ�Z����
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
	public://��Ⱦ��������
		void UpdateRenderEnvironment();
	public://���������
		void InitialobServer();//��ʼ�������λ�ã�����������Ե�ǰ��ĳ�ʼֵ����ʼ�������
		void MoveobServer(GLdouble up,GLdouble offset,bool isAuto = true);
		void TwistobServer(GLdouble destX,GLdouble destY,GLdouble destZ,bool isAuto = true);
		void PullobServer(GLdouble distance,bool isAuto = true);
		void DrawSector();
	public://�ı�����
		void fontOutput(float x,float y,char* str);
	public:
		Control();
		~Control();
};

/* MEMBER FUNCTION ***************************************************************/
Control::Control()
{
	//��ʼ�������Ĳ���
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

	//��ʼ�����ƵĶ���
	this->InitialobServer();//�������ʼ��
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
//��Ⱦ���Ʋ���**********************************************************************************************************>>>>>>>>>>>>
void Control::UpdateRenderEnvironment()
{
	if(this->sectorStyle.isBlend){
		glDisable(GL_DEPTH_TEST);
		glColor4f(1.0f,1.0f,1.0f,1.0f);								// ȫ���ȣ� 50% Alpha ���
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);		// ����Դ����alphaͨ��ֵ�İ�͸����Ϻ���
		glEnable(GL_BLEND);											//�������
	}else{
		glEnable(GL_DEPTH_TEST);								// ������Ȳ���(���õ�����£����ʧЧ��������͸��״̬)
	};
	if(this->sectorStyle.isTexture){glEnable(GL_TEXTURE_2D);								// ��������ӳ��
	}else{glDisable(GL_TEXTURE_2D);}
};

//�������������*******************************************************************************************************>>>>>>>>>>>>
void Control::InitialobServer()
{
	this->camera->InitialCameraPostion();
	this->camera->setCameraDirection(this->obServer.obDx, this->obServer.obDy, this->obServer.obDz);
	this->camera->setCameraPostion(0.0,0.0,this->obServer.obRad);
	this->camera->RollCameraPostion(this->obServer.obRad,this->obServer.obAUp,this->obServer.obAOffset);
};

//�ƶ������λ�ã�Բ���ڽ���
void Control::MoveobServer(GLdouble up,GLdouble offset,bool isAuto)
{
	obServer.obAUp		=	isAuto?obServer.obAUp+up:up;
	obServer.obAOffset	=	isAuto?obServer.obAOffset+offset:offset;
	if(abs((GLint)obServer.obAUp)%180 == 0){
		this->camera->upVertex.y *= -1; obServer.obAUp+=up;
		this->camera->setCameraUpside(0.0,this->camera->upVertex.y,0.0);
	}//����180�Ⱥ���������뾵���ڣ�һ�ж����ˣ�����Ҫ�����������һ��
	if(abs(obServer.obAOffset)>360.0)		obServer.obAOffset = offset;
	this->camera->RollCameraPostion(obServer.obRad,obServer.obAUp,obServer.obAOffset);	
	sprintf(t,"eye--->x:%4.2f y:%4.2f z:%4.2f look--->x:%4.2f y:%4.2f z:%4.2f  deg:%f",
		this->camera->eyePostion.x,this->camera->eyePostion.y,this->camera->eyePostion.z,
		this->camera->lookPostion.x,this->camera->lookPostion.y,this->camera->lookPostion.z,
		this->obServer.obAUp);

};

//Ťת�������ͷ
void Control::TwistobServer(GLdouble destX,GLdouble destY,GLdouble destZ,bool isAuto)
{
	obServer.obDx = isAuto?obServer.obDx+destX:destX;
	obServer.obDy = isAuto?obServer.obDy+destY:destY;
	obServer.obDz = isAuto?obServer.obDz+destZ:destZ;
	this->camera->setCameraDirection(obServer.obDx ,obServer.obDy ,obServer.obDz );
};
//�����������������ľ���
void Control::PullobServer(GLdouble distance,bool isAuto)
{
	obServer.obRad = isAuto?obServer.obRad+distance:distance;
	this->camera->RollCameraPostion(obServer.obRad,obServer.obAUp,obServer.obAOffset);	
};

void Control::DrawSector()
{
	this->sector->RenderSector(sectorStyle.isTexture);
};


//���ֲ�������*******************************************************************************************************>>>>>>>>>>>>
void Control::fontOutput(float x, float y, char *str)
{
 	glPushMatrix();
 		glMatrixMode(GL_MODELVIEW);						// ѡ��ģ�͹۲����	
		glLoadIdentity();														// ����ͶӰ����
 		gluLookAt(	0.0,0.0,1.0,
 							0.0,0.0,0.0,
 							0.0,1.0,0.0); //��ʱֻ�ṩ����ͼ��		
		gfont->Printftext(x ,y ,str);
	glPopMatrix();
};


//�����������º������ǲ�������>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
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
	//glLoadIdentity();																					// ����ģ�͹۲����
	//glTranslatef(0.0f,0.0f, -10.0f);
	//glRotatef(xrot,1.0f,0.0f,0.0f);																//  ��X����ת�ı���	
	//glRotatef(yrot,0.0f,1.0f,0.0f);																//  ��Y����ת�ı���	
	//glRotatef(rquad,0.0f,0.0f,1.0f);																//  ��Z����ת
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

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// �����Ļ����Ȼ���
	GLint a = control->gtimer->GetEngineFPS(glutGet(GLUT_ELAPSED_TIME));
	char buff[512]={0};sprintf_s(buff,sizeof(buff)-1,"fps:%d",a);
	control->fontOutput(0,0,t);
	//drawQuads();
	control->DrawSector();
	
};