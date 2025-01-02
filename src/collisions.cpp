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

        // std::cout << "Object min: " << object_min.x << " " << object_min.y << " " << object_min.z << std::endl;
        // std::cout << "Object max: " << object_max.x << " " << object_max.y << " " << object_max.z << std::endl;

        // std::cout << "x hitbox: " << (player_position.x >= object_min.x) << " " << (player_position.x <= object_max.x) << std::endl; 
        // std::cout << "y hitbox: " << (player_position.y >= object_min.y) << " " << (player_position.y <= object_max.y) << std::endl;
        // std::cout << "z hitbox: " << (player_position.z >= object_min.z) << " " << (player_position.z <= object_max.z) << std::endl;

        if (player_position.x >= object_min.x && player_position.x <= object_max.x &&
            player_position.y >= object_min.y && player_position.y <= object_max.y &&
            player_position.z >= object_min.z && player_position.z <= object_max.z)
        {
            return true;
        }
    }
    return false;
}