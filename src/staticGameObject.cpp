
#include <iostream>
#include "staticGameObject.h"
#include "gpuFunctions.h"



StaticGameObject::StaticGameObject(GpuFunctions *gpu_functions, Object *object, int texture_id, LightType light_type, glm::mat4 model)
    : GameObject(gpu_functions, model, object, light_type)
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
        TextureType::OBJ_FILE,
        light_type,
        texture_id,
        object->num_indices,
        object->first_index);
}
