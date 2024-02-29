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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader DefaultProgram("Shader_Code/shader.vs", "Shader_Code/shader.fs");
	Shader ComputeProgram("Shader_Code/Agent.comp");

	Agent* Agents = SpawnAgents(SimSettings.AgentCount);
	unsigned int VAO = InitVerts();
	unsigned int texture = InitTex(width, height);
	unsigned int AgentBuffer = InitAgentBuff(SimSettings.AgentCount, Agents);


	while (!glfwWindowShouldClose(window))//Continues to render image on screen until window is closed
	{
		//Input
		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Render
		ComputeProgram.use();
		ComputeProgram.setInt("width", width);
		ComputeProgram.setInt("height", height);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, AgentBuffer);
		glDispatchCompute(SimSettings.AgentCount, 1, 1);
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

	//Creates window with given size 800x600 and name LearnOpenGL
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
		textureData[i] = 1.0f;   // Red
		textureData[i + 1] = 1.0f; // Green
		textureData[i + 2] = 1.0f; // Blue
		textureData[i + 3] = 1.0f; // Alpha
	}

	// Load texture data to GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

	delete[] textureData;

	//Create a texture image the size of the window
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 800, 600, 0, GL_RGBA, GL_FLOAT, NULL);

	glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);


	return texture;
}

unsigned int InitAgentBuff(int AgentCount, Agent* Agents)
{
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, AgentCount * sizeof(Agent), Agents, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, buffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	return buffer;
}

Agent* SpawnAgents(int AgentCount)
{
	Agent* Agents = new Agent[AgentCount];
	glm::vec2 center = glm::vec2(SimSettings.width / 2, SimSettings.height / 2);
	if (SimSettings.SpawnType == "Center-Point")//Spawn agents in the center of the map with a random angle
	{
		for (int x = 0; x < AgentCount; x++)
		{
			srand(time(NULL));
			Agents[x].Pos = center;
			float angle = rand() % 360;
			Agents[x].Dir = glm::vec2(cos(angle), sin(angle));
		}
	}

	return Agents;
}
