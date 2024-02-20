#pragma once
#include "../Agent_Class/Agent.h"


class SimScreen
{
public:

	SimScreen(int Wwidth, int Wheight);

	//Used when render that object to the screen
	void BindArray();
	void BindTex();

	//Used to alter the pixel data of the texture
	void ChangePixel(int xoffset, int yoffset, float Color[]);

private:

	//Used to Initialize the Object used to show the simulation
	void InitVertices();
	void InitTexture();
	void CreateImage();

private:

	unsigned int VAO;
	unsigned int EBO;
	
	unsigned int texture;

	int width;
	int height;

	float defaultcolor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; //default color for the texture image
};