#include "transform.h"
#include "matrices.h"


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

void Transform::SetPosition(float x, float y, float z)
{
    this->position.x = x;
    this->position.y = y;
    this->position.z = z;
}

void Transform::SetScale(float x, float y, float z)
{
    this->scale.x = x;
    this->scale.y = y;
    this->scale.z = z;
}

void Transform::SetRotation(float x, float y, float z)
{
    this->rotation.x = x;
    this->rotation.y = y;
    this->rotation.z = z;
}

void Transform::SetDeltaPosition(float x, float y, float z)
{
    this->delta_position.x = x;
    this->delta_position.y = y;
    this->delta_position.z = z;
}
