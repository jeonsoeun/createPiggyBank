#pragma once

#include"OBJ.h"
//이미지를 가져와 저장.
void loadImage(string filename, GLubyte*& texel,GLuint& cx, GLuint& cy)
{
	FreeImage_Initialise();
	FIBITMAP* fibmp = NULL;
	FREE_IMAGE_FORMAT fiformat = FIF_UNKNOWN; //초기화(?)
	string l_filename = "../PiggyBankUVTex.tga";
	fiformat = FreeImage_GetFIFFromFilename(l_filename.data());
	//fiformat = FreeImage_GetFIFFromFilename(filename.data());
	
	if (fiformat != FIF_UNKNOWN)
	{
		//로드
		fibmp = FreeImage_Load(fiformat, l_filename.data());
	}
	cx = FreeImage_GetWidth(fibmp);
	cy = FreeImage_GetHeight(fibmp);
	BYTE* a = FreeImage_GetBits(fibmp);
	texel = (GLubyte*) FreeImage_GetBits(fibmp);

	FreeImage_DeInitialise();
}

////로드된 이미지 해제.
void unloadImage(FIBITMAP* pImage)
{
	if (pImage)
	{
		FreeImage_Unload(pImage);
		pImage = NULL;
	}
}

//저장된 이미지을 이용해 텍스쳐 넣기. 이거 사용전에 꼭 텍스쳐가 잇는지 확인하고!
int prepareTexture(Object& object)
{
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, object.TEX);
	glBindTexture(GL_TEXTURE_2D, object.TEX[0]);
	GLuint w=0, h=0;
	GLubyte* texel = 0;
	object.texels = 0;
	
	loadImage(object.imageName, texel, w, h);
	//memset(texel, 255, 2048 * 2048 * 3);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, texel);
	glGenSamplers(1, object.sampler);
	glSamplerParameteri(object.sampler[0], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(object.sampler[0], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(object.sampler[0], GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(object.sampler[0], GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindSampler(0, object.sampler[0]);

	return 0;
}

void checkerboard(Object &object)
{
	GLubyte image[64][64][3];
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			GLubyte c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 255;
			image[i][j][0] = c;
			image[i][j][1] = c;
			image[i][j][2] = c;
		}
	}
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, object.TEX);
	glBindTexture(GL_TEXTURE_2D, object.TEX[0]);
	GLuint w = 64, h = 64;
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenSamplers(1, object.sampler);
	glSamplerParameteri(object.sampler[0], GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glSamplerParameteri(object.sampler[0], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(object.sampler[0], GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(object.sampler[0], GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindSampler(0, object.sampler[0]);

	vector<GLfloat>vt;
	vector<GLuint> vt_index;
	glBindVertexArray(object.VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, object.VBO[1]);
}