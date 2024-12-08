#include "gameObject.h"

GameObject::GameObject(GpuFunctions *gpuController, Object *object)
{
    this->gpuController = gpuController;
    this->object = object;
}

void GameObject::UpdateModel()
{
    this->transform.UpdateModelMatrix(this->model);
    //Adicionar colision.UpdateColision(model);
}