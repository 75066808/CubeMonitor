#pragma once

#include "TextureManager.h"
#include "glut.h"
#include <math.h>
#include <stdio.h>

#define UP     0x00
#define DOWN   0x01
#define LEFT   0x02
#define RIGHT  0x03
#define FRONT  0x04
#define BACK   0x05


#define BLACK  0x00
#define YELLOW 0x01
#define RED    0x02
#define BLUE   0x03
#define GREEN  0x04
#define ORANGE 0x05
#define WHITE  0x06

#define ORDER  3
#define HSIDE_LEN 1.0f // half block side length

#define SPEED  0.3f

#define DIR_NUM 6

struct Block {
	GLuint surface[DIR_NUM]; // up, down, front, back, left, right
};


class Cube
{
public:
	Cube();
	~Cube();

	void initCube(void);
	void drawCube(void);

	void randomDistribute(void);
	void startRotate(GLint* index, GLint* direction);

	void clearAll(void);

	Block getBlock(GLuint layer, GLuint row, GLuint col);
	GLint getState(void);

private:
	Block block[ORDER][ORDER][ORDER]; // layer, row, col
	TextureManager texture;

	GLint sig;
	GLfloat angle;
	GLint index[ORDER];
	GLint direction[3];
	
	void initColor(const char* fileName);

	void xRotate(GLuint index);
	void xRotateReverse(GLuint index);
	void yRotate(GLuint index);
	void yRotateReverse(GLuint index);
	void zRotate(GLuint index);
	void zRotateReverse(GLuint index);

	void finishRotate(void);


};

