#include "Settings.h"

Settings::Settings()
{
	//default settings for TrailMap
	width = 800;
	height = 600;
	AgentCount = 550;
	DiffK = 3;
	DecayRate = 0.1f;

	//default settings for Agents
	movespeed = 50;
	PS = 1;
	SA = 22.5;
	SO = 9;
	SW = 1;
	depT = 1.0f;

	SpawnType = "Center-Point";

}

