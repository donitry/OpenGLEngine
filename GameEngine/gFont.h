#pragma once
#include "EngineHead.h"

class gFont
{
	public:
		int version;
	public:
		void Printftext (float x, float y, char* lpszText);
	public:
		gFont();
		~gFont();
};

gFont::gFont(){};
gFont::~gFont(){};

void gFont::Printftext (float x, float y, char* lpszText)
{	
	HFONT	hFont  =CreateFont(14,0,0,0,400,0,0,0,GB2312_CHARSET,0,0,0,FF_MODERN,"Arial");
	HDC MDC = CreateCompatibleDC(NULL);	SIZE size;
	HGDIOBJ oldfont = SelectObject(MDC,hFont);
	COLORREF a = SetTextColor(MDC, RGB(255,255,255));
	GetTextExtentPoint32(MDC,lpszText,strlen(lpszText),&size);
	HBITMAP bitmap = CreateCompatibleBitmap(MDC,size.cx,size.cy);
	HGDIOBJ oldbitmap = SelectObject(MDC,bitmap);
	SetBkMode(MDC,0);	
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
	glColor3f (0.0, 1.0, 1.0);
	glRasterPos3f(0.0f,0.0f,0.0f);          
// 	glOrtho(x,x+size.cx,0,100,y,y+size.cy);
// 	glRasterPos3i(x,y,0);
// 	glOrtho(0,200,300,0,0,1);
//	glBitmap(size.cx,size.cy,0,0,0,0,Bits);
// 
// 	glColor3f (1.0, 1.0, 0.0);
// 	glRasterPos3f(0.0f,10.0f,0.0f); 
	glBitmap(size.cx,size.cy,500,-250,0,0,Bits);
	glColor3f (1.0, 1.0, 1.0);
	glRasterPos3f(0.0f,0.0f,0.0f);    
// 	static GLint    ImageWidth;
// 	static GLint    ImageHeight;
// 	static GLint    PixelLength;
// 	static GLubyte* PixelData;
// 	// 计算像素数据长度
// 	PixelLength = size.cx * 3;
// 	while( PixelLength % 4 != 0 )
// 		++PixelLength;
// 	PixelLength *= size.cy;
// 
// 	// 读取像素数据
// 	PixelData = (GLubyte*)malloc(PixelLength);
// 
// 	glDrawPixels(size.cx,size.cy,GL_BGR_EXT,GL_UNSIGNED_BYTE,PixelData);
	delete[] Bits;  
	SelectObject(MDC,oldfont);
	DeleteObject(hFont);
	SelectObject(MDC,oldbitmap);
	DeleteObject(bitmap);
	DeleteDC(MDC);
};