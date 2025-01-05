#include "gameObject.h"

GameObject::GameObject(GpuFunctions *gpu_functions, glm::mat4 model, Object *object,TextureType texture_type, LightType light_type)
{
    this->gpu_functions = gpu_functions;
    this->model = model;
    this->object = object;
    this->texture_type = texture_type;
    this->light_type = light_type;
}

void GameObject::UpdateModel()
{
    this->transform.UpdateModelMatrix(this->model);
}

HitBox GameObject::getHitbox() const
{
    return this->hitbox;
}

HitSphere GameObject::getHitsphere() const
{
    return this->hitsphere;
}

