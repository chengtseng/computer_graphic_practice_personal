#pragma once
#include<string>
#include<fstream>
#include<iostream>
#include<sstream>
#include"GL/glew.h"


class Shader
{
public:
	GLuint program;//program id

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

	void use();

	Shader();
	~Shader();
};

