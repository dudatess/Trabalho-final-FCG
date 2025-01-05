#include "collisions.h"
#include "gameObject.h"
#include "hitbox.h"
#include "vector_operations.h"

void Collisions::addHitbox(const GameObject& object)
{
    HitBox hitbox = object.getHitbox();
    hitboxes.push_back(hitbox);
}

void Collisions::removeHitbox(const GameObject& object)
{

}

void Collisions::addHitsphere(const GameObject& object)
{
    HitSphere hitsphere = object.getHitsphere();
    hitSpheres.push_back(hitsphere);
}

void Collisions::removeHitsphere(const GameObject& object)
{

}

void Collisions::clearHitboxes()
{
    hitboxes.clear();
}

glm::vec4 Collisions::checkPlayerCollision(Player& player)
{
    glm::vec4 player_position = player.getPosition();
    glm::vec4 player_velocity = player.getVelocity();

    glm::vec4 new_player_velocity = glm::vec4(player_velocity.x, player_velocity.y, player_velocity.z, 0.0f);
    glm::vec4 future_player_position = player_position + player_velocity;

    glm::vec4 future_x_position = glm::vec4(future_player_position.x, player_position.y, player_position.z, 1.0f);
    glm::vec4 future_y_position = glm::vec4(player_position.x, future_player_position.y, player_position.z, 1.0f);
    glm::vec4 future_z_position = glm::vec4(player_position.x, player_position.y, future_player_position.z, 1.0f);

    for (HitBox& hitbox : hitboxes)
    {
        glm::vec4 object_min = hitbox.getMinPoint();
        glm::vec4 object_max = hitbox.getMaxPoint();

        bool isXColliding = checkPointAABBCollision(future_x_position, object_min, object_max);
        bool isYColliding = checkPointAABBCollision(future_y_position, object_min, object_max);
        bool isZColliding = checkPointAABBCollision(future_z_position, object_min, object_max);

        if (isXColliding)
        {
            new_player_velocity.x = 0;
        }
        if (isYColliding)
        {
            new_player_velocity.y = 0;
        }
        if (isZColliding)
        {
            new_player_velocity.z = 0;
        }
    }

    for (HitSphere& hitsphere : hitSpheres)
    {
        glm::vec4 sphere_center = hitsphere.getCenter();
        float sphere_radius = hitsphere.getRadius();

        bool isColliding = checkPointSphereCollision(future_player_position, sphere_center, sphere_radius);

        if (isColliding)
        {
            glm::vec4 collision_normal = VectorOperations::normalize(future_player_position - sphere_center);
            new_player_velocity = VectorOperations::reflect(player_velocity, collision_normal);
            new_player_velocity.y = 0;
        }
    }


    return new_player_velocity;
}

bool Collisions::checkPointAABBCollision(glm::vec4 point, glm::vec4 min, glm::vec4 max)
{
    return point.x >= min.x && point.x <= max.x &&
           point.y >= min.y && point.y <= max.y &&
           point.z >= min.z && point.z <= max.z;
}

bool Collisions::checkPointSphereCollision(glm::vec4 point, glm::vec4 sphere_center, float sphere_radius)
{
    return glm::distance(point, sphere_center) <= sphere_radius;
}