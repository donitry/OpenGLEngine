#pragma once
#include "EngineHead.h"

typedef struct
{
	GLubyte* imageData;			// ��������ͼ�����ɫֵ
	GLuint  bpp;							// ���Ƶ�λ���ص�bit��
	GLuint width;							// ����ͼ��Ŀ��
	GLuint height;							// ����ͼ��ĸ߶�
	GLuint texID;							// ʹ��glBindTexture���������ID.
	GLuint type;			 				// �����洢��*ImageData�е�����(GL_RGB Or GL_RGBA)
} tgaTexture;

typedef struct
{
	GLubyte Header[12];			// �ļ�ͷ�����ļ�����
} TGAHeader;

typedef struct
{
	GLubyte header[6];				// ����ǰ6���ֽ�
	GLuint bytesPerPixel;			// ÿ���ص��ֽ��� (3 �� 4)
	GLuint imageSize;				// ���ƴ洢ͼ��������ڴ�ռ�
	GLuint type;				// ͼ������ GL_RGB �� GL_RGBA
	GLuint Height;				// ͼ��ĸ߶�
	GLuint Width;				// ͼ����
	GLuint Bpp;				// ÿ���صı����� (24 �� 32)
} TGA;

TGAHeader tgaheader;				// �����洢���ǵ��ļ�ͷ
TGA tga;											// �����洢�ļ���Ϣ

// δѹ����TGAͷ
GLubyte uTGAcompare[12] = {0,0, 2,0,0,0,0,0,0,0,0,0};
// ѹ����TGAͷ
GLubyte cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0};

bool LoadUncompressedTGA(tgaTexture * texture, char * filename, FILE * fTGA);
bool LoadCompressedTGA(tgaTexture * texture, char * filename, FILE * fTGA);

