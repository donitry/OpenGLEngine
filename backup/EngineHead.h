#include "stdafx.h"

/* DEFINES **************************************************************************/
#define ENGINEVERSION					1

//MATH
#define DEGTORAD(A)	((A * M_PI) / 180.0f)  //度数->弧度
#define RADTODEG(A)	((A * 180.0f) / M_PI)  //弧度->度数

//INITIALFUNTION
#define MAKEVECTOR(a,vx,vy,vz)	a.x = vx; a.y = vy; a.z = vz;   //初始化各点坐标的宏

//TYPE
typedef unsigned int		uInt;
typedef unsigned long	uLong;
