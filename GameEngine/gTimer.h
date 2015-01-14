#pragma once;

typedef struct tFpsCount
{
	GLint t_cur_frame;
	GLint t_old_frame;
	GLint t_cur_time;
	GLint t_old_time;
	GLulong t_pri_span;
	tFpsCount():t_cur_frame(0),t_old_frame(0),t_cur_time(0),t_old_time(0),t_pri_span(0){}
}FPSCOUNT;

class gTimer
{
	public:
		int version;
		FPSCOUNT fpsCount;
	public://fps¿ØÖÆ
		GLint GetEngineFPS(GLulong);
		GLboolean LimitEngineFPS(GLulong,GLint fps=60);
	public:
		gTimer();~gTimer();
};

gTimer::gTimer(){};
gTimer::~gTimer(){};

GLint gTimer::GetEngineFPS(GLulong curr)
{
	this->fpsCount.t_cur_time = curr;this->fpsCount.t_cur_frame++;
	if(this->fpsCount.t_cur_time - this->fpsCount.t_old_time >= 1000){
		this->fpsCount.t_old_frame = this->fpsCount.t_cur_frame*1000/(this->fpsCount.t_cur_time-this->fpsCount.t_old_time);
		this->fpsCount.t_old_time = this->fpsCount.t_cur_time;this->fpsCount.t_cur_frame=0;
	}return this->fpsCount.t_old_frame;
};

GLboolean gTimer::LimitEngineFPS(GLulong curr, GLint fps)
{
	if(curr-this->fpsCount.t_pri_span >= 1000/fps){this->fpsCount.t_pri_span = curr;return GL_TRUE;}
	return GL_FALSE;
};