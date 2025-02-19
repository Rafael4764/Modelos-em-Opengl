#include "Shader.h"

Shader::Shader(const char* path)
{
	ShaderProgram program(path);
	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	const char* vss = program.vertexShader.c_str();
	glShaderSource(vertex, 1, &vss, nullptr);
	glCompileShader(vertex);
	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fss = program.fragmentShader.c_str();
	glShaderSource(fragment, 1, &fss, nullptr);
	glCompileShader(fragment);
	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);
	glValidateProgram(id);
}

void Shader::Bind()
{
	glUseProgram(id);
}

void Shader::u3f(const char* path, glm::vec3 vec)
{
	glUniform3f(glGetUniformLocation(id, path), vec.x, vec.y, vec.z);
}

void Shader::u1i(const char* path, int i)
{
	glUniform1i(glGetUniformLocation(id, path), i);
}

void Shader::um4(const char* path, glm::mat4 mat)
{
	glUniformMatrix4fv(glGetUniformLocation(id, path), 1, GL_FALSE, &mat[0][0]);
}

void Shader::u1f(const char* path, float f)
{
	glUniform1f(glGetUniformLocation(id, path), f);
}
