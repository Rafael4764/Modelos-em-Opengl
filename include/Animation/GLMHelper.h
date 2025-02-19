#pragma once
#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>
#include <assimp/vector3.h>
#include <assimp/matrix4x4.h>
#include <assimp/quaternion.h>
class GLMHelper
{
public:
	static glm::mat4 ConverMatrix4(aiMatrix4x4& aiMat4);
	static glm::vec3 ConvertVector3(aiVector3D& vec);
	static glm::quat ConvertQuaternion(aiQuaternion& quat);
};

