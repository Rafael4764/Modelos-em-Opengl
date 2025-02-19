#pragma once
#include <vector>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "Shader.h"
#include "../../vendor/stb_image.h"
#include "Mesh.h"
class Model
{
private:
	std::string directory;
	std::vector<Mesh> meshes;
	std::string directory;
	void loadModel(std::string& path);
	void loadNode(aiNode* node, const aiScene* scene);
	void processMesh(aiMesh* , const aiScene* scene);
	unsigned int textureFromFile(const char* path, std::string& directory);
public:
	Model(std::string& path);
	void Draw(Shader& shader);
};

