#include "Shader_Class/Shader.h"
#include "Renderer_Class/Renderer.h"

void framebuffer_size_callback(GLFWwindow*, int, int);
void processInput(GLFWwindow*);
bool InitOpenGL(int, int, GLFWwindow*&);

int main()
{
	GLFWwindow* window;
	if (!(InitOpenGL(800, 600, window)))
	{
		return -1;
	}
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float TrailColor[4] = { 1.0f };

	Renderer RendScreen(TrailColor);


	while (!glfwWindowShouldClose(window))//Continues to render image on screen until window is closed
	{
		//Input
		processInput(window);

		//Render


		RendScreen.Render();


		//check and call events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		//RendScreen.OpacityPass(-1.0f);
	}

	glfwTerminate();//cleans out all of the resources we allocated for glfw
	return 0;
}

bool InitOpenGL(int width, int height, GLFWwindow*& window)
{
	glfwInit();

	//Sets the options for glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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




