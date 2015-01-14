
#include <stdafx.h>

class CCamera 
{
	protected:
		CVertex3f m_position; //位置
		float m_azimuth;  //方位角
		float m_elevation;  //仰角
		float m_roll;   //转角

	private:
	//更新摄影机
		void Update( void );
      
	public:
		CCamera(){ Initialize(); }
		~CCamera(){}
  
		//初始化摄影机
		void Initialize();
  
		/////////////////////////////
		//操纵摄影机
		///////////////////////////
		//设置摄影机位置
		void SetPosition( float x, float y, float z );
		void SetPosition( CVertex3f &Pos );
		//设置摄影机方位角
		void  SetAzimuth( float angle );
		//设置摄影机仰角
		void  SetElevation( float angle );
		//设置摄影机倾角 
		void  SetRoll( float angle );
		//跟踪空间某目标点
		void Target( float targetx, float targety, float targetz );
		void Target( CVertex3f &target );
   
		//用空间信息驱动摄影机
		void Drive( CSpaceInfo& SpaceInfo );

		//////////////////////////////
		//使用摄影机
		////////////////////////////
		void GetMatrix( CMatrix& matrix );   //获得变换矩阵
		void GetContraryMatrix( CMatrix& matrix );  //获得逆向变换矩阵

		//获得摄影机XYZ轴向量 
		bool GetXYZAxialVector( CVector3f* pVector );

		////////////////////////////////////////
		//获得摄影机各种信息
		//////////////////////////////////////////
		void GetPosition( CVertex3f &Pos );
		float GetAzimuth();
		float GetElevation();
		float GetRoll();
  
 };


//////////////////////////////////
//CCamera implement
//////////////////////////

//摄影机初始化
void CCamera::Initialize()
{
	m_position.Set( 0, 0, 0 );
	m_azimuth = 90;
	m_elevation = 180;
	m_roll = 0;
}

/////////////////////////////
//操纵摄影机
///////////////////////////
//设置摄影机位置
void CCamera::SetPosition( float x, float y, float z )
{
	m_position.Set( x, y, z );
}
void CCamera::SetPosition( CVertex3f &Pos )
{
	m_position.Copy( Pos );
}

//设置方位角
void  CCamera::SetAzimuth( float angle )
{
	m_azimuth = angle;
}

//设置仰角 
void  CCamera::SetElevation( float angle )
{
	m_elevation = angle;
}

//设置倾角
void  CCamera::SetRoll( float angle )
{
	m_roll = angle;
}

//跟踪目标
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

//用空间信息驱动摄影机
void CCamera::Drive( CSpaceInfo& SpaceInfo )
{
	m_azimuth = SpaceInfo.m_azimuth;
	m_elevation = SpaceInfo.m_elevation;
	m_roll = SpaceInfo.m_roll;
	m_position.Copy( SpaceInfo.m_position );
}

///////////////////////
//使用摄影机
///////////////////////////
//获得变换矩阵
void CCamera::GetMatrix( CMatrix& matrix )
{
	matrix.Identity();
	matrix.Translate( -m_position.X(), -m_position.Y(), -m_position.Z() );
	matrix.RotateY( 90 - m_azimuth );
	matrix.RotateX( 180 - m_elevation );
	matrix.RotateZ( -m_roll );
}
//获得逆向变换矩阵
void CCamera::GetContraryMatrix( CMatrix& matrix )
{
	matrix.Identity();
	matrix.RotateZ( m_roll );
	matrix.RotateX( m_elevation - 180 );
	matrix.RotateY( m_azimuth - 90 );
	matrix.Translate( m_position.X(), m_position.Y(), m_position.Z() );
}

//获得摄影机向前,向右,向上向量 
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
//获得摄影机各种信息
//////////////////////////////////////////
//获得摄影机位置 
void CCamera::GetPosition( CVertex3f &Pos )
{
	Pos.Copy( m_position );
}

//获得方位角
float CCamera::GetAzimuth( )
{
	return m_azimuth;
}

//获得仰角
float CCamera::GetElevation( )
{
	return m_elevation;
}

//获得倾角
float CCamera::GetRoll( )
{
	return m_roll;
}  
//////////////// 