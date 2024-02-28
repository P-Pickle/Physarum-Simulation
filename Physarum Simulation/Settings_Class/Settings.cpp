#include "Settings.h"

Settings::Settings()
{
	//default settings for TrailMap
	width = 400;
	height = 400;
	AgentCount = 50;
	DiffK = 3;
	DecayRate = 0.1;

	//default settings for Agents
	PS = 1;
	TrailColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	SA = 22.5;
	SO = 9;
	SW = 1;
	depT = 1;

	SpawnType = "Center-Point";

}

