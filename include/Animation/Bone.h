#include <vector>
#include "GLMHelper.h"
#include <assimp/scene.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>
struct KeyPosition {
	glm::vec3 position;
	float timeStamp;
};
struct KeyRotation {
	glm::quat orientation;
	float timeStamp;
};
struct KeyScale {
	glm::vec3 scale;
	float timeStamp;
};
class Bone {
private:
	std::string m_name;
	int m_ID;
	glm::mat4 m_LocalTransform;
	int m_NumPosition, m_NumRotation, m_NumScaling;
	std::vector<KeyPosition> m_Positions;
	std::vector<KeyRotation> m_Rotations;
	std::vector<KeyScale> m_Scale;
public:
	Bone(const std::string& name, int ID, const aiNodeAnim* channel);
};