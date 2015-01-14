#include "EngineHead.h"

GLfloat	LightAmbient[]	= { 0.5f, 0.5f, 0.5f, 1.0f }; 				// 环境光参数(环境光)
GLfloat	LightDiffuse[]		= { 1.0f, 1.0f, 1.0f, 1.0f };				// 漫射光参数(点光源)
GLfloat	LightPosition[]	= { 0.0f, 0.0f, 2.0f, 1.0f };				// 光源位置

void SetLight()
{
	//设置光源 exmaple 07
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);				// 设置环境光
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);					// 设置漫射光
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);				// 设置光源位置

	glEnable(GL_LIGHT1);																	// 启用一号光源
};
