#include <atlImage.h>
#include "TextureManager.h"

TextureManager::TextureManager()
{
	this->texture = new GLuint[TEXUTRE_NUM];
}

TextureManager::TextureManager(int num)
{
	this->texture = new GLuint[num];
}

TextureManager::~TextureManager()
{
	delete this->texture;
}

bool TextureManager::setTexture(const char *fileName, GLint id) // set the texture in given id
{

	if (!PathFileExists(fileName))   // can't find the file
		return false;

	CImage image;
	HRESULT result;
	COLORREF color;
	int width, height;
	unsigned char* imageData;

	result = image.Load(fileName);  // load the image file

	if (result != S_OK)  // load failed
		return false;

	width = image.GetWidth();
	height = image.GetHeight();
	imageData = (unsigned char*)malloc(width * height * 3 * sizeof(unsigned char)); // allocate the RGB data

	if (imageData == NULL) // can't get the memory
		return false;

	for (int i = 0;i < height;i++) // get RGB data
	{
		for (int j = 0;j < width;j++)
		{
			color = image.GetPixel(j, i);
			imageData[(height - 1 - i) * width * 3 + j * 3] = GetRValue(color);
			imageData[(height - 1 - i) * width * 3 + j * 3 + 1] = GetGValue(color);
			imageData[(height - 1 - i) * width * 3 + j * 3 + 2] = GetBValue(color);
		}
	}

	if(!glIsTexture(texture[id])) // if not already exist, generate a new texture
		glGenTextures(1, this->texture + id);

	glBindTexture(GL_TEXTURE_2D, texture[id]);

	glPixelStorei(GL_TEXTURE_2D, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData); // set data

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // replace mode

	free(imageData); // free data memory
	return true;
}

GLuint TextureManager::getTexture(GLint id) // set the texture in given id
{
	return this->texture[id];
}

