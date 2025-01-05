#ifndef SPHERE_H
#define SPHERE_H

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

class HitSphere {
private:
    glm::vec4 center;
    float radius;
public:
    HitSphere(glm::vec4 center, float radius);
    HitSphere();
    glm::vec4 getCenter() { return center; }
    float getRadius() { return radius; }
};

#endif // SPHERE_H