bool LoadTGA(tgaTexture * texture, char * filename)
{
	FILE * fTGA;					// �����ļ�ָ��
	fTGA = fopen(filename, "rb");			// �Զ�ģʽ���ļ�
	if(fTGA == NULL)				// ����˴��д���
	{
		return false;					// ���� False
	}

	if(fread(&tgaheader, sizeof(TGAHeader), 1, fTGA) == 0)
	{
		/*...Error code here...*/
		return false;				//  ���ʧ���򷵻� False
	}

	// ����ļ�ͷ����δѹ�����ļ�ͷ��ʽ
	if(memcmp(uTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)
	{
		// ��ȡδѹ����TGA�ļ�
		LoadUncompressedTGA(texture, filename, fTGA);
	}
	// ����ļ�ͷ����ѹ�����ļ�ͷ��ʽ
	else if(memcmp(cTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)
	{
		// ��ȡѹ����TGA��ʽ
		LoadCompressedTGA(texture, filename, fTGA);
	}
	else						// �����һ����������
	{
		/*...Error code here...*/
		return false;				// ���� False
	}
	return true;
};

//��ȡδѹ����TGA�ļ�
bool LoadUncompressedTGA(tgaTexture * texture, char * filename, FILE * fTGA)
{
	// ���Լ�����ȡ6���ֽڵ�����
	if(fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)
	{
		/*...Error code here...*/
		return false;				// ���� False
	}

	texture->width  = tga.header[1] * 256 + tga.header[0];	// ����߶�
	texture->height = tga.header[3] * 256 + tga.header[2];	// ������
	texture->bpp = tga.header[4];			// ����BPP
	tga.Width = texture->width;				// ����Width�����ؽṹ��ȥ
	tga.Height = texture->height;			// ����Height�����ؽṹ��ȥ
	tga.Bpp = texture->bpp;				// ����Bpp�����ؽṹ��ȥ
	// ȷ�����е���Ϣ������Ч��
	if((texture->width <= 0) || (texture->height <= 0) || ((texture->bpp != 24) && (texture->bpp !=32)))
	{
		/*...Error code here...*/
		return false;				// ���� False
	}
	if(texture->bpp == 24)				// ��24 bitͼ����
	{
		texture->type	= GL_RGB;			//����ǣ���������ΪGL_RGB
	}
	else						// �������24bit,�����32bit
	{
		texture->type	= GL_RGBA;		//������������ΪGL_RGBA
	}
	tga.bytesPerPixel = (tga.Bpp / 8);		// ����BPP
	// ����洢ͼ��������ڴ�
	tga.imageSize = (tga.bytesPerPixel * tga.Width * tga.Height);
	// �����ڴ�
	texture->imageData = (GLubyte *)malloc(tga.imageSize);
	if(texture->imageData == NULL)			// ȷ���Ѿ�����ɹ�
	{
		/*...Error code here...*/
		return false;				// ȷ���Ѿ�����ɹ�
	}
	// ���Զ�ȡ����ͼ������
	if(fread(texture->imageData, 1, tga.imageSize, fTGA) != tga.imageSize)
	{
		/*...Error code here...*/
		return false;				// ������ܣ�����false
	}
	//  ��ʼѭ��
	for(GLuint cswap = 0; cswap < (int)tga.imageSize; cswap += tga.bytesPerPixel)
	{
		// ��һ�ֽ� XOR�����ֽ�XOR ��һ�ֽ� XOR �����ֽ�
		texture->imageData[cswap] ^= texture->imageData[cswap+2] ^=
		texture->imageData[cswap] ^= texture->imageData[cswap+2];
	}

	fclose(fTGA);					// �ر��ļ�
	return true;					// ���سɹ�
};

bool LoadCompressedTGA(tgaTexture * texture, char * filename, FILE * fTGA)
{
	if(fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)
	{
		/*...Error code here...*/
	}
	texture->width  = tga.header[1] * 256 + tga.header[0];
	texture->height = tga.header[3] * 256 + tga.header[2];
	texture->bpp	= tga.header[4];
	tga.Width	= texture->width;
	tga.Height	= texture->height;
	tga.Bpp	= texture->bpp;
	if((texture->width <= 0) || (texture->height <= 0) || ((texture->bpp != 24) && (texture->bpp !=32)))
	{
		/*...Error code here...*/
		return false;
	}								

	tga.bytesPerPixel	= (tga.Bpp / 8);
	tga.imageSize		= (tga.bytesPerPixel * tga.Width * tga.Height);
	// ����洢ͼ��������ڴ�ռ�
	texture->imageData	= (GLubyte *)malloc(tga.imageSize);
	if(texture->imageData == NULL)			// ������ܷ����ڴ�
	{
		/*...Error code here...*/
		return false;				// ���� False
	}
	GLuint pixelcount = tga.Height * tga.Width;	// ͼ���е�������
	GLuint currentpixel	= 0;		// ��ǰ���ڶ�ȡ������
	GLuint currentbyte	= 0;			// ��ǰ������ͼ����д�������
// һ�����صĴ洢�ռ�
	GLubyte * colorbuffer = (GLubyte *)malloc(tga.bytesPerPixel);
	do						// ��ʼѭ��
	{
		GLubyte chunkheader = 0;				// �洢Id��ֵ�ı���
		if(fread(&chunkheader, sizeof(GLubyte), 1, fTGA) == 0)	// ���Զ�ȡ���ͷ
		{
			/*...Error code...*/
			return false;				// If It Fails, Return False
		}
		if(chunkheader < 128)				// �����RAW��
		{
			chunkheader++;				// ����ֵ��1�Ի�ȡRAW���ص�����
			// ��ʼ���ض�ȡѭ��
			for(short counter = 0; counter < chunkheader; counter++)
			{
				// ���Զ�ȡһ������
				if(fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) != tga.bytesPerPixel)
				{
					/*...Error code...*/
					return false;			// ���ʧ�ܣ�����false
				}
				texture->imageData[currentbyte] = colorbuffer[2];		// д��R���ֽ�
				texture->imageData[currentbyte + 1	] = colorbuffer[1];	//д��G���ֽ�
				texture->imageData[currentbyte + 2	] = colorbuffer[0];	// д��B���ֽ�
				if(tga.bytesPerPixel == 4)					// �����32λͼ��...
				{
					texture->imageData[currentbyte + 3] = colorbuffer[3];	// д��A���ֽ�
				}
				else						// �����RLEͷ
				{
					chunkheader -= 127;			//  ��ȥ127���ID Bit��Rid
					// ��ȡ��һ������
					if(fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) != tga.bytesPerPixel)
					{
						/*...Error code...*/
						return false;				// ���ʧ�ܣ�����false
					}
						// ��ʼѭ��
					for(short counter = 0; counter < chunkheader; counter++)
					{
						// ������R���ֽ�
						texture->imageData[currentbyte] = colorbuffer[2];
						// ������G���ֽ�
						texture->imageData[currentbyte + 1	] = colorbuffer[1];
						// ������B���ֽ�
						texture->imageData[currentbyte + 2	] = colorbuffer[0];
						if(tga.bytesPerPixel == 4)		// �����32λͼ��
						{
							// ������A���ֽ�
							texture->imageData[currentbyte + 3] = colorbuffer[3];
						}
						currentbyte += tga.bytesPerPixel;	// �����ֽڼ�����
						currentpixel++;			// �����ֽڼ�����
					}
				}
				// ����ÿ���ص��ֽ��������ֽڼ�����
				currentbyte += tga.bytesPerPixel;
				currentpixel++;					// ���ؼ�������1
			}
		}
	}while(currentpixel < pixelcount);	// �Ƿ��и��������Ҫ��ȡ����ʼѭ��ֱ�����
		fclose(fTGA);			// �ر��ļ�
		return true;			// ���سɹ�
}



