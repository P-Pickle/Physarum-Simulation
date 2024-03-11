#include "Settings_Class/Settings.h"
#include "Shader_Class/Shader.h"

struct Agent
{
	glm::vec2 Pos;
	glm::vec2 Dir;
};

bool InitOpenGL(int, int, GLFWwindow*&);
void framebuffer_size_callback(GLFWwindow*, int, int);
void processInput(GLFWwindow*);
unsigned int InitVerts();
unsigned int InitTex(int, int);
unsigned int InitAgentBuff(int, Agent*);
Agent* SpawnAgents(int);


Settings SimSettings;

int main()
{
	int width = SimSettings.width;
	int height = SimSettings.height;
	GLFWwindow* window;
	if (!(InitOpenGL(width, height, window)))
	{
		return -1;
	}

	GLint InvocationLimit[3] = { 0 };
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, (GLuint)0, &InvocationLimit[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, (GLuint)1, &InvocationLimit[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, (GLuint)2, &InvocationLimit[2]);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader DefaultProgram("Shader_Code/shader.vs", "Shader_Code/shader.fs");
	Shader ComputeProgram("Shader_Code/Agent.comp");
	Shader DecayProgram("Shader_Code/Decay.comp");

	Agent* Agents = SpawnAgents(SimSettings.AgentCount);
	unsigned int VAO = InitVerts();
	unsigned int texture = InitTex(width, height);
	unsigned int AgentBuffer = InitAgentBuff(SimSettings.AgentCount, Agents);

	ComputeProgram.use();
	glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

	DecayProgram.use();
	glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

	//int ready;
	//std::cout << "input when ready: ";
	//std::cin >> ready;

	std::chrono::steady_clock::time_point LastTime = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point ThisTime;
	std::chrono::duration<float> deltatime;

	while (!glfwWindowShouldClose(window))//Continues to render image on screen until window is closed
	{
		//Input
		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Render
		ThisTime = std::chrono::steady_clock::now();
		deltatime = std::chrono::duration_cast<std::chrono::duration<float>>(ThisTime - LastTime);
		LastTime = ThisTime;



		//bind Compute Program
		ComputeProgram.use();

		//set uniforms and bind buffer to compute shader
		//overall Simulation settings
		ComputeProgram.setInt("AgentCount", SimSettings.AgentCount);
		ComputeProgram.setInt("width", width);
		ComputeProgram.setInt("height", height);
		ComputeProgram.setVec4("TrailColor", SimSettings.TrailColor);
		ComputeProgram.setFloat("deltaTime", deltatime.count() * 30);

		//Agent Movement settings
		ComputeProgram.setFloat("movespeed", SimSettings.movespeed);
		ComputeProgram.setFloat("TrailDeposit", SimSettings.depT);
		ComputeProgram.setFloat("RotationAngle", SimSettings.RA);

		//Sensor Settings
		ComputeProgram.setFloat("SensorOffsetAngle", SimSettings.SA);
		ComputeProgram.setInt("SensorOffsetDistance", SimSettings.SO);
		ComputeProgram.setInt("SensorWidth", SimSettings.SW);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, AgentBuffer);

		//Dispatch invocations 
		glDispatchCompute(SimSettings.AgentCount/10, 1, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		//bind Decay Program
		DecayProgram.use();

		//Simulation screen parameters
		DecayProgram.setInt("width", width);
		DecayProgram.setInt("height", height);
		DecayProgram.setVec4("TrailColor", SimSettings.TrailColor);
		DecayProgram.setVec4("EndColor", SimSettings.EndColor);//remove when running primary decay shader
		DecayProgram.setFloat("deltaTime", deltatime.count() * 30);

		//decay and diffusion settings
		DecayProgram.setFloat("DecayRate", SimSettings.DecayRate);
		DecayProgram.setFloat("DiffuseRate", SimSettings.DiffuseRate);
		DecayProgram.setInt("KernelSize", SimSettings.DiffK);

		//Dispatch inovcations
		glDispatchCompute(SimSettings.width, SimSettings.height, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		DefaultProgram.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//check and call events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();//cleans out all of the resources we allocated for glfw
	return 0;

}

bool InitOpenGL(int width, int height, GLFWwindow*& window)
{
	glfwInit();

	//Sets the options for glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "Simulation Window", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed To Create Window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);//sets the current window to the newly created window

	//Initializes GLAD to the current glfw proccess
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	//sets the rendering view port to a given dimension first 2 parameters set inital pixel to render the last 2 set the size of the rendering viewport
	//(You can set the size to be smaller than the window if you wanted to render other things on the screen such as a UI)
	glViewport(0, 0, width, height);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//makes it so when the window is resized so is the viewport
	return true;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//if escape key is pressed close the window
		glfwSetWindowShouldClose(window, true);
}

unsigned int InitVerts()
{
	unsigned int VAO;
	unsigned int EBO;


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

	return VAO;
}

unsigned int InitTex(int width, int height)
{
	unsigned int texture;
	//Generate texture object
	glGenTextures(1, &texture);

	//bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	//set texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	// Generate texture data (fill with white color)
	float* textureData = new float[width * height * 4]; // 4 components (RGBA)
	for (int i = 0; i < width * height * 4; i += 4) {
		textureData[i] = SimSettings.TrailColor[0];   // Red
		textureData[i + 1] = SimSettings.TrailColor[1]; // Green
		textureData[i + 2] = SimSettings.TrailColor[1]; // Blue
		textureData[i + 3] = 0.0f; // Alpha
	}

	// Load texture data to GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, textureData);

	delete[] textureData;

	//Create a texture image the size of the window
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 800, 600, 0, GL_RGBA, GL_FLOAT, NULL);

	return texture;
}

unsigned int InitAgentBuff(int AgentCount, Agent* Agents)
{
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, AgentCount * sizeof(Agent), Agents, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, buffer);
	//glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	return buffer;
}

Agent* SpawnAgents(int AgentCount)
{
	srand(time(NULL));
	Agent* Agents = new Agent[AgentCount];
	glm::vec2 center = glm::vec2(SimSettings.width / 2, SimSettings.height / 2);
	if (SimSettings.SpawnType == "Center-Point")//Spawn agents in the center of the map with a random angle
	{
		for (int x = 0; x < AgentCount; x++)
		{
			Agents[x].Pos = center;
			float angle = rand() % 360;
			Agents[x].Dir = glm::vec2(cos(angle), sin(angle));
		}

	}
	else if (SimSettings.SpawnType == "Random")
	{
		for (int x = 0; x < AgentCount; x++)
		{
			Agents[x].Pos.x = rand() % (SimSettings.width - 1);
			Agents[x].Pos.y = rand() % (SimSettings.height - 1);
			float angle = rand() % 360;
			Agents[x].Dir = glm::vec2(cos(angle), sin(angle));
		}
	}

	return Agents;
}
