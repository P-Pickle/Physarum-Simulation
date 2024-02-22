#pragma once
#include "../SimScreen_Class/SimScreen.h"
#include "../Shader_Class/Shader.h"

class Renderer
{
public:

	Renderer(float TrailColo[]);

	void Render();
	void OpacityPass(float Scalar);




private:

	SimScreen Screen;
	Shader ShaderProgram; 

	

};