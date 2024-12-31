#include "hitbox.h"

HitBox::HitBox(glm::vec4 min_point, glm::vec4 max_point)
{
    this->min_point = min_point;
    this->max_point = max_point;
}

HitBox::HitBox()
{
    this->min_point = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    this->max_point = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}
