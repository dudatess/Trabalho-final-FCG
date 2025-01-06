
#include <iostream>
#include "staticGameObject.h"
#include "gpuFunctions.h"

StaticGameObject::StaticGameObject(
    GpuFunctions *gpu_functions, Object *object, TextureType texture_type, 
    int texture_id, LightType light_type, std::string object_id)
    : GameObject(gpu_functions, Matrices::Matrix_Identity(), object, texture_type, light_type, object_id)
{
    this->texture_id = texture_id;
}


void StaticGameObject::Update(float deltaTime)
{
}


void StaticGameObject::Render()
{
     gpu_functions->gpuDraw(
        object->vertex_array_object_id,
        model,
        texture_type,
        light_type,
        texture_id,
        object->num_indices,
        object->first_index);
}

void StaticGameObject::setHitbox(glm::vec4 min_point, glm::vec4 max_point)
{
    this->hitbox = HitBox(min_point, max_point);
}

void StaticGameObject::setHitsphere(glm::vec4 center, float radius)
{
    this->hitsphere = HitSphere(center, radius);
}
