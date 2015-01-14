
#include <stdafx.h>

class CCamera 
{
	protected:
		CVertex3f m_position; //λ��
		float m_azimuth;  //��λ��
		float m_elevation;  //����
		float m_roll;   //ת��

	private:
	//������Ӱ��
		void Update( void );
      
	public:
		CCamera(){ Initialize(); }
		~CCamera(){}
  
		//��ʼ����Ӱ��
		void Initialize();
  
		/////////////////////////////
		//������Ӱ��
		///////////////////////////
		//������Ӱ��λ��
		void SetPosition( float x, float y, float z );
		void SetPosition( CVertex3f &Pos );
		//������Ӱ����λ��
		void  SetAzimuth( float angle );
		//������Ӱ������
		void  SetElevation( float angle );
		//������Ӱ����� 
		void  SetRoll( float angle );
		//���ٿռ�ĳĿ���
		void Target( float targetx, float targety, float targetz );
		void Target( CVertex3f &target );
   
		//�ÿռ���Ϣ������Ӱ��
		void Drive( CSpaceInfo& SpaceInfo );

		//////////////////////////////
		//ʹ����Ӱ��
		////////////////////////////
		void GetMatrix( CMatrix& matrix );   //��ñ任����
		void GetContraryMatrix( CMatrix& matrix );  //�������任����

		//�����Ӱ��XYZ������ 
		bool GetXYZAxialVector( CVector3f* pVector );

		////////////////////////////////////////
		//�����Ӱ��������Ϣ
		//////////////////////////////////////////
		void GetPosition( CVertex3f &Pos );
		float GetAzimuth();
		float GetElevation();
		float GetRoll();
  
 };


//////////////////////////////////
//CCamera implement
//////////////////////////

//��Ӱ����ʼ��
void CCamera::Initialize()
{
	m_position.Set( 0, 0, 0 );
	m_azimuth = 90;
	m_elevation = 180;
	m_roll = 0;
}

/////////////////////////////
//������Ӱ��
///////////////////////////
//������Ӱ��λ��
void CCamera::SetPosition( float x, float y, float z )
{
	m_position.Set( x, y, z );
}
void CCamera::SetPosition( CVertex3f &Pos )
{
	m_position.Copy( Pos );
}

//���÷�λ��
void  CCamera::SetAzimuth( float angle )
{
	m_azimuth = angle;
}

//�������� 
void  CCamera::SetElevation( float angle )
{
	m_elevation = angle;
}

//�������
void  CCamera::SetRoll( float angle )
{
	m_roll = angle;
}

//����Ŀ��
void CCamera::Target( float targetx, float targety, float targetz )
{
	CVertex3f target( targetx, targety, targetz );
	CVector3f v( 0.0f, 0.0f, 0.0f );
	m_position.GetVector( target, v );
	m_azimuth = v.GetAzimuth();
	m_elevation = v.GetElevation();
}
void CCamera::Target( CVertex3f &target )
{
	CVector3f v( 0.0f, 0.0f, 0.0f );
	m_position.GetVector( target, v );
	m_azimuth = v.GetAzimuth();
	m_elevation = v.GetElevation();
}

//�ÿռ���Ϣ������Ӱ��
void CCamera::Drive( CSpaceInfo& SpaceInfo )
{
	m_azimuth = SpaceInfo.m_azimuth;
	m_elevation = SpaceInfo.m_elevation;
	m_roll = SpaceInfo.m_roll;
	m_position.Copy( SpaceInfo.m_position );
}

///////////////////////
//ʹ����Ӱ��
///////////////////////////
//��ñ任����
void CCamera::GetMatrix( CMatrix& matrix )
{
	matrix.Identity();
	matrix.Translate( -m_position.X(), -m_position.Y(), -m_position.Z() );
	matrix.RotateY( 90 - m_azimuth );
	matrix.RotateX( 180 - m_elevation );
	matrix.RotateZ( -m_roll );
}
//�������任����
void CCamera::GetContraryMatrix( CMatrix& matrix )
{
	matrix.Identity();
	matrix.RotateZ( m_roll );
	matrix.RotateX( m_elevation - 180 );
	matrix.RotateY( m_azimuth - 90 );
	matrix.Translate( m_position.X(), m_position.Y(), m_position.Z() );
}

//�����Ӱ����ǰ,����,�������� 
bool CCamera::GetXYZAxialVector( CVector3f* pVector )
{
	if( NULL == pVector )
	{
		return false;
	}

	pVector[0].Set( 1.0f, 0.0f, 0.0f );
	pVector[1].Set( 0.0f, 1.0f, 0.0f );
	pVector[2].Set( 0.0f, 0.0f, -1.0f );

	CMatrix T;
	T.RotateZ( m_roll );
	T.RotateX( m_elevation - 180 );
	T.RotateY( m_azimuth - 90.0f );

	T.Transform( pVector, 3 );

	return true;
}

////////////////////////////////////////
//�����Ӱ��������Ϣ
//////////////////////////////////////////
//�����Ӱ��λ�� 
void CCamera::GetPosition( CVertex3f &Pos )
{
	Pos.Copy( m_position );
}

//��÷�λ��
float CCamera::GetAzimuth( )
{
	return m_azimuth;
}

//�������
float CCamera::GetElevation( )
{
	return m_elevation;
}

//������
float CCamera::GetRoll( )
{
	return m_roll;
}  
//////////////// 