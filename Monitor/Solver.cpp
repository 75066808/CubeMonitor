#include "Solver.h"

Step Solver::formula[2][26][26][MAX_LEN];
Step Solver::extraFormula[MAX_LEN];

GLboolean Solver::parity; // parity check
GLbyte Solver::positionCode[2][MAX_NUM];     // position code
GLbyte Solver::orientationCode[2][MAX_NUM];  // orientation code

Block Solver::cubeCode[ORDER][ORDER][ORDER];
Block Solver::cubeColor[ORDER][ORDER][ORDER];


GLint Solver::visited[ORDER][ORDER][ORDER];

void Solver::init(void)
{
	int i, j, k;

	initFormula("formula/edge.txt", EDGE);    // init formula
	initFormula("formula/corner.txt", CORNER);
	initExtraFormula("formula/extra.txt");

	initCode("model/code.txt");   // init code
	initColor("model/color.txt"); // init color
}

void Solver::solve(Cube& cube)
{
	turnWholeOrientation(cube);  // turn to yellow-up-red-front 
	observe(cube, EDGE);         // memory edge code
	observe(cube, CORNER);       // memory cornere code
	applyFormula(cube);          // execute formula
}


void Solver::initFormula(const char* fileName, GLint num)
{
	FILE* fp;

	int i, j, k, index;
	char buffer[MAX_LEN];

	fp = fopen(fileName, "r");


	while (1)
	{
		if (fgets(buffer, MAX_LEN, fp) == NULL)
		{
			if (feof(fp) != 0) // end of file
				break;
		}

		if (buffer[0] == 0 || buffer[0] == '\n') // empty line
			continue;


		index = -1;
		j = buffer[0] - 'A'; // start code
		k = buffer[1] - 'A'; // end code

		for (i = 2; buffer[i] && buffer[i] != '\n'; i++)
		{

			switch (buffer[i])
			{
			case ' ':
			case '\t':
				break;
			case '\'':
				formula[num][j][k][index].direction[0] = -formula[num][j][k][index].direction[0];
				formula[num][j][k][index].direction[1] = -formula[num][j][k][index].direction[1];
				formula[num][j][k][index].direction[2] = -formula[num][j][k][index].direction[2];
				break;
			case '2':
				index++;
				formula[num][j][k][index] = formula[num][j][k][index - 1];
				break;
			default:
				mapStep(formula[num][j][k][++index], buffer[i]);
				break;
			}
		}

		formula[num][j][k][index + 1].direction[0] = -2; // end signal
	}

	fclose(fp);
}

void Solver::initExtraFormula(const char* fileName)
{
	FILE* fp;

	int i, j, k, index;
	char buffer[MAX_LEN];

	fp = fopen(fileName, "r");

	fgets(buffer, MAX_LEN, fp);

	index = -1;
	for (i = 0; buffer[i] != '\n' && buffer[i] != '\0'; i++)
	{
		switch (buffer[i])
		{
		case ' ':
		case '\t':
			break;
		case '\'':
			extraFormula[index].direction[0] = -extraFormula[index].direction[0];
			extraFormula[index].direction[1] = -extraFormula[index].direction[1];
			extraFormula[index].direction[2] = -extraFormula[index].direction[2];
			break;
		case '2':
			index++;
			extraFormula[index] = extraFormula[index - 1];
			break;
		default:
			mapStep(extraFormula[++index], buffer[i]);
			break;
		}
	}

	extraFormula[index + 1].direction[0] = -2; // end signal


	fclose(fp);
}

void Solver::initCode(const char* fileName)
{
	FILE* fp;

	int i, j, k, m;
	char buffer[MAX_LEN];


	fp = fopen(fileName, "r");
	for (i = 0; i < ORDER; i++)
	{
		for (j = 0; j < ORDER; j++)
		{
			for (k = 0; k < ORDER; k++)
			{
				fgets(buffer, MAX_LEN, fp);
				for (m = 0; m < DIR_NUM; m++)
				{
					cubeCode[i][j][k].surface[m] = buffer[m];
				}
			}
		}
	}

	fclose(fp);
}

