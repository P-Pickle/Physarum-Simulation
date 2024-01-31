#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
" FragColor = vec4(0.5, 1.0, 0.5, 1.0);\n"
"}\0";


void framebuffer_size_callback(GLFWwindow*, int, int);
void processInput(GLFWwindow*);

int main()
{
	glfwInit();

	//Sets the options for glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Creates window with given size 800x600 and name LearnOpenGL
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed To Create Window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);//sets the current window to the newly created window

	//Initializes GLAD to the current glfw proccess
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//sets the rendering view port to a given dimension first 2 parameters set inital pixel to render the last 2 set the size of the rendering viewport
	//(You can set the size to be smaller than the window if you wanted to render other things on the screen such as a UI)
	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//makes it so when the window is resized so is the viewport

	//compile the vertex shader
	unsigned int vertexShader;//holds the Shader id
	vertexShader = glCreateShader(GL_VERTEX_SHADER);//creates the Vertex Shader object the function takes what kind of shader we are making
	glShaderSource(vertexShader/*where to bind*/, 1/*number of strings*/, &vertexShaderSource/*string to compile*/, NULL);//bind the shader source code to the shader object
	glCompileShader(vertexShader);//compile the code for the shader

	//test if compile was a success
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//compile the fragment shader
	unsigned int fragshader;
	fragshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragshader, 1, &fragShaderSource, NULL);
	glCompileShader(fragshader);

	glGetShaderiv(fragshader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragshader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int ShaderProgram;
	ShaderProgram = glCreateProgram();//create a shader program

	glAttachShader(ShaderProgram, vertexShader);//link the vertex shader to the program
	glAttachShader(ShaderProgram, fragshader);//link the fragment shader to the program
	glLinkProgram(ShaderProgram);//link the program to the project

	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);//check if linking is successful
	if (!success) {
		glGetProgramInfoLog(ShaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	
	//float vertices[] =
	//{
	//	-0.5f,-0.5f,0.0f,
	//	0.5f,-0.5f,0.0f,
	//	0.0f,0.5f,0.0f
	//};
	float vertices[] = { // all unique vertices for a rectangle
		0.5f, 0.5f, 0.0f, // top right
		0.5f, -0.5f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f // top left
	};
	
	unsigned int indices[] =
	{
		0,1,3,
		1,2,3
	};

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	while (!glfwWindowShouldClose(window))//Continues to render image on screen until window is closed
	{
		//Input
		processInput(window);

		//Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(ShaderProgram);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//check and call events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}





	glfwTerminate();//cleans out all of the resources we allocated for glfw
	return 0;
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



