#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include <glad/glad.h>
#include <glm/mat4x4.hpp>

class Transform
{
public:

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 delta_position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 axis_position = glm::vec3(0.0f, 0.0f, 0.0f);

    void UpdateModelMatrix(glm::mat4& model);

    void SetPosition(float x, float y, float z);
    void SetScale(float x, float y, float z);
    void SetRotation(float x, float y, float z);
    void SetDeltaPosition(float x, float y, float z);


};

#endif // _TRANSFORM_H_