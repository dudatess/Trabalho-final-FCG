#include "door.h"

Door::Door(GpuFunctions *gpu_functions, Object *object, Player *player, Collisions *collisions, int texture_id, glm::mat4 model)
    : GameObject(gpu_functions, model, object, TextureType::OBJ_FILE, LightType::PHONG)
{
    this->texture_id = texture_id;
    this->player = player;
    this->collisions = collisions; // Inicializa a variável de colisões

    this->is_opening = false;
    this->is_closing = false;
    this->progression_time = 0.0f;
}

void Door::Update(float deltaTime)
{
    // Verifica se o jogador está pressionando a tecla E e se a porta está dentro da linha de visão
    InputState currentState = input_handler->getInputState();

    // Acesse a HitBox do GameObject através do método getHitbox()
    if (currentState.is_interacting && this->collisions->checkRayToAABBCollision(player->getPosition(), player->getFreeCamera().getCameraViewVector(), this->getHitbox().getMinPoint(), this->getHitbox().getMaxPoint()))
    {
        if (this->progression_time <= this->animation && !is_closing)
            this->is_opening = true;
        else
            this->is_closing = true;
    }

    if (this->is_opening)
        OpeningDoor(deltaTime);
    else if (this->is_closing)
        ClosingDoor(deltaTime);

    // Atualiza a posição da porta e a colisão
    this->UpdateModel();

    // Aqui chamamos a função que verifica a colisão da porta
    this->collisions->addHitbox(*this);  // Adiciona a colisão da porta ao sistema
    this->collisions->checkPlayerCollision(*player); // Verifica colisão com o jogador
}


void Door::OpeningDoor(float deltaTime)
{
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

void Door::Render()
{
    gpu_functions->gpuDraw(
        object->vertex_array_object_id,
        model,
        TextureType::OBJ_FILE,
        LightType::PHONG,
        texture_id,
        object->num_indices,
        object->first_index);
    
}

void Door::setHitbox(glm::vec4 min_point, glm::vec4 max_point)
{
    this->hitbox = HitBox(min_point, max_point);
}