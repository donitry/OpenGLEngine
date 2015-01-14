#pragma once
#include "EngineHead.h"

typedef struct
{
	GLubyte* imageData;			// 控制整个图像的颜色值
	GLuint  bpp;							// 控制单位像素的bit数
	GLuint width;							// 整个图像的宽度
	GLuint height;							// 整个图像的高度
	GLuint texID;							// 使用glBindTexture所需的纹理ID.
	GLuint type;			 				// 描述存储在*ImageData中的数据(GL_RGB Or GL_RGBA)
} tgaTexture;

typedef struct
{
	GLubyte Header[12];			// 文件头决定文件类型
} TGAHeader;

typedef struct
{
	GLubyte header[6];				// 控制前6个字节
	GLuint bytesPerPixel;			// 每像素的字节数 (3 或 4)
	GLuint imageSize;				// 控制存储图像所需的内存空间
	GLuint type;				// 图像类型 GL_RGB 或 GL_RGBA
	GLuint Height;				// 图像的高度
	GLuint Width;				// 图像宽度
	GLuint Bpp;				// 每像素的比特数 (24 或 32)
} TGA;

TGAHeader tgaheader;				// 用来存储我们的文件头
TGA tga;											// 用来存储文件信息

// 未压缩的TGA头
GLubyte uTGAcompare[12] = {0,0, 2,0,0,0,0,0,0,0,0,0};
// 压缩的TGA头
GLubyte cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0};

bool LoadUncompressedTGA(tgaTexture * texture, char * filename, FILE * fTGA);
bool LoadCompressedTGA(tgaTexture * texture, char * filename, FILE * fTGA);

