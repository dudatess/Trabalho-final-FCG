#include "player.h"
#include "matrices.h"

Player::Player()
{
    this->camera = FreeCamera();
    this->position = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
}

void Player::update(InputState state, float delta_time)
{
    updatePosition(state, delta_time);
    this->camera.updateCameraPosition(this->position);
    this->camera.updateCameraRotation(state, delta_time);
}

void Player::updatePosition(InputState state, float delta_time)
{
    glm::vec4 camera_view_vector = this->camera.getCameraViewVector();
    glm::vec4 camera_up_vector = this->camera.getCameraUpVector();

    float speed = 5.0f;

    if (state.is_running)
    {
        speed = 20.0f;
    }

    if (state.move_forward)
    {
        glm::vec4 vec = camera_view_vector;
        vec.y = 0.0f;
        vec = vec / Matrices::norm(vec);
        this->position += vec * speed * delta_time;
    }
    if (state.move_backward)
    {
        glm::vec4 vec = -camera_view_vector;
        vec.y = 0.0f;
        vec = vec / Matrices::norm(vec);
        this->position += vec * speed * delta_time;
    }
    if (state.move_left)
    {
        glm::vec4 vec = Matrices::crossproduct(camera_up_vector, camera_view_vector);
        vec.y = 0.0f;
        vec = vec / Matrices::norm(vec);
        this->position += vec * speed * delta_time;
    }
    if (state.move_right)
    {
        glm::vec4 vec = Matrices::crossproduct(camera_up_vector, -camera_view_vector);
        vec.y = 0.0f;
        vec = vec / Matrices::norm(vec);
        this->position += vec * speed * delta_time;
    }
}

FreeCamera Player::getCamera()
{
    return this->camera;
}

glm::vec4 Player::getPosition()
{
    return this->position;
}

void Player::setPosition(glm::vec4 position)
{
    this->position = position;
    this->camera.updateCameraPosition(this->position);
}