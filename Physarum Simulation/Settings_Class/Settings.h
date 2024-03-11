#pragma once
#include "../includes.h"

class Settings
{
public:
	Settings();//default values for simulation

private:
	float DegreeToRad(float Degree);

public:
	//TrailMap settings
	int width; //done
	int height; //done
	int AgentCount; //done
	int DiffK;//Kernel size -> done
	float DecayRate;//trail decay rate -> done
	float DiffuseRate;// done
	std::string SpawnType; // done

	//Agent settings
	float movespeed; //done
	int PS;//Pixel Step -> how many steps per frame -> 
	float TrailColor[4] = {1.0f, 1.0f, 1.0f, 0.0f}; //done
	float EndColor[4] = { 0.0f, 0.75f, 1.0f, 0.0f };
	float SA; //angle between each sensor
	int SO; //sensor offset distance or the distance away from agent
	int SW; //Sensor width
	float depT;//Amount of Trail depositied per step -> done
	float RA; //Max Rotation an Agent can make
	


private: 

};



