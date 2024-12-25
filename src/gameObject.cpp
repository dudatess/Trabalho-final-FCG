#include "gameObject.h"

GameObject::GameObject(GpuFunctions *gpu_functions, glm::mat4 model, Object *object, LightType light_type)
{
    this->gpu_functions = gpu_functions;
    this->model = model;
    this->object = object;
    this->light_type = light_type;
}

void GameObject::UpdateModel()
{
    this->transform.UpdateModelMatrix(this->model);
}

