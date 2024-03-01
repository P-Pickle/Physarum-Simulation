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
	int DiffK;//Kernel size -> delete
	float DecayRate;//trail decay rate -> done
	float DiffuseRate;
	std::string SpawnType; // done

	//Agent settings
	float movespeed = 100.0f; //done
	int PS;//Pixel Step -> how many steps per frame -> delete
	float TrailColor[4] = {1.0f, 1.0f, 1.0f, 0.0f}; //done
	float SA; //angle between each sensor
	int SO; //sensor offset distance or the distance away from agent
	int SW; //Sensor width
	float depT;//Amount of Trail depositied per step -> done
	


private: 

};



