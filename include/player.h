#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include "free_camera.h"


class Player
{
private:
    FreeCamera camera;
    glm::vec4 position;
    glm::vec4 velocity;
public:
    
    void update(InputState state, float delta_time);
    void updatePosition();
    void updateVelocity(InputState state, float delta_time);
    void setPosition(glm::vec4 position);

    glm::vec4 getPosition();
    glm::vec4 getVelocity() { return velocity; }
    FreeCamera getCamera();

    void setPositionX(float x) { position.x = x; }
    void setPositionY(float y) { position.y = y; }
    void setPositionZ(float z) { position.z = z; }

    void setVelocity(glm::vec4 velocity) { this->velocity = velocity; }
    void setVelocityX(float x) { velocity.x = x; }
    void setVelocityY(float y) { velocity.y = y; }
    void setVelocityZ(float z) { velocity.z = z; }

    void printPlayerPosition();

    Player();
};

#endif // PLAYER_H