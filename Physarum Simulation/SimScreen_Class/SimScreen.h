#pragma once
#include "../includes.h"
#include "../Shader_Class/Shader.h"


struct Agent
{
	glm::vec2 Pos;
	glm::vec2 Dir;
};

class SimScreen
{
public:

	SimScreen();
	SimScreen(int Wwidth, int Wheight, float TrailColor[], int ACount);

	//Used when render that object to the screen
	void BindArray();
	void BindTexImage();
	void BindTex();
	void BindAgents();

	//Used to alter the pixel data of the texture
	void ChangePixel(int xoffset, int yoffset, float Color[]);
	void SpawnAgents(Shader SpawnProgram, std::string SpawnMode);
	void SpawnAgentsCPU(std::string SpawnMode);
	void GetAgentData();

private:

	//Used to Initialize the Object used to show the simulation
	void InitVertices();
	void InitTexture();
	void CreateWhite();
	void InitAgentBuff();

private:

	unsigned int VAO;
	unsigned int EBO;
	
	unsigned int TrailMap;

	int width;
	int height;

	int AgentCount;
	Agent* Agents;
	unsigned int AgentBuffer;//used to send agent information to the compute shader
};