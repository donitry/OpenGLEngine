#include "EngineHead.h"

GLfloat	LightAmbient[]	= { 0.5f, 0.5f, 0.5f, 1.0f }; 				// ���������(������)
GLfloat	LightDiffuse[]		= { 1.0f, 1.0f, 1.0f, 1.0f };				// ��������(���Դ)
GLfloat	LightPosition[]	= { 0.0f, 0.0f, 2.0f, 1.0f };				// ��Դλ��

void SetLight()
{
	//���ù�Դ exmaple 07
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);				// ���û�����
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);					// ���������
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);				// ���ù�Դλ��

	glEnable(GL_LIGHT1);																	// ����һ�Ź�Դ
};
