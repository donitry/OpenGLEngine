#pragma once;
/* struct ************************************************************************************************************/

typedef struct tagVERTEX						// ����Vertex����ṹ
{
	float x, y, z;							// 3D ����
	float u, v;								// ��������
} VERTEX;								// ����ΪVERTEX

typedef struct tagTRIANGLE					// ����Triangle�����νṹ
{
	VERTEX vertex[3];						// VERTEXʸ�����飬��СΪ3
} TRIANGLE;										// ����Ϊ TRIANGLE

typedef struct tagSECTOR						// ����Sector���νṹ
{
	GLulong numtriangles;						// Sector�е������θ���
	TRIANGLE* triangle;						    // ָ�����������ָ��
	tagSECTOR():numtriangles(0),triangle(NULL){}
} SECTOR;




/* CLASS ************************************************************************************/
class Sector
{
	public:
		UINT Version;
		ULONG sectorID;
		SECTOR tagSector;
		BOOL setSector(uInt width,uInt height,float scale);
		void RenderSector(GLboolean);
	public:
		Sector();
		~Sector();
	private:	
		void getSectorFormFile(char* fileName);
};
Sector::Sector(){};
Sector::~Sector()
{
	if(this->tagSector.numtriangles>0){
		delete[] tagSector.triangle;tagSector.triangle=NULL;
	}//end if
};

/* MEMBER FUNCTION ******************************************************************/
BOOL Sector::setSector(GLuint width, GLuint height,float scale)
{
	tagSector.numtriangles = (width/scale)*(height / scale) *2;
	tagSector.triangle = new TRIANGLE[tagSector.numtriangles];	
	GLulong temp = 0;
	for(GLuint i=0;i<width;i+=scale)
	{
		for(GLuint j=0;j<height;j+=scale)
		{
			if(temp >= tagSector.numtriangles) break;

			tagSector.triangle[temp].vertex[0].x = (float)i;
			tagSector.triangle[temp].vertex[0].z = (float)j;
			tagSector.triangle[temp].vertex[0].y = 0.0f;
			tagSector.triangle[temp].vertex[0].u = 1.0f;
			tagSector.triangle[temp].vertex[0].v = 0.0f;


			tagSector.triangle[temp].vertex[1].x = (float)i;
			tagSector.triangle[temp].vertex[1].z = (float)j + scale;
			tagSector.triangle[temp].vertex[1].y = 0.0f;
			tagSector.triangle[temp].vertex[1].u = 1.0f;
			tagSector.triangle[temp].vertex[1].v = 1.0f;

			tagSector.triangle[temp].vertex[2].x = (float)i + scale;
			tagSector.triangle[temp].vertex[2].z = (float)j + scale;
			tagSector.triangle[temp].vertex[2].y = 0.0f;
			tagSector.triangle[temp].vertex[2].u = 0.0f;
			tagSector.triangle[temp].vertex[2].v = 1.0f;

			tagSector.triangle[temp+1].vertex[0].x = (float)i + scale;
			tagSector.triangle[temp+1].vertex[0].z = (float)j + scale;
			tagSector.triangle[temp+1].vertex[0].y = 0.0f;
			tagSector.triangle[temp+1].vertex[0].u = 0.0f;
			tagSector.triangle[temp+1].vertex[0].v = 1.0f;

			tagSector.triangle[temp+1].vertex[1].x = (float)i + scale;
			tagSector.triangle[temp+1].vertex[1].z = (float)j;
			tagSector.triangle[temp+1].vertex[1].y = 0.0f;
			tagSector.triangle[temp+1].vertex[1].u = 0.0f;
			tagSector.triangle[temp+1].vertex[1].v = 0.0f;

			tagSector.triangle[temp+1].vertex[2].x = (float)i;
			tagSector.triangle[temp+1].vertex[2].z = (float)j;
			tagSector.triangle[temp+1].vertex[2].y = 0.0f;
			tagSector.triangle[temp+1].vertex[2].u = 1.0f;
			tagSector.triangle[temp+1].vertex[2].v = 0.0f;
			
			temp += 2;
		}
	}
	return TRUE;
};

void Sector::RenderSector(GLboolean isTexture)
{
	if(this->tagSector.numtriangles>0)
	{
		for(ULONG i=0;i<this->tagSector.numtriangles;i++)
		{
			glBegin(isTexture?GL_TRIANGLES:GL_LINE_LOOP);
			for(int j = 0;j<3;j++)
			{
				glTexCoord2f(this->tagSector.triangle[i].vertex[j].u,this->tagSector.triangle[i].vertex[j].v);
				glVertex3f(this->tagSector.triangle[i].vertex[j].x,this->tagSector.triangle[i].vertex[j].y,this->tagSector.triangle[i].vertex[j].z);
			}
			glEnd();
		}
	}
};