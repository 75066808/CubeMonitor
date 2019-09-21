#include "Cube.h"

Cube::Cube()
{

}

Cube::~Cube()
{

}

void Cube::initCube(void)
{
	texture.setTexture("picture/black.jpg", BLACK); // init textures
	texture.setTexture("picture/yellow.jpg", YELLOW); 
	texture.setTexture("picture/white.jpg", WHITE);
	texture.setTexture("picture/green.jpg", GREEN);
	texture.setTexture("picture/blue.jpg", BLUE);
	texture.setTexture("picture/red.jpg", RED);
	texture.setTexture("picture/orange.jpg", ORANGE);
	
	initColor("model/color.txt");

	this->sig = 0;
}

void Cube::initColor(const char* fileName)
{
	FILE* fp;

	int i, j, k, m;
	char buffer[DIR_NUM + 2];

	GLint trans[26];

	trans['K' - 'A'] = BLACK;
	trans['W' - 'A'] = WHITE;
	trans['Y' - 'A'] = YELLOW;
	trans['B' - 'A'] = BLUE;
	trans['G' - 'A'] = GREEN;
	trans['R' - 'A'] = RED;
	trans['O' - 'A'] = ORANGE;

	fp = fopen(fileName, "r");
	for (i = 0; i < ORDER; i++)
	{
		for (j = 0; j < ORDER; j++)
		{
			for (k = 0; k < ORDER; k++)
			{
				fgets(buffer, DIR_NUM + 2, fp);
				for (m = 0; m < DIR_NUM; m++)
				{
					block[i][j][k].surface[m] = trans[buffer[m] - 'A'];
				}

			}
		}
	}

	fclose(fp);
}


void Cube::randomDistribute(void)
{

}

void Cube::startRotate(GLint* index, GLint* direction)
{
	if (this->sig == 1) // rotating
		return;

	this->sig = 1;
	this->index[0] = index[0];
	this->index[1] = index[1];
	this->index[2] = index[2];

	this->direction[0] = direction[0];
	this->direction[1] = direction[1];
	this->direction[2] = direction[2];

	this->angle = 0;
}
void Cube::finishRotate(void)
{
	int i;
	switch (this->direction[0] * 4 + this->direction[1] * 2 + this->direction[2] + 7)
	{
	case 8:
		for (i = 0; i < ORDER; i++)
			if (this->index[i])
				zRotate(i);
		break;
	case 9:
		for (i = 0; i < ORDER; i++)
			if (this->index[i])
				yRotate(i);
		break;
	case 11:
		for (i = 0; i < ORDER; i++)
			if (this->index[i])
				xRotate(i);
		break;

	case 6:
		for (i = 0; i < ORDER; i++)
			if (this->index[i])
				zRotateReverse(i);
		break;
	case 5:
		for (i = 0; i < ORDER; i++)
			if (this->index[i])
				yRotateReverse(i);
		break;
	case 3:
		for (i = 0; i < ORDER; i++)
			if (this->index[i])
				xRotateReverse(i);
		break;

	default:
		break;
	}

	this->sig = 0; // set to initial
}

void Cube::clearAll(void)
{
	int i, j, k, m;

	for (i = 0; i < ORDER; i++)
	{
		for (j = 0; j < ORDER; j++)
		{
			for (k = 0; k < ORDER; k++)
			{
				for (m = 0; m < DIR_NUM; m++)
				{
					block[i][j][k].surface[m] = BLACK;
				}
			}
		}
	}

	for (i = 0; i < ORDER; i++)
	{
		for (j = 0; j < ORDER; j++)
		{
			block[ORDER - 1][i][j].surface[UP] = YELLOW;
			block[0][i][j].surface[DOWN] = WHITE;
			block[i][ORDER - 1][j].surface[FRONT] = RED;
			block[i][0][j].surface[BACK] = ORANGE;
			block[i][j][ORDER - 1].surface[RIGHT] = GREEN;
			block[i][j][0].surface[LEFT] = BLUE;
		}
	}
}

