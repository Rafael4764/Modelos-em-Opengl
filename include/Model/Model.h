#pragma once
#include "vector"
#include "Mesh.h"
#include "../vendor/stb_image.h"
#include "Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#define MAX_BONE_INFLUENCE 4
class Model
{
private:
	unsigned int VBO, EBO;
	std::string directory;
	std::vector<Mesh> meshes;
	std::vector<Texture> textures_loaded;
	void loadModel(std::string& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	unsigned int textureFromFile(const char* path, std::string& directory);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
public:
	Model(std::string& path);
	void Draw(Shader& shader);
};

