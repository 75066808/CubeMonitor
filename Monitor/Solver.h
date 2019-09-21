#pragma once

#include <windows.h>
#include <stdio.h>
#include "Cube.h"

#define EDGE   0
#define CORNER 1

#define MAX_NUM 20
#define MAX_LEN 100


struct Step {
	GLint index[3];
	GLint direction[3];
};

class Solver
{
public:
	Solver();
	~Solver();

	static void init(void);
	static void solve(Cube &cube);

private:

	static GLboolean parity; // parity check
	static GLbyte positionCode[2][MAX_NUM];     // position code
	static GLbyte orientationCode[2][MAX_NUM];  // orientation code
	static GLint visited[ORDER][ORDER][ORDER];

	static Block cubeCode[ORDER][ORDER][ORDER];
	static Block cubeColor[ORDER][ORDER][ORDER];

	static Step formula[2][26][26][MAX_LEN];
	static Step extraFormula[MAX_LEN];

	static void initFormula(const char* fileName, GLint num);
	static void initExtraFormula(const char* fileName);

	static void initCode(const char* fileName);
	static void initColor(const char* fileName);


	static void turnWholeOrientation(Cube& cube);
	static void observe(Cube& cube, GLint type);
	static void applyFormula(Cube& cube);


	static void parityCheck(GLint type, GLint num);
	static GLint observeBlockPosition(Cube& cube, GLint type);
	static void observeBlockOrientation(Cube& cube, GLint type);

	
	static void mapStep(Step& step, GLbyte symbol);
	static void execStep(Cube& cube, Step& step);
	static GLint compareBlock(Block& block1, Block& block2);

	
};

