#include "Renderer.h"

Renderer::Renderer(float TrailColor[])
{
	//Intialize shader programs
	ShaderProgram =  Shader("Shader_Code/shader.vs", "Shader_Code/shader.fs");
	AgentComputeProgram = Shader("Shader_Code/TestCompute.compute");
	OpacityComputeProgram = Shader("Shader_Code/OpacityPass.compute");
	SpawnProgram = Shader("Shader_Code/AgentSpawn.compute");

	//Initialize TrailMap
	Screen = SimScreen(SimSettings.width, SimSettings.height, TrailColor, SimSettings.AgentCount);

	Screen.SpawnAgentsCPU(SimSettings.SpawnType);


}

void Renderer::Render()
{

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//update agents
	Update();

	//bind objects to be rendered
	ShaderProgram.use();
	Screen.BindArray();
	ShaderProgram.setInt("TrailMap", 0);
	Screen.BindTex();

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);


	//run over every pixel in the TrailMap and decrease it's opacity by the diffusion rate
	//OpacityComputeProgram.use();
	//glDispatchCompute(SimSettings.width, SimSettings.height, 1);
	//glMemoryBarrier(GL_ALL_BARRIER_BITS);
}

void Renderer::Update()
{

	//Use Shader program
	AgentComputeProgram.use();
	Screen.BindTexImage();

	//bind Storage buffer
	//Screen.BindAgents();

	//send needed variable information to the shader
	//AgentComputeProgram.setInt("width", SimSettings.width);
	//AgentComputeProgram.setInt("height", SimSettings.height);
	//AgentComputeProgram.setInt("AgentCount", SimSettings.AgentCount);

	//dispatch 
	glDispatchCompute((unsigned int)SimSettings.width, (unsigned int)SimSettings.height, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	//retreive data
	//Screen.GetAgentData();



}

void Renderer::DispertionPass()
{

	//run over every pixel in the TrailMap and decrease it's opacity by the diffusion rate
	OpacityComputeProgram.use();
	glDispatchCompute(SimSettings.width, SimSettings.height, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
}

