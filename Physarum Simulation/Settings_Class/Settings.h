#pragma once
#include "../includes.h"

class Settings
{
public:
	Settings();//default values for simulation

public:
	//TrailMap settings
	int width;
	int height;
	int AgentCount;
	int DiffK;//Kernel size
	float DecayRate;//trail decay rate
	std::string SpawnType;
	
	//Agent settings
	int PS;//Pixel Step -> how far agent moves per tick
	glm::vec4 TrailColor;
	float SA; //angle between each sensor
	int SO; //sensor offset distance or the distance away from agent
	int SW; //Sensor width
	float depT;//Amount of Trail depositied per step
	


private: 



};



