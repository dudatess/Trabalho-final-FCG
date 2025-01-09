#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <iostream>
#include "gameObject.h"
#include "hitbox.h"
#include "player.h"
#include "hitsphere.h"


struct CollisionState {
    bool isXColliding = false;
    bool isYColliding = false;
    bool isZColliding = false;
};
class Collisions {
private:
    std::unordered_map<std::string, HitBox> hitboxes;
    std::unordered_map<std::string, HitBox> clickableHitboxes;
    std::vector<HitSphere> hitSpheres;
    
public:
    void addHitbox(const GameObject& object);
    void removeHitbox(const GameObject& object);

    void addHitsphere(const GameObject& object);
    void removeHitsphere(const GameObject& object);

    void addClickableHitbox(const GameObject& object);
    void removeClickableHitbox(const GameObject& object);

    glm::vec4 checkPlayerCollision(Player& player);
    std::unordered_map<std::string, bool> checkClickableCollision(Player& player);
    void clearHitboxes();

    bool checkPointAABBCollision(glm::vec4 point, glm::vec4 min, glm::vec4 max);
    bool checkPointSphereCollision(glm::vec4 point, glm::vec4 sphere_center, float sphere_radius);
    bool checkRayToAABBCollision(glm::vec4 ray_origin, glm::vec4 ray_direction, glm::vec4 min, glm::vec4 max);


};

#endif