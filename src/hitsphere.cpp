#include "hitsphere.h"

HitSphere::HitSphere(glm::vec4 center, float radius)
{
    this->center = center;
    this->radius = radius;
}

HitSphere::HitSphere()
{
    this->center = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    this->radius = 0.0f;
}
