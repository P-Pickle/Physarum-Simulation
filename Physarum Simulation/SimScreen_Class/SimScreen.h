#pragma once
#include "../Agent_Class/Agent.h"


class SimScreen
{
public:

	SimScreen();
	SimScreen(int Wwidth, int Wheight, float TrailColor[], int ACount);

	//Used when render that object to the screen
	void BindArray();
	void BindTex();

	//Used to alter the pixel data of the texture
	void ChangePixel(int xoffset, int yoffset, float Color[]);
	void ScaleOpacity(float Scalar);//Scales all of the pixels on the screen by a Scalar
	void PurpleSquare();//used for testing


private:

	//Used to Initialize the Object used to show the simulation
	void InitVertices();
	void InitTexture();
	void CreateImage();
	
	//Initializes the trail map to be overlayed the base texture
	void InitTrailMap();
	void CreateTrailImage();

private:

	unsigned int VAO;
	unsigned int EBO;
	
	unsigned int texture;
	unsigned int TrailMap;

	int width;
	int height;

	float defaultcolor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //default color for the texture image
};