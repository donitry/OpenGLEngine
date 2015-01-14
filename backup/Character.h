#include "EngineHead.h"

  void   glDrawString(unsigned   char   *str)  
  {  
          GLYPHMETRICSFLOAT   pgmf[1];  
   
          HDC   hDC=wglGetCurrentDC();  
          HFONT   hFont;  
          LOGFONT   lf   ;  
          memset(&lf,0,sizeof(LOGFONT));  
          lf.lfHeight   =   1   ;  
          lf.lfWidth   =   0   ;  
          lf.lfEscapement   =   0   ;  
          lf.lfOrientation   =   0   ;  
          lf.lfWeight   =   FW_NORMAL   ;  
          lf.lfItalic   =   FALSE   ;  
          lf.lfUnderline   =   FALSE   ;  
          lf.lfStrikeOut   =   FALSE   ;  
          lf.lfCharSet   =   GB2312_CHARSET   ;  
          lf.lfOutPrecision   =   OUT_TT_PRECIS   ;  
          lf.lfClipPrecision=   CLIP_DEFAULT_PRECIS   ;  
          lf.lfQuality   =   PROOF_QUALITY   ;  
          lf.lfPitchAndFamily   =   VARIABLE_PITCH   |   TMPF_TRUETYPE   |   FF_MODERN   ;  
          lstrcpy   (lf.lfFaceName,   "宋体")   ;  
          hFont   =   CreateFontIndirect(&lf);  
          //设置当前字体  
          SelectObject(wglGetCurrentDC(),hFont);  
   
   
          DWORD   dwChar;  
          int   ListNum;  
          for(size_t   i=0;i<strlen((char   *)str);i++)  
          {  
                  if(IsDBCSLeadByte(str[i]))  
                  {  
                          dwChar=(DWORD)((str[i]<<8)|str[i+1]);  
                          i++;  
                  }  
                  else  
                          dwChar=str[i];  
                  ListNum=glGenLists(1);  
                  wglUseFontOutlines(hDC,dwChar,1,ListNum,0.0,0.1f,WGL_FONT_POLYGONS,pgmf);  
                  glCallList(ListNum);  
                  glDeleteLists(ListNum,1);  
          }  
  }   ;