void Cube::xRotate(GLuint index)
{
	int i, j, k;
	Block temp[ORDER][ORDER][ORDER];

	for (i = 0; i < ORDER; i++)
	{
		for (j = 0; j < ORDER; j++)
		{
			for (k = 0; k < ORDER; k++)
			{
				temp[i][j][k] = block[i][j][k];
			}
		}
	}

	for (i = 0; i < ORDER; i++)
	{
		for (j = 0; j < ORDER; j++)
		{
			temp[2 - j][i][index].surface[FRONT] = block[i][j][index].surface[UP];
			temp[2 - j][i][index].surface[BACK] = block[i][j][index].surface[DOWN];
			temp[2 - j][i][index].surface[LEFT] = block[i][j][index].surface[LEFT];
			temp[2 - j][i][index].surface[RIGHT] = block[i][j][index].surface[RIGHT];
			temp[2 - j][i][index].surface[UP] = block[i][j][index].surface[BACK];
			temp[2 - j][i][index].surface[DOWN] = block[i][j][index].surface[FRONT];
		}
	}

	for (i = 0; i < ORDER; i++)
	{
		for (j = 0; j < ORDER; j++)
		{
			for (k = 0; k < ORDER; k++)
			{
				block[i][j][k] = temp[i][j][k];
			}
		}
	}
}

void Cube::xRotateReverse(GLuint index)
{
	int i, j, k;
	Block temp[ORDER][ORDER][ORDER];

	for (i = 0; i < ORDER; i++)
	{
		for (j = 0; j < ORDER; j++)
		{
			for (k = 0; k < ORDER; k++)
			{
				temp[i][j][k] = block[i][j][k];
			}
		}
	}

	for (i = 0; i < ORDER; i++)
	{
		for (j = 0; j < ORDER; j++)
		{
			temp[j][2 - i][index].surface[BACK] = block[i][j][index].surface[UP];
			temp[j][2 - i][index].surface[FRONT] = block[i][j][index].surface[DOWN];
			temp[j][2 - i][index].surface[LEFT] = block[i][j][index].surface[LEFT];
			temp[j][2 - i][index].surface[RIGHT] = block[i][j][index].surface[RIGHT];
			temp[j][2 - i][index].surface[DOWN] = block[i][j][index].surface[BACK];
			temp[j][2 - i][index].surface[UP] = block[i][j][index].surface[FRONT];
		}
	}

	for (i = 0; i < ORDER; i++)
	{
		for (j = 0; j < ORDER; j++)
		{
			for (k = 0; k < ORDER; k++)
			{
				block[i][j][k] = temp[i][j][k];
			}
		}
	}
}

void Cube::yRotate(GLuint index)
{
	int i, j, k;
	Block temp[ORDER][ORDER][ORDER];

	for (i = 0; i < ORDER; i++)
	{
		for (j = 0; j < ORDER; j++)
		{
			for (k = 0; k < ORDER; k++)
			{
				temp[i][j][k] = block[i][j][k];
			}
		}
	}

	for (i = 0; i < ORDER; i++)
	{
		for (j = 0; j < ORDER; j++)
		{
			temp[index][2 - j][i].surface[UP] = block[index][i][j].surface[UP];
			temp[index][2 - j][i].surface[DOWN] = block[index][i][j].surface[DOWN];
			temp[index][2 - j][i].surface[FRONT] = block[index][i][j].surface[LEFT];
			temp[index][2 - j][i].surface[BACK] = block[index][i][j].surface[RIGHT];
			temp[index][2 - j][i].surface[LEFT] = block[index][i][j].surface[BACK];
			temp[index][2 - j][i].surface[RIGHT] = block[index][i][j].surface[FRONT];
		}
	}

	for (i = 0; i < ORDER; i++)
	{
		for (j = 0; j < ORDER; j++)
		{
			for (k = 0; k < ORDER; k++)
			{
				block[i][j][k] = temp[i][j][k];
			}
		}
	}
}

