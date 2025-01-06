#include "door.h"

Door::Door(
    GpuFunctions *gpu_functions, Object *object, TextureType texture_type, 
    int texture_id, LightType light_type, std::string object_id)
    : GameObject(gpu_functions, Matrices::Matrix_Identity(), object, texture_type, light_type, object_id)
{
    this->texture_id = texture_id;
    this->is_opening = false;
    this->is_closing = false;
    this->progression_time = 0.0f;
}


void Door::OpeningDoor(float deltaTime)
{
    std::cout << "Opening door" << std::endl;
    if (this->progression_time >= animation)
        this->is_opening = false;

    this->progression_time += deltaTime;

    // Defina a rotação z diretamente no transform.rotation
    this->transform.rotation.z = (this->progression_time / animation) * angle;

    // Atualize a rotação usando a função SetRotation
    this->transform.SetRotation(this->transform.rotation.x, this->transform.rotation.y, this->transform.rotation.z);
}


void Door::ClosingDoor(float deltaTime)
{
    if (this->progression_time <= 0.0f)
        this->is_closing = false;

    this->progression_time -= deltaTime;
    this->transform.rotation.z = (progression_time / animation) * (3.141592 / 2);
    this->transform.SetRotation(this->transform.rotation.x, this->transform.rotation.y, this->transform.rotation.z);
}

void Door::Update(float deltaTime)
{
}


void Door::Render()
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