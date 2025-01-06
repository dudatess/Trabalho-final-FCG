#ifndef _DOOR_H
#define _DOOR_H

#include "gameObject.h"
#include "player.h"
#include "texture.h"
#include "player.h"
#include "collisions.h"

class Door : public GameObject
{
private:
    Player *player;
    Collisions *collisions; 
    InputHandler *input_handler;
    int texture_id;

    bool is_opening;
    bool is_closing;
    float progression_time;

    const float angle = 3.141592 / 2;
    const float animation = 0.75f;
    const float distance = 1.5f;

    

public:
    Door(
        GpuFunctions *gpu_controller, Object *object, TextureType texture_type, int texture_id, 
        LightType light_type, std::string object_id = "");
    void Update(float deltaTime) override;
    void Render() override;

    void OpeningDoor(float deltaTime);
    void ClosingDoor(float deltaTime);
};

#endif