void Cube::yRotateReverse(GLuint index)
{
	int i, j, k;
	Block temp[ORDER][ORDER][ORDER];

	for (i = 0; i < ORDER; i++)
	{
		for (j = 0; j < ORDER; j++)
		{
			for (k = 0; k < ORDER; k++)
			{
				temp[i][j][k] = block[i][j][k];
			}
		}
	}

	for (i = 0; i < ORDER; i++)
	{
		for (j = 0; j < ORDER; j++)
		{
			temp[index][j][2 - i].surface[UP] = block[index][i][j].surface[UP];
			temp[index][j][2 - i].surface[DOWN] = block[index][i][j].surface[DOWN];
			temp[index][j][2 - i].surface[BACK] = block[index][i][j].surface[LEFT];
			temp[index][j][2 - i].surface[FRONT] = block[index][i][j].surface[RIGHT];
			temp[index][j][2 - i].surface[RIGHT] = block[index][i][j].surface[BACK];
			temp[index][j][2 - i].surface[LEFT] = block[index][i][j].surface[FRONT];
		}
	}

	for (i = 0; i < ORDER; i++)
	{
		for (j = 0; j < ORDER; j++)
		{
			for (k = 0; k < ORDER; k++)
			{
				block[i][j][k] = temp[i][j][k];
			}
		}
	}
}

void Cube::zRotate(GLuint index)
{
	int i, j, k;
	Block temp[ORDER][ORDER][ORDER];

	for (i = 0; i < ORDER; i++)
	{
		for (j = 0; j < ORDER; j++)
		{
			for (k = 0; k < ORDER; k++)
			{
				temp[i][j][k] = block[i][j][k];
			}
		}
	}

	for (i = 0; i < ORDER; i++)
	{
		for (j = 0; j < ORDER; j++)
		{
			temp[j][index][2 - i].surface[LEFT] = block[i][index][j].surface[UP];
			temp[j][index][2 - i].surface[RIGHT] = block[i][index][j].surface[DOWN];
			temp[j][index][2 - i].surface[DOWN] = block[i][index][j].surface[LEFT];
			temp[j][index][2 - i].surface[UP] = block[i][index][j].surface[RIGHT];
			temp[j][index][2 - i].surface[BACK] = block[i][index][j].surface[BACK];
			temp[j][index][2 - i].surface[FRONT] = block[i][index][j].surface[FRONT];
		}
	}

	for (i = 0; i < ORDER; i++)
	{
		for (j = 0; j < ORDER; j++)
		{
			for (k = 0; k < ORDER; k++)
			{
				block[i][j][k] = temp[i][j][k];
			}
		}
	}
}

void Cube::zRotateReverse(GLuint index)
{
	int i, j, k;
	Block temp[ORDER][ORDER][ORDER];

	for (i = 0; i < ORDER; i++)
	{
		for (j = 0; j < ORDER; j++)
		{
			for (k = 0; k < ORDER; k++)
			{
				temp[i][j][k] = block[i][j][k];
			}
		}
	}

	for (i = 0; i < ORDER; i++)
	{
		for (j = 0; j < ORDER; j++)
		{
			temp[2 - j][index][i].surface[RIGHT] = block[i][index][j].surface[UP];
			temp[2 - j][index][i].surface[LEFT] = block[i][index][j].surface[DOWN];
			temp[2 - j][index][i].surface[UP] = block[i][index][j].surface[LEFT];
			temp[2 - j][index][i].surface[DOWN] = block[i][index][j].surface[RIGHT];
			temp[2 - j][index][i].surface[BACK] = block[i][index][j].surface[BACK];
			temp[2 - j][index][i].surface[FRONT] = block[i][index][j].surface[FRONT];
		}
	}

	for (i = 0; i < ORDER; i++)
	{
		for (j = 0; j < ORDER; j++)
		{
			for (k = 0; k < ORDER; k++)
			{
				block[i][j][k] = temp[i][j][k];
			}
		}
	}
}

