#include "collisions.h"
#include "gameObject.h"
#include "hitbox.h"

void Collisions::addHitbox(const GameObject& object)
{
    HitBox hitbox = object.getHitbox();
    hitboxes.push_back(hitbox);
}

void Collisions::removeHitbox(const GameObject& object)
{

}

void Collisions::clearHitboxes()
{
    hitboxes.clear();
}

bool Collisions::checkPlayerCollision(Player& player)
{
    glm::vec4 player_position = player.getPosition();
    for (HitBox hitbox : hitboxes)
    {
        glm::vec4 object_min = hitbox.getMinPoint();
        glm::vec4 object_max = hitbox.getMaxPoint();

        if (player_position.x >= object_min.x && player_position.x <= object_max.x &&
            player_position.y >= object_min.y && player_position.y <= object_max.y &&
            player_position.z >= object_min.z && player_position.z <= object_max.z)
        {
            return true;
        }
    }
    return false;
}