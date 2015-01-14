/* STRUCT *************************************************************************/
typedef struct{
	GLdouble x,y,z;
}EYEPOSTION;

typedef struct{
	GLdouble x,y,z;
}LOOKPOSTION;

typedef struct{
	GLdouble x,y,z;
}UPVERTEX;

class Camera
{
	public:
		int Version;
		EYEPOSTION eyePostion;
		LOOKPOSTION lookPostion;
		UPVERTEX upVertex;
	public:
		void InitialCameraPostion();
		void RollCameraPostion(GLdouble rad,GLdouble lookUp,GLdouble lookAngle);
		void setCameraPostion(GLdouble x,GLdouble y,GLdouble z);
		void setCameraDirection(GLdouble x,GLdouble y,GLdouble z);
		void setCameraUpside(GLdouble x,GLdouble y,GLdouble z);
	public:
		Camera();
		~Camera();
	private:
		void UpdateCamera();
};

/* MEMBER FUNCTION **************************************************************************************************/
Camera::Camera()
{
	this->Version = ENGINEVERSION;
};

Camera::~Camera(){};

void Camera::InitialCameraPostion()
{
	glMatrixMode(GL_MODELVIEW);						// 选择模型观察矩阵	
	glLoadIdentity();														// 重置投影矩阵
	this->eyePostion.x = this->eyePostion.y = this->eyePostion.z = 0.0;
	this->lookPostion.x = this->lookPostion.y = this->lookPostion.z = 0.0;
	this->upVertex.x = this->upVertex.z = 0.0;
	this->upVertex.y = 1.0;
	gluLookAt(this->eyePostion.x,this->eyePostion.y,this->eyePostion.z ,
						this->lookPostion.x,this->lookPostion.y,this->lookPostion.z,
						this->upVertex.x,this->upVertex.y,this->upVertex.z); //暂时只提供正立图像
};

void Camera::RollCameraPostion(GLdouble rad, GLdouble lookUp, GLdouble lookAngle)
{
	/*x, y, z 是摄像机的位置，alpha，beta就是公式中的α，β
	dst 是公式中的半径r
	球体的参数方程
	x=r * sin(α) * cos(β)
	y=r * sin(α) * sin(β)
	z=r * cos(α)
	（这里z轴是纵轴， y轴指向右方， x轴指向垂直于纸面的方向）
	r是球的半径，α角是半径与Z轴正向的夹角，β角是半径在xy平面的投影与x轴正向的夹角，
	他们的取值范围是 0≤ r ≤ ∞， 0≤ α ≤ π， 0 ≤ β ≤ 2π。)*/
	//这里值得注意，OpenGL的坐标系是x轴向右，y轴向上，z轴向前
	//在赋值的时候需要交换公式中的坐标轴，需要做的变换如下：
	//Zogl  =  X公式  
	//Xogl  =  Y公式   
	//Yogl  =  Z公式  
	//degrees2radians() 函数是我自己定义的角度转弧度，C库函数sin() 和cos()的参数 
	//是弧度

	this->eyePostion.x =    rad * sin		(DEGTORAD (lookUp)) * sin		(DEGTORAD (lookAngle))  + lookPostion.x;
	this->eyePostion.y =    rad * cos		(DEGTORAD (lookUp)) + lookPostion.y;
	this->eyePostion.z =    rad * sin		(DEGTORAD (lookUp)) * cos		(DEGTORAD (lookAngle))	+ lookPostion.z;

	this->UpdateCamera();
};

void Camera::setCameraPostion(GLdouble x, GLdouble y,GLdouble z)
{
	this->eyePostion.x = x;
	this->eyePostion.y = y;
	this->eyePostion.z = z;
	this->UpdateCamera();
};

void Camera::setCameraDirection(GLdouble x, GLdouble y, GLdouble z)
{
	this->lookPostion.x = x;
	this->lookPostion.y = y;
	this->lookPostion.z = z;
	this->UpdateCamera();
};

void Camera::setCameraUpside(GLdouble x, GLdouble y, GLdouble z)
{
	this->upVertex.x = x;
	this->upVertex.y = y;
	this->upVertex.z = z;
	this->UpdateCamera();
};

void Camera::UpdateCamera()
{
	glMatrixMode(GL_MODELVIEW);						// 选择模型观察矩阵	
	glLoadIdentity();														// 重置投影矩阵
	gluLookAt(	this->eyePostion.x,this->eyePostion.y,this->eyePostion.z,
						this->lookPostion.x,this->lookPostion.y,this->lookPostion.z,
						this->upVertex.x,this->upVertex.y,this->upVertex.z); //暂时只提供正立图像
};