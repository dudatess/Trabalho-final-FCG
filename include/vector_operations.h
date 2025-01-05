#ifndef VECTOR_OPERATIONS_H
#define VECTOR_OPERATIONS_H

#include <glm/glm.hpp>
#include <cmath>

class VectorOperations{
    public:

    static glm::vec4 normalize(glm::vec4 vector);
    static glm::vec4 reflect(glm::vec4 vector, glm::vec4 normal);

};

#endif