bool LoadTGA(tgaTexture * texture, char * filename)
{
	FILE * fTGA;					// 声明文件指针
	fTGA = fopen(filename, "rb");			// 以读模式打开文件
	if(fTGA == NULL)				// 如果此处有错误
	{
		return false;					// 返回 False
	}

	if(fread(&tgaheader, sizeof(TGAHeader), 1, fTGA) == 0)
	{
		/*...Error code here...*/
		return false;				//  如果失败则返回 False
	}

	// 如果文件头附合未压缩的文件头格式
	if(memcmp(uTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)
	{
		// 读取未压缩的TGA文件
		LoadUncompressedTGA(texture, filename, fTGA);
	}
	// 如果文件头附合压缩的文件头格式
	else if(memcmp(cTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)
	{
		// 读取压缩的TGA格式
		LoadCompressedTGA(texture, filename, fTGA);
	}
	else						// 如果任一个都不符合
	{
		/*...Error code here...*/
		return false;				// 返回 False
	}
	return true;
};

//读取未压缩的TGA文件
bool LoadUncompressedTGA(tgaTexture * texture, char * filename, FILE * fTGA)
{
	// 尝试继续读取6个字节的内容
	if(fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)
	{
		/*...Error code here...*/
		return false;				// 返回 False
	}

	texture->width  = tga.header[1] * 256 + tga.header[0];	// 计算高度
	texture->height = tga.header[3] * 256 + tga.header[2];	// 计算宽度
	texture->bpp = tga.header[4];			// 计算BPP
	tga.Width = texture->width;				// 拷贝Width到本地结构中去
	tga.Height = texture->height;			// 拷贝Height到本地结构中去
	tga.Bpp = texture->bpp;				// 拷贝Bpp到本地结构中去
	// 确认所有的信息都是有效的
	if((texture->width <= 0) || (texture->height <= 0) || ((texture->bpp != 24) && (texture->bpp !=32)))
	{
		/*...Error code here...*/
		return false;				// 返回 False
	}
	if(texture->bpp == 24)				// 是24 bit图像吗？
	{
		texture->type	= GL_RGB;			//如果是，设置类型为GL_RGB
	}
	else						// 如果不是24bit,则必是32bit
	{
		texture->type	= GL_RGBA;		//这样设置类型为GL_RGBA
	}
	tga.bytesPerPixel = (tga.Bpp / 8);		// 计算BPP
	// 计算存储图像所需的内存
	tga.imageSize = (tga.bytesPerPixel * tga.Width * tga.Height);
	// 分配内存
	texture->imageData = (GLubyte *)malloc(tga.imageSize);
	if(texture->imageData == NULL)			// 确认已经分配成功
	{
		/*...Error code here...*/
		return false;				// 确认已经分配成功
	}
	// 尝试读取所有图像数据
	if(fread(texture->imageData, 1, tga.imageSize, fTGA) != tga.imageSize)
	{
		/*...Error code here...*/
		return false;				// 如果不能，返回false
	}
	//  开始循环
	for(GLuint cswap = 0; cswap < (int)tga.imageSize; cswap += tga.bytesPerPixel)
	{
		// 第一字节 XOR第三字节XOR 第一字节 XOR 第三字节
		texture->imageData[cswap] ^= texture->imageData[cswap+2] ^=
		texture->imageData[cswap] ^= texture->imageData[cswap+2];
	}

	fclose(fTGA);					// 关闭文件
	return true;					// 返回成功
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
	// 分配存储图像所需的内存空间
	texture->imageData	= (GLubyte *)malloc(tga.imageSize);
	if(texture->imageData == NULL)			// 如果不能分配内存
	{
		/*...Error code here...*/
		return false;				// 返回 False
	}
	GLuint pixelcount = tga.Height * tga.Width;	// 图像中的像素数
	GLuint currentpixel	= 0;		// 当前正在读取的像素
	GLuint currentbyte	= 0;			// 当前正在向图像中写入的像素
// 一个像素的存储空间
	GLubyte * colorbuffer = (GLubyte *)malloc(tga.bytesPerPixel);
	do						// 开始循环
	{
		GLubyte chunkheader = 0;				// 存储Id块值的变量
		if(fread(&chunkheader, sizeof(GLubyte), 1, fTGA) == 0)	// 尝试读取块的头
		{
			/*...Error code...*/
			return false;				// If It Fails, Return False
		}
		if(chunkheader < 128)				// 如果是RAW块
		{
			chunkheader++;				// 变量值加1以获取RAW像素的总数
			// 开始像素读取循环
			for(short counter = 0; counter < chunkheader; counter++)
			{
				// 尝试读取一个像素
				if(fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) != tga.bytesPerPixel)
				{
					/*...Error code...*/
					return false;			// 如果失败，返回false
				}
				texture->imageData[currentbyte] = colorbuffer[2];		// 写“R”字节
				texture->imageData[currentbyte + 1	] = colorbuffer[1];	//写“G”字节
				texture->imageData[currentbyte + 2	] = colorbuffer[0];	// 写“B”字节
				if(tga.bytesPerPixel == 4)					// 如果是32位图像...
				{
					texture->imageData[currentbyte + 3] = colorbuffer[3];	// 写“A”字节
				}
				else						// 如果是RLE头
				{
					chunkheader -= 127;			//  减去127获得ID Bit的Rid
					// 读取下一个像素
					if(fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) != tga.bytesPerPixel)
					{
						/*...Error code...*/
						return false;				// 如果失败，返回false
					}
						// 开始循环
					for(short counter = 0; counter < chunkheader; counter++)
					{
						// 拷贝“R”字节
						texture->imageData[currentbyte] = colorbuffer[2];
						// 拷贝“G”字节
						texture->imageData[currentbyte + 1	] = colorbuffer[1];
						// 拷贝“B”字节
						texture->imageData[currentbyte + 2	] = colorbuffer[0];
						if(tga.bytesPerPixel == 4)		// 如果是32位图像
						{
							// 拷贝“A”字节
							texture->imageData[currentbyte + 3] = colorbuffer[3];
						}
						currentbyte += tga.bytesPerPixel;	// 增加字节计数器
						currentpixel++;			// 增加字节计数器
					}
				}
				// 依据每像素的字节数增加字节计数器
				currentbyte += tga.bytesPerPixel;
				currentpixel++;					// 像素计数器加1
			}
		}
	}while(currentpixel < pixelcount);	// 是否有更多的像素要读取？开始循环直到最后
		fclose(fTGA);			// 关闭文件
		return true;			// 返回成功
}



