#ifndef HITBOX_H
#define HITBOX_H

#include <glm/vec4.hpp>

class HitBox {
private:
    glm::vec4 min_point;
    glm::vec4 max_point;
public:
    HitBox(glm::vec4 min_point, glm::vec4 max_point);
    HitBox();
    glm::vec4 getMinPoint() { return min_point; }
    glm::vec4 getMaxPoint() { return max_point; }
};

#endif // HITBOX_H