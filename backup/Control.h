
#include "EngineHead.h"
#include "texture.h"
#include "Sector.h"
#include "Camera.h"
#include "Light.h"

/* STRCUT *****************************************************************************/
typedef struct
{
	bool isBlend;						//是否支持混合模式
	bool isTexture;					//材质模式还是线条显示
}SECTORSTYLE;

typedef struct
{
	GLdouble obRad;				//观察半径
	GLdouble obAUp;				//观察仰角
	GLdouble obAOffset;		//观察偏角
	GLdouble obSx;				//观察位置的X坐标
	GLdouble obSy;				//观察位置的Y坐标
	GLdouble obSz;				//观察位置的Z坐标
}OBSERVER;

/* CLASS *******************************************************************************/
class Control
{
	public:
		int version;
		OBSERVER	obServer;
		SECTORSTYLE sectorStyle;
		Sector *sector;
		Camera *camera;
	public:
		void InitialSector(uInt width,uInt height,float scale);
		void InitialobServer();
		void MoveobServer(GLdouble up,GLdouble offset,bool isAuto = true);
		void TwistobServer(GLdouble destX,GLdouble destY,GLdouble destZ,bool isAuto = true);
		void PullobServer(GLdouble distance,bool isAuto = true);
		void DrawSector();
	public:
		Control();
		~Control();
};

/* MEMBER FUNCTION ***************************************************************/
Control::Control()
{
	this->version = ENGINEVERSION;
	sector		= new Sector();
	camera	= new Camera();
	sectorStyle.isBlend		= false;
	sectorStyle.isTexture	= false;
	obServer.obRad			= 5.0;
	obServer.obAUp			=	45.0;
	obServer.obAOffset		=	0.0;
	obServer.obSx				=	1.0;
	obServer.obSy				=	0.0;
	obServer.obSz				=	1.0;
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
};

//初始化一切默认参数
void Control::InitialSector(uInt width, uInt height, float scale)
{
	InitialTexture();
	glBindTexture(GL_TEXTURE_2D, TextureName[3]);
	this->sector->setSector(width,height,scale);
	if(sectorStyle.isBlend){
		glDisable(GL_DEPTH_TEST);
		glColor4f(1.0f,1.0f,1.0f,1.0f);								// 全亮度， 50% Alpha 混合
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);		// 基于源象素alpha通道值的半透明混合函数
		glEnable(GL_BLEND);											//混合启动
	}else{
		glEnable(GL_DEPTH_TEST);								// 启用深度测试(启用的情况下，混合失效——即不透明状态)
	};
	if(sectorStyle.isTexture){glEnable(GL_TEXTURE_2D);								// 启用纹理映射
	}else{glDisable(GL_TEXTURE_2D);}
};

void Control::InitialobServer()
{
	this->camera->InitialCameraPostion();
	this->camera->setCameraDirection(	0.0,0.0,0.0);
	this->camera->RollCameraPostion(5.0,45.0,0.0);	
};

void Control::MoveobServer(GLdouble up,GLdouble offset,bool isAuto)
{
	obServer.obAUp		=	isAuto?obServer.obAUp+up:up;
	obServer.obAOffset	=	isAuto?obServer.obAOffset+offset:offset;
	this->camera->RollCameraPostion(obServer.obRad,obServer.obAUp,obServer.obAOffset);	
};

void Control::TwistobServer(GLdouble destX,GLdouble destY,GLdouble destZ,bool isAuto)
{
	obServer.obSx = isAuto?obServer.obSx+destX:destX;
	obServer.obSy = isAuto?obServer.obSy+destY:destY;
	obServer.obSz = isAuto?obServer.obSz+destZ:destZ;
	this->camera->setCameraDirection(obServer.obSx ,obServer.obSy ,obServer.obSz );
};

void Control::PullobServer(GLdouble distance,bool isAuto)
{
	obServer.obRad = isAuto?obServer.obRad+distance:distance;
	this->camera->RollCameraPostion(obServer.obRad,obServer.obAUp,obServer.obAOffset);	
};

void Control::DrawSector()
{
	if(sector->tagSector.numtriangles>0)
	{
		for(uLong i=0;i<sector->tagSector.numtriangles;i++)
		{
			glBegin(sectorStyle.isTexture?GL_TRIANGLES:GL_LINE_LOOP);
			for(int j = 0;j<3;j++)
			{
				glTexCoord2f(sector->tagSector.triangle[i].vertex[j].u,sector->tagSector.triangle[i].vertex[j].v);
				glVertex3f(sector->tagSector.triangle[i].vertex[j].x,sector->tagSector.triangle[i].vertex[j].y,sector->tagSector.triangle[i].vertex[j].z);
			}
			glEnd();
		}
	}
};

void Test(int x, int y, char* lpszText)
{	
	HFONT	hFont  =CreateFont(-12,0,0,0,400,0,0,0,GB2312_CHARSET,0,0,0,FF_MODERN,"Arial");
	HDC MDC = CreateCompatibleDC(NULL);	
	SIZE size;
	HGDIOBJ oldfont = SelectObject(MDC,hFont);
	GetTextExtentPoint32(MDC,lpszText,strlen(lpszText),&size);
	HBITMAP bitmap = CreateCompatibleBitmap(MDC,size.cx,size.cy);
	HGDIOBJ oldbitmap = SelectObject(MDC,bitmap);
	SetBkMode(MDC,0);	
	SetTextColor(MDC, RGB(255, 255, 255));
	TextOut(MDC, 0, 0, lpszText, strlen(lpszText));
	Bitmap bm(bitmap,NULL);
	size.cx = (bm.GetWidth() + 31) & (~31);
	int bufsize =size.cy * size.cx;	
	struct {  
		BITMAPINFOHEADER bih;
		RGBQUAD col[2];
	}bic;
	BITMAPINFO *binf = (BITMAPINFO *)&bic;       
	binf->bmiHeader.biSize     = sizeof(binf->bmiHeader);
	binf->bmiHeader.biWidth    = bm.GetWidth();	
	binf->bmiHeader.biHeight   = bm.GetHeight();	
	binf->bmiHeader.biPlanes   = 1;         
	binf->bmiHeader.biBitCount = 1;             
	binf->bmiHeader.biCompression = BI_RGB;     
	binf->bmiHeader.biSizeImage   = bufsize; 
	UCHAR* Bits = new UCHAR[bufsize];
	GetDIBits(MDC,bitmap,0,bm.GetHeight(),Bits,binf,DIB_RGB_COLORS); 
	glPixelStorei(GL_UNPACK_ALIGNMENT ,1);
	glRasterPos3i(x,0,y);          
	glOrtho(x,x+size.cx,0,100,y,y+size.cy);
	//glRasterPos3i(x,y,0);
	//glOrtho(0,200,300,0,0,1);
	glBitmap(size.cx,size.cy,0,0,0,0,Bits);
	delete[] Bits;  
	SelectObject(MDC,oldfont);
	DeleteObject(hFont);
	SelectObject(MDC,oldbitmap);
	DeleteObject(bitmap);
	DeleteDC(MDC);
};