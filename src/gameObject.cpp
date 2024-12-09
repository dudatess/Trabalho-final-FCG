#include "gameObject.h"

GameObject::GameObject(GpuFunctions *gpu_functions, Object *object, LightType light_type) : model(glm::mat4(1.0f))
{
    this->gpu_functions = gpu_functions;
    this->object = object;
    this->light_type = light_type;
}

void GameObject::UpdateModel()
{
    this->transform.UpdateModelMatrix(this->model);
    //Adicionar colision.UpdateColision(model);
}