void Solver::initColor(const char* fileName)
{
	FILE* fp;

	int i, j, k, m;
	char buffer[MAX_LEN];

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
				fgets(buffer, MAX_LEN, fp);
				for (m = 0; m < DIR_NUM; m++)
				{
					cubeColor[i][j][k].surface[m] = trans[buffer[m] - 'A'];
				}

			}
		}
	}

	fclose(fp);
}

void Solver::turnWholeOrientation(Cube& cube)
{
	int i;
	Step step;

	step.index[0] = 1;
	step.index[1] = 1;
	step.index[2] = 1;

	if (cube.getBlock(0, 1, 1).surface[DOWN] == YELLOW) 
	{
		step.direction[0] = 1;
		step.direction[1] = 0;
		step.direction[2] = 0;
		execStep(cube, step);
		execStep(cube, step);  // x axis rotate 180
	}
	else if (cube.getBlock(1, 0, 1).surface[BACK] == YELLOW)
	{
		step.direction[0] = 1;
		step.direction[1] = 0;
		step.direction[2] = 0;
		execStep(cube, step);  // x axis rotate 90
	}
	else if (cube.getBlock(1, 1, 0).surface[LEFT] == YELLOW)
	{
		step.direction[0] = 0;
		step.direction[1] = 0;
		step.direction[2] = -1;
		execStep(cube, step);  // -z axis rotate 90
	}
	else if (cube.getBlock(1, 1, 2).surface[RIGHT] == YELLOW)
	{
		step.direction[0] = 0;
		step.direction[1] = 0;
		step.direction[2] = 1;
		execStep(cube, step);  // z axis rotate 90
	}
	else if (cube.getBlock(1, 2, 1).surface[FRONT] == YELLOW)
	{
		step.direction[0] = -1;
		step.direction[1] = 0;
		step.direction[2] = 0;
		execStep(cube, step);  // -x axis rotate 90
	}


	if (cube.getBlock(1, 0, 1).surface[BACK] == RED)
	{
		step.direction[0] = 0;
		step.direction[1] = 1;
		step.direction[2] = 0;
		execStep(cube, step); 
		execStep(cube, step); // y axis rotate 180
	}
	else if (cube.getBlock(1, 1, 0).surface[LEFT] == RED)
	{
		step.direction[0] = 0;
		step.direction[1] = 1;
		step.direction[2] = 0;
		execStep(cube, step); // y axis rotate 90
	}
	else if (cube.getBlock(1, 1, 2).surface[RIGHT] == RED)
	{
		step.direction[0] = 0;
		step.direction[1] = -1;
		step.direction[2] = 0;
		execStep(cube, step); // -y axis rotate 90
	}

		
}

void Solver::observe(Cube& cube, GLint type)
{
	GLint number;

	number = observeBlockPosition(cube, type); // position move
	observeBlockOrientation(cube, type);       // orientation inverse

	parityCheck(type, number); // parity check
}

void Solver::applyFormula(Cube& cube)
{
	int i, j, k;
	GLint start, end;


	for (k = 0; k < 2; k++) // position move
	{
		for (i = 0; positionCode[k][i] != -1; i += 2) // each combination
		{
			start = positionCode[k][i] - 'A';
			end = positionCode[k][i + 1] - 'A';

			for (j = 0; formula[k][start][end][j].direction[0] != -2; j++) // each step
			{
				execStep(cube, formula[k][start][end][j]);
			}
		}
	}

	if (parity) // need to do extra parity operation
	{
		for (j = 0; extraFormula[j].direction[0] != -2; j++) // each step
		{
			execStep(cube, extraFormula[j]);
		}
	}

	for (k = 0; k < 2; k++) // orientation inverse
	{
		for (i = 0; orientationCode[k][i] != -1; i += 2) // each combination
		{
			start = orientationCode[k][i] - 'A';
			end = orientationCode[k][i + 1] - 'A';

			for (j = 0; formula[k][start][end][j].direction[0] != -2; j++) // each step
			{
				execStep(cube, formula[k][start][end][j]);
			}
		}
	}

}



