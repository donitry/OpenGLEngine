
/* struct ************************************************************************************************************/

typedef struct tagVERTEX						// 创建Vertex顶点结构
{
	float x, y, z;							// 3D 坐标
	float u, v;								// 纹理坐标
} VERTEX;								// 命名为VERTEX

typedef struct tagTRIANGLE					// 创建Triangle三角形结构
{
	VERTEX vertex[3];						// VERTEX矢量数组，大小为3
} TRIANGLE;										// 命名为 TRIANGLE

typedef struct tagSECTOR						// 创建Sector区段结构
{
	uLong numtriangles;						// Sector中的三角形个数
	TRIANGLE* triangle;						// 指向三角数组的指针
} SECTOR;




/* CLASS ************************************************************************************/
class Sector
{
	public:
		uInt Version;
		uLong sectorID;
		SECTOR tagSector;
		BOOL setSector(uInt width,uInt height,float scale);
	public:
		Sector();
		~Sector();
	private:	
		void getSectorFormFile(char* fileName);
};
Sector::Sector(){};
Sector::~Sector()
{
	delete[] tagSector.triangle;
};

/* MEMBER FUNCTION ******************************************************************/
BOOL Sector::setSector(uInt width, uInt height,float scale)
{
	tagSector.numtriangles = (width/scale)*(height / scale) *2;
	tagSector.triangle = new TRIANGLE[tagSector.numtriangles];	
	uLong temp = 0;
	for(float i=0;i<width;i+=scale)
	{
		for(float j=0;j<height;j+=scale)
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