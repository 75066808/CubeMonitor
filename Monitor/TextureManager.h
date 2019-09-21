#pragma once
#include "glut.h"
#define TEXUTRE_NUM 128
#define PI 3.1415926535

class TextureManager
{

public:
	TextureManager();
	TextureManager(int num);
	~TextureManager();

	bool setTexture(const char *filename, GLint id); // set the texture in given id
	GLuint getTexture(GLint id); // get the texture in given id

private:
	GLuint *texture; // texture label
};

