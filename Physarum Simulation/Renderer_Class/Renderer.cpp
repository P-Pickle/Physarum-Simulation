#include "Renderer.h"

Renderer::Renderer(float TrailColor[])
{

	ShaderProgram =  Shader("Shader_Class/shader.vs", "Shader_Class/shader.fs");

	Screen = SimScreen(800, 600, TrailColor, 5);

	Screen.PurpleSquare();

	ShaderProgram.use();
	glUniform1i(glGetUniformLocation(ShaderProgram.ID, "texture1"), 0); // manually
	//ShaderProgram.setInt("texture2", 1);

}

void Renderer::Render()
{

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	ShaderProgram.use();
	Screen.BindArray();
	Screen.BindTex();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

void Renderer::OpacityPass(float Scalar)
{
	Screen.ScaleOpacity(Scalar);
}
