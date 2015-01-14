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
	glMatrixMode(GL_MODELVIEW);						// ѡ��ģ�͹۲����	
	glLoadIdentity();														// ����ͶӰ����
	this->eyePostion.x = this->eyePostion.y = this->eyePostion.z = 0.0;
	this->lookPostion.x = this->lookPostion.y = this->lookPostion.z = 0.0;
	this->upVertex.x = this->upVertex.z = 0.0;
	this->upVertex.y = 1.0;
	gluLookAt(this->eyePostion.x,this->eyePostion.y,this->eyePostion.z ,
						this->lookPostion.x,this->lookPostion.y,this->lookPostion.z,
						this->upVertex.x,this->upVertex.y,this->upVertex.z); //��ʱֻ�ṩ����ͼ��
};

void Camera::RollCameraPostion(GLdouble rad, GLdouble lookUp, GLdouble lookAngle)
{
	/*x, y, z ���������λ�ã�alpha��beta���ǹ�ʽ�еĦ�����
	dst �ǹ�ʽ�еİ뾶r
	����Ĳ�������
	x=r * sin(��) * cos(��)
	y=r * sin(��) * sin(��)
	z=r * cos(��)
	������z�������ᣬ y��ָ���ҷ��� x��ָ��ֱ��ֽ��ķ���
	r����İ뾶�������ǰ뾶��Z������ļнǣ��½��ǰ뾶��xyƽ���ͶӰ��x������ļнǣ�
	���ǵ�ȡֵ��Χ�� 0�� r �� �ޣ� 0�� �� �� �У� 0 �� �� �� 2�С�)*/
	//����ֵ��ע�⣬OpenGL������ϵ��x�����ң�y�����ϣ�z����ǰ
	//�ڸ�ֵ��ʱ����Ҫ������ʽ�е������ᣬ��Ҫ���ı任���£�
	//Zogl  =  X��ʽ  
	//Xogl  =  Y��ʽ   
	//Yogl  =  Z��ʽ  
	//degrees2radians() ���������Լ�����ĽǶ�ת���ȣ�C�⺯��sin() ��cos()�Ĳ��� 
	//�ǻ���

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
	glMatrixMode(GL_MODELVIEW);						// ѡ��ģ�͹۲����	
	glLoadIdentity();														// ����ͶӰ����
	gluLookAt(	this->eyePostion.x,this->eyePostion.y,this->eyePostion.z,
						this->lookPostion.x,this->lookPostion.y,this->lookPostion.z,
						this->upVertex.x,this->upVertex.y,this->upVertex.z); //��ʱֻ�ṩ����ͼ��
};