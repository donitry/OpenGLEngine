// TestEngine.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

void myinit(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK display(void);

GLubyte rasters[12] = {
	0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc,
	0xfc, 0xc0, 0xc0, 0xc0, 0xff, 0xff};

	void myinit(void)
	{
		glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
		glClearColor (0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void CALLBACK display(void)
	{
		glColor3f (1.0, 0.0, 1.0);
		glRasterPos2i (100, 200);
		glBitmap (8, 12, 0.0, 0.0, 20.0, 20.0, rasters);
		glBitmap (8, 12, 0.0, 0.0, 0.0, 0.0, rasters);

		glColor3f (1.0, 1.0, 0.0);
		glRasterPos2i (150, 200);
		glBitmap (8, 12, 0.0, 0.0, 0.0, 0.0, rasters);

		glFlush();
	}

	void CALLBACK myReshape(GLsizei w, GLsizei h)
	{
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho (0, w, 0, h, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
	}



int _tmain(int argc, _TCHAR* argv[])
{
	auxInitDisplayMode (AUX_SINGLE | AUX_RGBA);
	auxInitPosition (0, 0, 500, 500);
	auxInitWindow ("Bitmap");
	myinit();
	auxReshapeFunc (myReshape);
	auxMainLoop(display);
}

