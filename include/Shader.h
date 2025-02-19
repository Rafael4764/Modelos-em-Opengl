#pragma once
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
enum class ShaderType{NONE = -1, VERTEX = 0, FRAGMENT = 1};
struct ShaderProgram
{
	std::string vertexShader;
	std::string fragmentShader;
	ShaderProgram(const char* path)
	{
		std::stringstream ss[2];
		ShaderType type = ShaderType::NONE;
		std::ifstream ifs(path);
		std::string line;
		if(!ifs.is_open())
		{
			std::cout << "erro";
		}
		while(std::getline(ifs, line))
		{
			std::cout << line;
			if(line.find("#shader") != std::string::npos)
			{
				if(line.find("vertex") != std::string::npos)
				{
					type = ShaderType::VERTEX;
				}else if(line.find("fragment") != std::string::npos)
				{
					type = ShaderType::FRAGMENT;
				}
			}
			else {
				ss[(int)type] << line << std::endl;
			}
		}
		vertexShader = ss[0].str();
		fragmentShader = ss[1].str();
	}
};
class Shader
{
public:
	unsigned int id;
	Shader(const char* path);
	void Bind();
	void u3f(const char* path, glm::vec3 vec);
	void u1i(const char* path, int i);
	void um4(const char* path, glm::mat4 mat);
	void u1f(const char* path, float f);
};

