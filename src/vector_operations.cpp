#include "vector_operations.h"

glm::vec4 VectorOperations::normalize(glm::vec4 vector)
{
    float norm = sqrt(vector.x*vector.x + vector.y*vector.y + vector.z*vector.z);
    return glm::vec4(vector.x/norm, vector.y/norm, vector.z/norm, 0.0f);
}

glm::vec4 VectorOperations::reflect(glm::vec4 vector, glm::vec4 normal)
{
    return vector - 2.0f * glm::dot(vector, normal) * normal;
}