GLint Solver::observeBlockPosition(Cube& cube, GLint type)
{
	int a, b, c;
	int i, j, k, m, n;

	GLint index;
	GLboolean parity;
	Block curBlock, tempBlock;
	GLint bufferPos[3], curColor;

	if (type == EDGE)
	{
		bufferPos[0] = 2; // start block
		bufferPos[1] = 2;
		bufferPos[2] = 1;
	}
	else
	{
		bufferPos[0] = 2; // start block
		bufferPos[1] = 2;
		bufferPos[2] = 0;
	}


	for (i = 0; i < ORDER; i++)
	{
		for (j = 0; j < ORDER; j++)
		{
			for (k = 0; k < ORDER; k++)
			{
				visited[i][j][k] = 0;

				curBlock = cubeColor[i][j][k];
				tempBlock = cube.getBlock(i, j, k);
				if (compareBlock(curBlock, tempBlock)) // no need to move
					visited[i][j][k] = 1;

			}
		}
	}

	index = 0;
	visited[bufferPos[0]][bufferPos[1]][bufferPos[2]] = 1;
	curBlock = cube.getBlock(bufferPos[0], bufferPos[1], bufferPos[2]);
	curColor = curBlock.surface[UP]; // start surface


	while (1)
	{
		for (i = 0; i < ORDER; i++)
		{
			for (j = 0; j < ORDER; j++)
			{
				for (k = 0; k < ORDER; k++)
				{
					tempBlock = cubeColor[i][j][k];
					if (compareBlock(curBlock, tempBlock)) // find the block
					{
						if (visited[i][j][k] == 1) // have visited
						{
							for (a = 0; a < ORDER; a++) // find new block
							{
								for (b = 0; b < ORDER; b++)
								{
									for (c = 0; c < ORDER; c++)
									{
										if (type == EDGE)
										{
											if ((a == 0 || a == ORDER - 1) && (b == 0 || b == ORDER - 1) && (c == 0 || c == ORDER - 1)) // corner
												continue;
											if ((a == 1 && b == 1) || (a == 1 && c == 1) || (b == 1 && c == 1)) // center
												continue;
										}
										else
										{
											if ((a == 1 || b == 1 || c == 1)) // edge + center
												continue;
										}

										if (visited[a][b][c] == 0)
										{
											curBlock = cube.getBlock(a, b, c);
											for (m = 0; m < DIR_NUM; m++)
											{
												if (curBlock.surface[m] != BLACK)
												{
													positionCode[type][index++] = cubeCode[a][b][c].surface[m];
													curColor = curBlock.surface[m];
													goto next; // next block
												}
											}
										}
									}
								}
							}
							goto finish; // finished observing
						}
						else // not visited
						{
							curBlock = cube.getBlock(i, j, k);
							for (m = 0; m < DIR_NUM; m++)
							{
								if (tempBlock.surface[m] == curColor)
								{
									positionCode[type][index++] = cubeCode[i][j][k].surface[m];
									curColor = curBlock.surface[m];
									visited[i][j][k] = 1;
									goto next; // next block
								}
							}
						}
					}

				}
			}
		}
	next:;
	}
finish:
	positionCode[type][index] = -1; // end flag
	return index; // whole step number
}

