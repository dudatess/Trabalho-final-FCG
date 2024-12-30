#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include "free_camera.h"


class Player
{
private:
    FreeCamera camera;
    glm::vec4 position;
public:
    
    void update(InputState state, float delta_time);
    void updatePosition(InputState state, float delta_time);
    FreeCamera getCamera();

    Player();
};

#endif // PLAYER_H