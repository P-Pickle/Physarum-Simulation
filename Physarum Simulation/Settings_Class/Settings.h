#pragma once
#include "../includes.h"

class Settings
{
public:
	Settings();//default values for simulation

public:
	//TrailMap settings
	int width; //done
	int height; //done
	int AgentCount; //done
	int DiffK;//Kernel size
	float DecayRate;//trail decay rate - done
	std::string SpawnType; // done

	//Agent settings
	float movespeed = 100.0f; //done
	int PS;//Pixel Step -> how far agent moves per tick
	float TrailColor[4] = {0.0f, 0.75f, 1.0f, 0.0f}; //done
	float SA; //angle between each sensor
	int SO; //sensor offset distance or the distance away from agent
	int SW; //Sensor width
	float depT;//Amount of Trail depositied per step - done
	


private: 

};