void Solver::observeBlockOrientation(Cube& cube, GLint type)
{
	int i, j, k, m, n;
	GLint index, finishCode;
	Block tempBlock, curBlock;

	GLint bufferPos[3];

	if (type == EDGE)
	{
		bufferPos[0] = 2; // start block
		bufferPos[1] = 2;
		bufferPos[2] = 1;
	}
	else
	{
		bufferPos[0] = 2; // start block
		bufferPos[1] = 2;
		bufferPos[2] = 0;
	}


	for (i = 0; i < ORDER; i++)
	{
		for (j = 0; j < ORDER; j++)
		{
			for (k = 0; k < ORDER; k++)
			{
				if (type == EDGE)
				{
					if ((i == 0 || i == ORDER - 1) && (j == 0 || j == ORDER - 1) && (k == 0 || k == ORDER - 1)) // corner
						continue;

					if ((i == 1 && j == 1) || (i == 1 && k == 1) || (j == 1 && k == 1)) // center
						continue;
				}
				else
				{
					if ((i == 1 || j == 1 || k == 1)) // edge + center
						continue;
				}

				if (i == bufferPos[0] && j == bufferPos[1] && k == bufferPos[2]) // not buffer block
					continue;


				tempBlock = cubeColor[i][j][k];
				curBlock = cube.getBlock(i, j, k);
				if (compareBlock(tempBlock, curBlock) != 1)
				{
					for (m = 0; m < DIR_NUM; m++)
					{
						if (tempBlock.surface[m] != BLACK)
						{
							finishCode = cubeCode[i][j][k].surface[m];
							goto over; // found finish code
						}
					}
				}
			}
		}
	}

over:
	index = 0;
	for (i = 0; i < ORDER; i++)
	{
		for (j = 0; j < ORDER; j++)
		{
			for (k = 0; k < ORDER; k++)
			{
				if (type == EDGE)
				{
					if ((i == 0 || i == ORDER - 1) && (j == 0 || j == ORDER - 1) && (k == 0 || k == ORDER - 1)) // corner
						continue;

					if ((i == 1 && j == 1) || (i == 1 && k == 1) || (j == 1 && k == 1)) // center
						continue;
				}
				else
				{
					if ((i == 1 || j == 1 || k == 1)) // edge + center
						continue;
				}

				if (i == bufferPos[0] && j == bufferPos[1] && k == bufferPos[2]) // not buffer block
					continue;

				tempBlock = cubeColor[i][j][k];
				curBlock = cube.getBlock(i, j, k);
				if (compareBlock(tempBlock, curBlock) == 1)
				{
					for (m = 0; m < DIR_NUM; m++)
					{
						for (n = 0; n < DIR_NUM; n++)
						{
							if (curBlock.surface[m] == tempBlock.surface[n] && curBlock.surface[m] != BLACK)
							{
								orientationCode[type][index++] = cubeCode[i][j][k].surface[m];
								orientationCode[type][index++] = finishCode;
								orientationCode[type][index++] = finishCode;
								orientationCode[type][index++] = cubeCode[i][j][k].surface[n];
								goto cont;
							}
						}
					}
				}
			cont:;
			}
		}
	}

	orientationCode[type][index] = -1; // end flag
}

void Solver::parityCheck(GLint type, GLint num)
{
	if (num % 2 == 1) // parity-check
	{
		if (type == EDGE)
		{
			if (positionCode[type][num - 1] == 'E') // delete 'E'
				num--;
			else if (positionCode[type][num - 1] == 'F') // add 'CCE'
			{
				positionCode[type][num++] = 'C';
				positionCode[type][num++] = 'C';
				positionCode[type][num++] = 'E';

			}
			else
				positionCode[type][num++] = 'E'; // add 'E'
		}
		else
		{
			if (positionCode[type][num - 1] == 'J') // delete 'J'
				num--;
			else if (positionCode[type][num - 1] == 'K' || positionCode[type][num - 1] == 'L') // add 'DDJ'
			{
				positionCode[type][num++] = 'D';
				positionCode[type][num++] = 'D';
				positionCode[type][num++] = 'J';

			}
			else // add 'J'
				positionCode[type][num++] = 'J';
		}
		parity = 1; // need to do extra operation
		positionCode[type][num] = -1; // add end flag
	}
	else
	{
		parity = 0; // no need to do extra operation
		positionCode[type][num] = -1; // add end flag
	}

}

