#pragma once
#include "EngineHead.h"

#define MAX_TEXTURE_NAMES		50              //纹理数量
#define MAX_TEXTURE_FILES		10

GLuint	 TextureName[MAX_TEXTURE_NAMES];							// 3种纹理的储存空间

/* Structures ****************************************************************************************************/
typedef struct
{
	char			filename[ 100 ];
	bool			is_transparent;
	long			filter_type;
	long			wrap_type;
} TEXTURE_FILE;

/* Texture Class *************************************************************************************************/

class Texture
{
	public:
		Texture();
		~Texture();

		BITMAPFILEHEADER		file_header;
		BITMAPINFOHEADER	info_header;		
		unsigned char		*data;

		bool Load(char *filename);
		void Release();
};


/* Constructor ***************************************************************************************************/
Texture::Texture()
{
	data = NULL;
	memset (&file_header, 0, sizeof(file_header));
	memset (&info_header, 0, sizeof(info_header));
}


/* Destructor ****************************************************************************************************/
Texture::~Texture(){}

/* Load **********************************************************************************************************/
bool Texture::Load(char *filename)
{
	FILE			*fp;
	long			image_size;	
	Release();
	fp = fopen("../texture/fild02/jt_campf_002.bmp", "rb");
	if (fp == NULL) return (false);				
				
		fread (&file_header, sizeof(BITMAPFILEHEADER), 1, fp);
		fread (&info_header,sizeof(BITMAPINFOHEADER), 1, fp);
		if (info_header.biBitCount != 24)
		{		
			memset (&file_header, 0, sizeof(BITMAPFILEHEADER));
			memset (&info_header, 0, sizeof(BITMAPINFOHEADER));
			fclose (fp);
			return (false);
		}

		image_size	= info_header.biWidth * info_header.biHeight * 3;
		data		= new unsigned char[ image_size+1 ];
		fread (data, image_size, 1, fp);		
		
	fclose (fp);	
	return (true);
}


/* Release *******************************************************************************************************/
void Texture::Release()
{
	if (data != NULL)
	{
		delete [] data;
		data = NULL;
		memset (&file_header, 0, sizeof(file_header));
		memset (&info_header, 0, sizeof(info_header));
	}
}

//Orther Functions*****************************************************************/

/* LoadTexture *******************************************************************************************************/
void LoadTexture(char *filename, long texture_id, long mag_filter, long min_filter, long wrap_type)
{
	Texture texture;
	if (!texture.Load (filename)) return;
		glBindTexture (GL_TEXTURE_2D, TextureName[ texture_id ]);
		glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, texture.info_header.biWidth, texture.info_header.biHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, texture.data);
		gluBuild2DMipmaps (GL_TEXTURE_2D, GL_RGB, texture.info_header.biWidth, texture.info_header.biHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, texture.data);

		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_type);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_type);
		glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	texture.Release();
}


/* LoadTransparentTexture ********************************************************************************************/
void LoadTransparentTexture(char *filename, long texture_id, long mag_filter, long min_filter, long wrap_type, unsigned char trans_r, unsigned char trans_g, unsigned char trans_b)
{
	Texture			texture;
	unsigned char	*alpha_texture=NULL;
	long			texel;

	if (!texture.Load (filename)) return;

		alpha_texture = new unsigned char[ texture.info_header.biWidth*texture.info_header.biHeight*4+1 ];
		for (texel = 0; texel < texture.info_header.biWidth*texture.info_header.biHeight; texel++)
		{
			alpha_texture[ texel*4 ] = texture.data[ texel*3 ];
			alpha_texture[ texel*4+1]= texture.data[ texel*3+1 ];
			alpha_texture[ texel*4+2]= texture.data[ texel*3+2 ];

			if (texture.data[ texel*3 ] == trans_b &&
				texture.data[ texel*3+1]== trans_g &&
				texture.data[ texel*3+2]== trans_r) alpha_texture[ texel*4+3] = 0;
			else alpha_texture[ texel*4+3] = 255;
		}

		glBindTexture (GL_TEXTURE_2D, TextureName[ texture_id ]);
		glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, texture.info_header.biWidth, texture.info_header.biHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, alpha_texture);
		gluBuild2DMipmaps (GL_TEXTURE_2D, GL_RGBA, texture.info_header.biWidth, texture.info_header.biHeight, GL_BGRA_EXT, GL_UNSIGNED_BYTE, alpha_texture);

		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_type);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_type);
		glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		delete [] alpha_texture;
		alpha_texture = NULL;
	texture.Release();
}

AUX_RGBImageRec *LoadBMP(char *Filename)
{
	if(!Filename)
		return NULL;
	FILE *fp = NULL;
	fp = fopen(Filename,"r");
	if (fp)										// 文件存在么?
	{
		fclose(fp);							// 关闭句柄
		return auxDIBImageLoad(Filename);				// 载入位图并返回指针
	}
	return NULL;
	//AUX_RGBImageRec *TextureImage[1];						// 创建纹理的存储空间
	//memset(TextureImage,0,sizeof(void *)*1);					// 将指针设为 NULL
};

void InitialTexture()
{
	TEXTURE_FILE texture_file[] =
	{
		{"hud.bmp", 	true,	GL_NEAREST, GL_REPEAT},
		{"wall.bmp", 	false,	GL_LINEAR,	GL_REPEAT},
		{"floor.bmp", 	false,	GL_LINEAR, 	GL_REPEAT},
		{"ceiling.bmp", false,	GL_LINEAR, 	GL_REPEAT},
		{"sky_fr.bmp", 	false,	GL_LINEAR, 	GL_REPEAT},
		{"sky_bk.bmp", 	false,	GL_LINEAR, 	GL_REPEAT},
		{"sky_lf.bmp", 	false,	GL_LINEAR, 	GL_REPEAT},
		{"sky_rt.bmp", 	false,	GL_LINEAR, 	GL_REPEAT},
		{"sky_up.bmp", 	false,	GL_LINEAR, 	GL_REPEAT},
		{"sky_dn.bmp", 	false,	GL_LINEAR,	GL_REPEAT}
	};
	long tex_id;
	char texture_path[] = "../media/";
	char filename[50];

	glGenTextures (MAX_TEXTURE_NAMES, TextureName);
	for (tex_id = 0; tex_id < MAX_TEXTURE_FILES; tex_id++)
	{
		sprintf (filename,"%s%s", texture_path, texture_file[ tex_id ].filename);

		if (texture_file[ tex_id ].is_transparent) LoadTransparentTexture (filename, tex_id+1, texture_file[ tex_id ].filter_type, texture_file[ tex_id ].filter_type, texture_file[ tex_id ].wrap_type, 0,0,255);
		else LoadTexture (filename, tex_id+1, texture_file[ tex_id ].filter_type, texture_file[ tex_id ].filter_type, texture_file[ tex_id ].wrap_type);
	}
};