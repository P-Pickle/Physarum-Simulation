#include "Settings.h"


Settings::Settings()
{
	//default settings for TrailMap
	width = 1920; //640
	height = 1080; // 360
	AgentCount = 2000000;
	DiffK = 3;
	DecayRate = 0.01f;
	DiffuseRate = 0.1f;

	//default settings for Agents
	movespeed = 1.0f;
	PS = 1;
	SA = DegreeToRad(90.0f);
	SO = 9;
	SW = 1;
	depT = 1.0f;
	RA = DegreeToRad(90.0f);

	SpawnType = "Random";

}

float Settings::DegreeToRad(float Degree)
{
	float Rad = Degree * (M_PI / 180);
	return Rad;
}

