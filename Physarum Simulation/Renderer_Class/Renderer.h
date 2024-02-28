#pragma once
#include "../SimScreen_Class/SimScreen.h"
#include "../Shader_Class/Shader.h"
#include "../Settings_Class/Settings.h"

class Renderer
{
public:

	Renderer(float TrailColo[]);

	void Render();

	void Update();

	void DispertionPass();


private:

	SimScreen Screen;
	Shader AgentComputeProgram;
	Shader OpacityComputeProgram;
	Shader ShaderProgram;
	Shader SpawnProgram;
	Settings SimSettings;

};