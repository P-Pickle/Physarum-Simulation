#include "SimScreen.h"

SimScreen::SimScreen()
{
}

SimScreen::SimScreen(int Wwidth, int Wheight, float TrailColor[], int ACount)
{
	//Initialize SimScreen Variables
	width = Wwidth;
	height = Wheight;

	//Sets the VAO and EBO to the vertices of an object the size of the screen
	InitVertices();

	//generates a texture object and sets it's options to the ones needed to make the sim work
	InitTexture();
	
	//Creates a texture image and sets it to a default color
	CreateImage();

	////Generates texture object to be used as a trail map
	//InitTrailMap();

	////generates the image that is going to be used as the trail map
	//CreateTrailImage();



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
	glGenTextures(1, &texture);

	//bind texture
	glBindTexture(GL_TEXTURE_2D, texture);

	//set texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//set the border color
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}

void SimScreen::CreateImage()
{

	//Create a texture image the size of the window
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	//change the pixels on the texture to the deafult color chosen earlier in the code
	for (int y = 0; y <= height; y++)
	{
		for (int x = 0; x <= width; x++)
		{
			glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, 1, 1, GL_RGBA, GL_FLOAT, defaultcolor);
		}
	}


}

void SimScreen::InitTrailMap()
{
	//Generate texture object
	glGenTextures(1, &TrailMap);

	//bind texture
	glBindTexture(GL_TEXTURE_2D, TrailMap);

	//set texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//set the border color
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);


}

void SimScreen::CreateTrailImage()
{
	float Color[] = { 1.0f,1.0f,1.0f, 0.0f };

	//Create a texture image the size of the window
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	//change the pixels on the texture to the deafult color chosen earlier in the code
	for (int y = 0; y <= height; y++)
	{
		for (int x = 0; x <= width; x++)
		{
			glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, 1, 1, GL_RGBA, GL_FLOAT, Color);
		}
	}

}

void SimScreen::BindArray()
{
	glBindVertexArray(VAO);
}

void SimScreen::BindTex()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, TrailMap);
}

void SimScreen::ChangePixel(int xoffset, int yoffset, float Color[])
{
	glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, 1, 1, GL_RGBA, GL_FLOAT, Color);
}

void SimScreen::ScaleOpacity(float Scalar)
{
	BindTex();
	float ScalarOpacity = Scalar ;
	float OldOpacity[4] = { 0.0f };
	float NewOpacity[4] = { 0.0f };

	//loop through every pixel changing it's opacity
	for (int y = 0; y <= height; y++)
	{
		for (int x = 0; x <= width; x++)
		{
			glReadPixels(x,y,1,1,GL_RGBA,GL_FLOAT,OldOpacity);
			if (OldOpacity[3] != 1.0f && OldOpacity[3] != 0.0f)//if the Old Opacity is within the range of 1.0f and 0.0f
			{
				NewOpacity[0] = OldOpacity[0];
				NewOpacity[1] = OldOpacity[1];
				NewOpacity[2] = OldOpacity[2];
				NewOpacity[3] = OldOpacity[3] + Scalar;//calculate new opacity level


				if (NewOpacity[3] > 1)//if the new Opacity levels exceed 1 clamp it to 1.0f
				{
					NewOpacity[3] = 1.0f;
				}
				else if (NewOpacity[3] < 0)//if the new Opacity levels fall below 0 clamp it to 0.0f
				{
					NewOpacity[3] = 0.0f;
				}

				glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, 1, 1, GL_RGBA, GL_FLOAT, NewOpacity);
			}

		}
	}

}

void SimScreen::PurpleSquare()
{
	float Color[4] = { 1.0f,0.0f,1.0f,1.0f };
	for (int y = 0; y < 100; y++)
	{
		for (int x = 0; x < 100; x++)
		{
			ChangePixel(100 + x, 100 + y, Color);
		}
	}
}
