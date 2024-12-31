#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <iostream>
#include "gameObject.h"
#include "hitbox.h"
#include "player.h"


class Collisions {
private:
    // std::unordered_map<GameObject, HitBox> hitboxes;
    std::vector<HitBox> hitboxes;
public:
    void addHitbox(const GameObject& object);
    void removeHitbox(const GameObject& object);
    bool checkPlayerCollision(Player& player);
    void clearHitboxes();


};

#endif