void Solver::mapStep(Step& step, GLbyte symbol)
{
	switch (symbol)
	{
	case 'R':
		step.index[0] = 0;
		step.index[1] = 0;
		step.index[2] = 1;

		step.direction[0] = -1;
		step.direction[1] = 0;
		step.direction[2] = 0;
		break;
	case 'L':
		
		step.index[0] = 1;
		step.index[1] = 0;
		step.index[2] = 0;

		step.direction[0] = 1;
		step.direction[1] = 0;
		step.direction[2] = 0;
		break;
	case 'U':
		
		step.index[0] = 0;
		step.index[1] = 0;
		step.index[2] = 1;

		step.direction[0] = 0;
		step.direction[1] = -1;
		step.direction[2] = 0;
		break;
	case 'D':
		
		step.index[0] = 1;
		step.index[1] = 0;
		step.index[2] = 0;

		step.direction[0] = 0;
		step.direction[1] = 1;
		step.direction[2] = 0;
		break;
	case 'F':
		
		step.index[0] = 0;
		step.index[1] = 0;
		step.index[2] = 1;

		step.direction[0] = 0;
		step.direction[1] = 0;
		step.direction[2] = -1;
		break;
	case 'B':
		
		step.index[0] = 1;
		step.index[1] = 0;
		step.index[2] = 0;

		step.direction[0] = 0;
		step.direction[1] = 0;
		step.direction[2] = 1;
		break;
	case 'M':
	
		step.index[0] = 0;
		step.index[1] = 1;
		step.index[2] = 0;

		step.direction[0] = 1;
		step.direction[1] = 0;
		step.direction[2] = 0;
		break;
	case 'S':
		
		step.index[0] = 0;
		step.index[1] = 1;
		step.index[2] = 0;

		step.direction[0] = 0;
		step.direction[1] = 0;
		step.direction[2] = -1;
		break;
	case 'E':
		
		step.index[0] = 0;
		step.index[1] = 1;
		step.index[2] = 0;

		step.direction[0] = 0;
		step.direction[1] = 1;
		step.direction[2] = 0;
		break;
	case 'u':
		
		step.index[0] = 0;
		step.index[1] = 1;
		step.index[2] = 1;

		step.direction[0] = 0;
		step.direction[1] = -1;
		step.direction[2] = 0;
		break;
	case 'r':
		
		step.index[0] = 0;
		step.index[1] = 1;
		step.index[2] = 1;

		step.direction[0] = -1;
		step.direction[1] = 0;
		step.direction[2] = 0;
		break;
	case 'l':
		
		step.index[0] = 1;
		step.index[1] = 1;
		step.index[2] = 0;

		step.direction[0] = 1;
		step.direction[1] = 0;
		step.direction[2] = 0;
		break;
	case 'f':
		
		step.index[0] = 0;
		step.index[1] = 1;
		step.index[2] = 1;

		step.direction[0] = 0;
		step.direction[1] = 0;
		step.direction[2] = -1;
		break;
	case 'x':
		
		step.index[0] = 1;
		step.index[1] = 1;
		step.index[2] = 1;

		step.direction[0] = -1;
		step.direction[1] = 0;
		step.direction[2] = 0;
		break;
	case 'y':
		
		step.index[0] = 1;
		step.index[1] = 1;
		step.index[2] = 1;

		step.direction[0] = 0;
		step.direction[1] = -1;
		step.direction[2] = 0;
		break;
	case 'z':
		
		step.index[0] = 1;
		step.index[1] = 1;
		step.index[2] = 1;

		step.direction[0] = 0;
		step.direction[1] = 0;
		step.direction[2] = -1;
		break;
	default:
		break;
	}
	
}
void Solver::execStep(Cube& cube, Step& step)
{
	cube.startRotate(step.index, step.direction);
	while (1)
	{
		if (cube.getState() == 0) // not rotating
			break;
	}
}

GLint Solver::compareBlock(Block& block1, Block& block2)
{
	int i;
	GLint vec1[DIR_NUM + 1] , vec2[DIR_NUM + 1];


	for (i = 0; i < DIR_NUM; i++)
	{
		if (block1.surface[i] != block2.surface[i])
			break;
	}

	if (i == DIR_NUM) // all equal
		return 2;

	for (i = 0; i < DIR_NUM + 1; i++)
		vec1[i] = vec2[i] = 0;

	for (i = 0; i < DIR_NUM; i++)
	{
		vec1[block1.surface[i]]++;
		vec2[block2.surface[i]]++;
	}

	for (i = 0; i < DIR_NUM + 1; i++)
		if (vec1[i] != vec2[i])
			return 0; // not equal

	return 1; // number equal

}
