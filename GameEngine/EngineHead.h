#include "stdafx.h"

/* DEFINES **************************************************************************/
#define ENGINEVERSION					1

//MATH
#define DEGTORAD(A)	((A * M_PI) / 180.0f)  //����->����
#define RADTODEG(A)	((A * 180.0f) / M_PI)  //����->����

//INITIALFUNTION
#define MAKEVECTOR(a,vx,vy,vz)	a.x = vx; a.y = vy; a.z = vz;   //��ʼ����������ĺ�

//TYPE
typedef unsigned int		uInt;
typedef unsigned long	uLong;
