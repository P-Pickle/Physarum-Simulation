#include "SimScreen.h"

SimScreen::SimScreen()
{
}

SimScreen::SimScreen(int Wwidth, int Wheight, float TrailColor[], int ACount)
{
	//Initialize SimScreen Variables
	width = Wwidth;
	height = Wheight;
	AgentCount = ACount;
	Agents = new Agent[AgentCount];


	//Sets the VAO and EBO to the vertices of an object the size of the screen
	InitVertices();

	//generates a texture object and sets it's options to the ones needed to make the sim work
	InitTexture();

	//Initializes the buffer used to talk to the compute shader
	InitAgentBuff();


}

void SimScreen::InitVertices()
{
	float vertices[] = {
		// positions	  // colors		    // texture coords
		1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
		1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom left
		-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f // top left
	};

	unsigned int indices[] = {
		0,1,2,
		0,2,3
	};

	//Generate and set the VBO, VAO, and EBO
	unsigned int VBO;//Holds the unique ID for a Vertex Buffer Object
	glGenBuffers(1, &VBO);//Generates Vertex Buffer Object used to pass and then store vertex data in the GPU
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//Binds Buffer to a specific Buffer type only one VBO can be bound to a Buffer type at a time
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//copies the vertices from the array to the buffer

	glGenVertexArrays(1, &VAO);//generate vertex array
	glBindVertexArray(VAO);//Bind Vertex Array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);//sets the vertex attribute pointer to be sent to the vertex shader
	glEnableVertexAttribArray(0);//enables the use of the VAO I think it specifially enables use of a particular attrib pointer 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &EBO);//generates ELement buffer object and puts the id into the variable passed in
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);//bind the EBO to the GL_ELEMENT_ARRAY_BUFFER object 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);//copies the indicies into the Element buffers

}

void SimScreen::InitTexture()
{
	//Generate texture object
	glGenTextures(1, &TrailMap);

	//bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TrailMap);

	//set texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	CreateWhite();
	//Create a texture image the size of the window
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

	glBindImageTexture(0, TrailMap, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

	//glBindTexture(GL_TEXTURE_2D, 0);
}

void SimScreen::CreateWhite()
{
	// Generate texture data (fill with white color)
	unsigned char* textureData = new unsigned char[width * height * 4]; // 4 components (RGBA)
	for (int i = 0; i < width * height * 4; i += 4) {
		textureData[i] = 255;   // Red
		textureData[i + 1] = 255; // Green
		textureData[i + 2] = 255; // Blue
		textureData[i + 3] = 255; // Alpha
	}

	// Load texture data to GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);

	delete[] textureData;

}

void SimScreen::InitAgentBuff()
{
	glGenBuffers(1, &AgentBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,AgentBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, AgentCount * sizeof(Agent), Agents, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, AgentBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void SimScreen::SpawnAgentsCPU(std::string SpawnMode)
{
	glm::vec2 center = glm::vec2(width / 2, height / 2);
	if (SpawnMode == "Center-Point")//Spawn agents in the center of the map with a random angle
	{
		for (int x = 0; x < AgentCount; x++)
		{
			srand(time(NULL));
			Agents[x].Pos = center;
			float angle = rand() % 360;
			Agents[x].Dir = glm::vec2(cos(angle), sin(angle));
		}
	}



}

void SimScreen::GetAgentData()
{
	glGetNamedBufferSubData(AgentBuffer, 1, AgentCount * sizeof(Agent), Agents);
}

void SimScreen::BindArray()
{
	glBindVertexArray(VAO);
}

void SimScreen::BindTexImage()
{
	glBindImageTexture(0, TrailMap, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}

void SimScreen::BindTex()
{
	glBindTexture(GL_TEXTURE_2D, TrailMap);
}

void SimScreen::BindAgents()
{
	//glBindBuffer(GL_SHADER_STORAGE_BUFFER, AgentBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, AgentBuffer);
}

void SimScreen::ChangePixel(int xoffset, int yoffset, float Color[])
{
	glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, 1, 1, GL_RGBA, GL_FLOAT, Color);
}

void SimScreen::SpawnAgents(Shader SpawnProgram, std::string SpawnMode)
{
	glDispatchCompute(AgentCount, 1, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
}

