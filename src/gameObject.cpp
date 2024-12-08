#include "gameObject.h"

GameObject::GameObject(GpuFunctions *gpu_functions, Object *object)
: model(glm::mat4(1.0f))
{
    this->gpu_functions = gpu_functions;
    this->object = object;
}

void GameObject::UpdateModel()
{
    this->transform.UpdateModelMatrix(this->model);
    //Adicionar colision.UpdateColision(model);
}

void GameObject::Render()
{
        gpu_functions->gpuDraw(
        object->vertex_array_object_id,
        model,
        object->num_indices,
        object->first_index,
        object->object_number);
}
