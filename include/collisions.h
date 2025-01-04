#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <iostream>
#include "gameObject.h"
#include "hitbox.h"
#include "player.h"


struct CollisionState {
    bool isXColliding = false;
    bool isYColliding = false;
    bool isZColliding = false;
};
class Collisions {
private:
    // std::unordered_map<GameObject, HitBox> hitboxes;
    std::vector<HitBox> hitboxes;
public:
    void addHitbox(const GameObject& object);
    void removeHitbox(const GameObject& object);
    glm::vec4 checkPlayerCollision(Player& player);
    void clearHitboxes();

    bool checkPointAABBCollision(glm::vec4 point, glm::vec4 min, glm::vec4 max);


};

#endif