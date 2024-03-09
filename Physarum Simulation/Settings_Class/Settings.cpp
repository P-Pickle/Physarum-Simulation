#include "Settings.h"


Settings::Settings()
{
	//default settings for TrailMap
	width = 640; //640
	height = 360; // 360
	AgentCount = 1000;
	DiffK = 3;
	DecayRate = 0.01f;
	DiffuseRate = 0.1f;

	//default settings for Agents
	movespeed = 1.0f;
	PS = 1;
	SA = M_PI/4;
	SO = 9;
	SW = 1;
	depT = 1.0f;
	RA = M_PI/4;

	SpawnType = "Center-Point";

}

