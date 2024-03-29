#pragma once
#ifndef Shader_H
#define Shader_H

#include "../includes.h"



class Shader
{
public:
	unsigned int ID; //Shader program ID

	//used to compile the shader program
	Shader();
	Shader(const char* ComputePath);
	Shader(const char* VertexPath, const char* FragPath);
	//used to Set the shader program to be the current one used
	void use();
	//used to set uniforms based on the data type
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec4(const std::string& name, float value[]) const;


private:
};

#endif