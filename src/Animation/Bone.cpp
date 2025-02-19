#include "Animation/Bone.h"

Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channel)
{
	this->m_ID = ID;
	this->m_name = name;
	this->m_LocalTransform = 1.0f;
	this->m_NumPosition = channel->mNumPositionKeys;
	this->m_NumRotation = channel->mNumRotationKeys;
	for(int positionIndex = 0; positionIndex < m_NumPosition; positionIndex++)
	{
		aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
		float timeStamp = channel->mPositionKeys[positionIndex].mTime;
		KeyPosition data;
		data.position = GLMHelper::ConvertVector3(aiPosition);
		data.timeStamp = timeStamp;
		m_Positions.push_back(data);
	}
	for(int rotationIndex = 0; rotationIndex < m_NumRotation; rotationIndex++)
	{
		aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
		float timeStamp = channel->mRotationKeys[rotationIndex].mTime;
		KeyRotation data;
		data.orientation = GLMHelper::ConvertQuaternion(aiOrientation);
		data.timeStamp = timeStamp;
	}
	for(int keyIndex = 0; keyIndex < m_NumScaling; keyIndex++)
	{
		aiVector3D aiScale = channel->mScalingKeys[keyIndex].mValue;
		float timeStamp = channel->mScalingKeys[keyIndex].mTime;
		KeyScale data;
		data.scale = GLMHelper::ConvertVector3(aiScale);
		data.timeStamp = timeStamp;
		m_Scale.push_back(data);
	}
}
