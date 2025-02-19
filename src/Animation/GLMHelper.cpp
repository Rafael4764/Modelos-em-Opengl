#include "Animation/GLMHelper.h"

glm::mat4 GLMHelper::ConverMatrix4(aiMatrix4x4& from)
{
    glm::mat4 to;
    to[0][0] = from[0][0]; to[0][1] = from[0][1]; to[0][2] = from[0][2]; to[0][3] = from[0][3];
    to[1][0] = from[1][0]; to[1][1] = from[1][1]; to[1][2] = from[1][2]; to[1][3] = from[1][3];
    to[2][0] = from[2][0]; to[2][1] = from[2][1]; to[2][2] = from[2][2]; to[2][3] = from[2][3];
    to[3][0] = from[3][0]; to[3][1] = from[3][1]; to[3][2] = from[3][2]; to[3][3] = from[3][3];
    return to;
}

glm::vec3 GLMHelper::ConvertVector3(aiVector3D& vec)
{
    return glm::vec3(vec.x, vec.y, vec.z);
}

glm::quat GLMHelper::ConvertQuaternion(aiQuaternion& quat)
{
    return glm::quat(quat.w, quat.x, quat.y, quat.z);
}
