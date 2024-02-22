#include "Agent.h"

void Agent::InitAgent(int startPos[], int startDir[], float Color[])
{
	//set starting position
	pos[0] = startPos[0];
	pos[1] = startPos[1];

	//set starting direction
	direction[0] = startDir[0];
	direction[1] = startDir[1];

	//set the trail color
	TrailColor[0] = Color[0];
	TrailColor[1] = Color[1];
	TrailColor[2] = Color[2];
	TrailColor[3] = Color[3];

}

void Agent::Move()
{
	
}
