#include "transform.h"
#include "matrices.h"

#include <glad/glad.h>
#include <glm/mat4x4.hpp>

void Transform::UpdateModelMatrix(glm::mat4& model)
{
    model =
        Matrices::Matrix_Translate(this->position.x + this->delta_position.x, this->position.y + this->delta_position.y, this->position.z + this->delta_position.z) *
        Matrices::Matrix_Scale(this->scale.x, this->scale.y, this->scale.z) *
        Matrices::Matrix_Rotate_X(this->rotation.x) *
        Matrices::Matrix_Rotate_Y(this->rotation.y) *
        Matrices::Matrix_Rotate_Z(this->rotation.z) *
        Matrices::Matrix_Translate(this->axis_position.x, this->axis_position.y, this->axis_position.z);
}