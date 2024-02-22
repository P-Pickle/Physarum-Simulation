#pragma once
#include "../includes.h"



class Agent
{
public:

	void InitAgent(int startPos[], int startDir[], float Color[]);

	void Move();

private:

	int direction[2];
	int pos[2];
	float TrailColor[4];
	int SepDistance;
	int ScanDistance;
	int ScanSize;
};