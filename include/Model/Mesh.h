#pragma once
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "../Shader.h"
#define MAX_BONE_INFLUENCE 4
struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	float m_Weights[MAX_BONE_INFLUENCE];
};
struct Texture {
	unsigned int id;
	std::string path;
	std::string type;
};
class Mesh
{
private:
	unsigned int VBO, EBO;
	void setupMesh();
public:
	unsigned int VAO;
	std::vector<Vertex> vertices;
	std::vector<Texture> textures;
	std::vector<unsigned int> indices;
	Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures, std::vector<unsigned int> indices);
	void Draw(Shader& shader);

};