void Cube::drawCube(void)
{
	int i, j, k;

	glPushMatrix();
	glTranslatef(0, 0, -10);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);


	for (i = 0; i < ORDER; i++)
	{
		for (j = 0; j < ORDER; j++)
		{
			for (k = 0; k < ORDER; k++)
			{
				glPushMatrix();
				if (sig == 1)
				{				
					switch (!!direction[0] * 4 + !!direction[1] * 2 + !!direction[2])
					{
					case 1:
						if (index[j])
							glRotatef(angle, direction[0], direction[1], direction[2]);
						break;
					case 2:
						if (index[i])
							glRotatef(angle, direction[0], direction[1], direction[2]);
						break;
					case 4:
						if (index[k])
							glRotatef(angle, direction[0], direction[1], direction[2]);
						break;
					default:
						break;
					}

					angle += SPEED;
				}
				
				glTranslatef(-HSIDE_LEN * 2 + k * HSIDE_LEN * 2, -HSIDE_LEN * 2 + i * HSIDE_LEN * 2, -HSIDE_LEN * 2 + j * HSIDE_LEN * 2);
				glEnable(GL_TEXTURE_2D); // texture mapping

				glBindTexture(GL_TEXTURE_2D, texture.getTexture(block[i][j][k].surface[FRONT]));
				glBegin(GL_QUADS); // front
				glNormal3f(0.0f, 0.0f, 1.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-HSIDE_LEN, -HSIDE_LEN, +HSIDE_LEN);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(+HSIDE_LEN, -HSIDE_LEN, +HSIDE_LEN);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(+HSIDE_LEN, +HSIDE_LEN, +HSIDE_LEN);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-HSIDE_LEN, +HSIDE_LEN, +HSIDE_LEN);
				glEnd();

				glBindTexture(GL_TEXTURE_2D, texture.getTexture(block[i][j][k].surface[BACK]));
				glBegin(GL_QUADS); // back
				glNormal3f(0.0f, 0.0f, -1.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(+HSIDE_LEN, -HSIDE_LEN, -HSIDE_LEN);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(-HSIDE_LEN, -HSIDE_LEN, -HSIDE_LEN);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(-HSIDE_LEN, +HSIDE_LEN, -HSIDE_LEN);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(+HSIDE_LEN, +HSIDE_LEN, -HSIDE_LEN);
				glEnd();

				glBindTexture(GL_TEXTURE_2D, texture.getTexture(block[i][j][k].surface[LEFT]));
				glBegin(GL_QUADS);  // left
				glNormal3f(-1.0f, 0.0f, 0.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-HSIDE_LEN, -HSIDE_LEN, -HSIDE_LEN);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(-HSIDE_LEN, -HSIDE_LEN, +HSIDE_LEN);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(-HSIDE_LEN, +HSIDE_LEN, +HSIDE_LEN);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-HSIDE_LEN, +HSIDE_LEN, -HSIDE_LEN);
				glEnd();

				glBindTexture(GL_TEXTURE_2D, texture.getTexture(block[i][j][k].surface[RIGHT]));
				glBegin(GL_QUADS); // right
				glNormal3f(1.0f, 0.0f, 0.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(+HSIDE_LEN, -HSIDE_LEN, +HSIDE_LEN);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(+HSIDE_LEN, -HSIDE_LEN, -HSIDE_LEN);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(+HSIDE_LEN, +HSIDE_LEN, -HSIDE_LEN);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(+HSIDE_LEN, +HSIDE_LEN, +HSIDE_LEN);
				glEnd();

				glBindTexture(GL_TEXTURE_2D, texture.getTexture(block[i][j][k].surface[DOWN]));
				glBegin(GL_QUADS);  // down
				glNormal3f(0.0f, -1.0f, 0.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-HSIDE_LEN, -HSIDE_LEN, -HSIDE_LEN);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(+HSIDE_LEN, -HSIDE_LEN, -HSIDE_LEN);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(+HSIDE_LEN, -HSIDE_LEN, +HSIDE_LEN);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-HSIDE_LEN, -HSIDE_LEN, +HSIDE_LEN);
				glEnd();

				glBindTexture(GL_TEXTURE_2D, texture.getTexture(block[i][j][k].surface[UP]));
				glBegin(GL_QUADS); // up
				glNormal3f(0.0f, 1.0f, 0.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-HSIDE_LEN, +HSIDE_LEN, +HSIDE_LEN);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(+HSIDE_LEN, +HSIDE_LEN, +HSIDE_LEN);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(+HSIDE_LEN, +HSIDE_LEN, -HSIDE_LEN);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-HSIDE_LEN, +HSIDE_LEN, -HSIDE_LEN);
				glEnd();
				glDisable(GL_TEXTURE_2D);

				glPopMatrix();
			}
		}
	}

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	if (sig  == 1 && angle > 90) // finished rotating
		finishRotate();
}

Block Cube::getBlock(GLuint layer, GLuint row, GLuint col)
{
	return this->block[layer][row][col];
}

GLint Cube::getState(void)
{
	return this->sig;
}