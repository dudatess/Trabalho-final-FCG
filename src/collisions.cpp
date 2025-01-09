#include "collisions.h"
#include "gameObject.h"
#include "hitbox.h"
#include "vector_operations.h"
#include "game_logic.h"

#include <iostream>

void Collisions::addHitbox(const GameObject& object)
{
    HitBox hitbox = object.getHitbox();
    hitboxes[object.getObjectId()] = hitbox;
}

void Collisions::removeHitbox(const GameObject& object)
{
    hitboxes.erase(object.getObjectId());
}

void Collisions::addHitsphere(const GameObject& object)
{
    HitSphere hitsphere = object.getHitsphere();
    hitSpheres.push_back(hitsphere);
}

void Collisions::addClickableHitbox(const GameObject& object)
{
    HitBox hitbox = object.getHitbox();
    this->clickableHitboxes[object.getObjectId()] = hitbox;
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
    glm::vec4 future_player_position = player_position + new_player_velocity;

    bool hasCollidedWithSphere = false;

    for (HitSphere& hitsphere : hitSpheres)
    {
        glm::vec4 sphere_center = hitsphere.getCenter();
        float sphere_radius = hitsphere.getRadius();

        bool isColliding = checkPointSphereCollision(future_player_position, sphere_center, sphere_radius);

        if (isColliding)
        {
            if (hasCollidedWithSphere)
            {
                new_player_velocity = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
                return new_player_velocity;
            }
            glm::vec4 collision_normal = VectorOperations::normalize(future_player_position - sphere_center);
            new_player_velocity = VectorOperations::reflect(new_player_velocity, collision_normal);
            new_player_velocity.y = 0;
            future_player_position = player_position + new_player_velocity;
            hasCollidedWithSphere = true;
        }
    }

    
    glm::vec4 future_x_position = glm::vec4(future_player_position.x, player_position.y, player_position.z, 1.0f);
    glm::vec4 future_y_position = glm::vec4(player_position.x, future_player_position.y, player_position.z, 1.0f);
    glm::vec4 future_z_position = glm::vec4(player_position.x, player_position.y, future_player_position.z, 1.0f);

    for (auto hitbox : hitboxes)
    {
        glm::vec4 object_min = hitbox.second.getMinPoint();
        glm::vec4 object_max = hitbox.second.getMaxPoint();

        //  std::cout << "Object min: " << object_min.x << " " << object_min.y << " " << object_min.z << std::endl;
        //  std::cout << "Object max: " << object_max.x << " " << object_max.y << " " << object_max.z << std::endl;

        bool isXColliding = checkPointAABBCollision(future_x_position, object_min, object_max);
        bool isYColliding = checkPointAABBCollision(future_y_position, object_min, object_max);
        bool isZColliding = checkPointAABBCollision(future_z_position, object_min, object_max);

        //  std::cout << "X colliding: " << isXColliding << std::endl;
        //  std::cout << "Y colliding: " << isYColliding << std::endl;
        //  std::cout << "Z colliding: " << isZColliding << std::endl;

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

        if (isXColliding || isYColliding || isZColliding)
        {
            if (hasCollidedWithSphere)
            {
                new_player_velocity = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
                return new_player_velocity;
            }
        }
    }

    return new_player_velocity;
}

std::unordered_map<std::string, bool> Collisions::checkClickableCollision(Player &player)
{
    std::unordered_map<std::string, bool> clickable_objects_collision;
    
    glm::vec4 player_position = player.getPosition();
    glm::vec4 player_view_vector = player.getFreeCamera().getCameraViewVector();

    for (auto object : this->clickableHitboxes)
    {
        bool isColliding = false;
        isColliding = checkRayToAABBCollision(player_position, player_view_vector, clickableHitboxes[object.first].getMinPoint(), clickableHitboxes[object.first].getMaxPoint());
        clickable_objects_collision[object.first] = isColliding;
    }
    return clickable_objects_collision;
}

bool Collisions::checkPointAABBCollision(glm::vec4 point, glm::vec4 min, glm::vec4 max)
{
    // std::cout << "Is collision on x: " << point.x << " " << min.x << " " << max.x << std::endl;
    // std::cout << "bool x: " << (point.x >= min.x) << " " << (point.x <= max.x) << std::endl;
    // std::cout << "Is collision on y: " << point.y << " " << min.y << " " << max.y << std::endl;
    // std::cout << "bool y: " << (point.y >= min.y) << " " << (point.y <= max.y) << std::endl;
    // std::cout << "Is collision on z: " << point.z << " " << min.z << " " << max.z << std::endl;
    // std::cout << "bool z: " << (point.z >= min.z) << " " << (point.z <= max.z) << std::endl;
    return point.x >= min.x && point.x <= max.x &&
           point.y >= min.y && point.y <= max.y &&
           point.z >= min.z && point.z <= max.z;
}

bool Collisions::checkPointSphereCollision(glm::vec4 point, glm::vec4 sphere_center, float sphere_radius)
{
    return glm::distance(point, sphere_center) <= sphere_radius;
}

 bool Collisions::checkRayToAABBCollision(glm::vec4 ray_origin, glm::vec4 ray_direction, glm::vec4 min, glm::vec4 max)
{
    float tMin = 0.0f;
    float tMax = 3.0f;

    // std::cout << "Ray origin: " << ray_origin.x << " " << ray_origin.y << " " << ray_origin.z << std::endl;
    // std::cout << "Ray direction: " << ray_direction.x << " " << ray_direction.y << " " << ray_direction.z << std::endl;

    for (int i = 0; i < 3; i++) {
        if (fabs(ray_direction[i]) < 1e-8f) {
            // Ray is parallel; check if origin is outside slab
            if (ray_origin[i] < min[i] || ray_origin[i] > max[i]) return false;
        } else {
            float ood = 1.0f / ray_direction[i];
            float t1 = (min[i] - ray_origin[i]) * ood;
            float t2 = (max[i] - ray_origin[i]) * ood;
            if (t1 > t2) std::swap(t1, t2);
            tMin = std::max(tMin, t1);
            tMax = std::min(tMax, t2);
            if (tMin > tMax) return false;
        }
    }
    return true;
}

