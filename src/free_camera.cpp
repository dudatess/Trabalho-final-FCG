#include "free_camera.h"
#include "matrices.h"
#include <GLFW/glfw3.h>
#include <iostream>

FreeCamera::FreeCamera()
{
    this->g_CameraDistance = 3.5f;
    this->g_CameraPhi = 0.0f;
    this->g_CameraTheta = 0.0f;

    float r = g_CameraDistance;
    float y = r * sin(g_CameraPhi);
    float z = r * cos(g_CameraPhi) * cos(g_CameraTheta);
    float x = r * cos(g_CameraPhi) * sin(g_CameraTheta);

    this->camera_position = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
    this->camera_lookat = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    this->camera_view_vector = glm::vec4(x, y, z, 0.0f);
    this->camera_up_vector = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
}

void FreeCamera::update(InputState state, float delta_time)
{
    updatePosition(state, delta_time);
    updateRotation(state, delta_time);
}

void FreeCamera::updatePosition(InputState state, float delta_time)
{
    float speed = 5.0f;

    if (state.move_forward)
    {
        glm::vec4 vec = camera_view_vector;
        vec.y = 0.0f;
        vec = vec / Matrices::norm(vec);
        this->camera_position += vec * speed * delta_time;
    }
    if (state.move_backward)
    {
        glm::vec4 vec = -camera_view_vector;
        vec.y = 0.0f;
        vec = vec / Matrices::norm(vec);
        this->camera_position += vec * speed * delta_time;
    }
    if (state.move_left)
    {
        glm::vec4 vec = Matrices::crossproduct(camera_up_vector, camera_view_vector);
        vec.y = 0.0f;
        vec = vec / Matrices::norm(vec);
        this->camera_position += vec * speed * delta_time;
    }
    if (state.move_right)
    {
        glm::vec4 vec = Matrices::crossproduct(camera_up_vector, -camera_view_vector);
        vec.y = 0.0f;
        vec = vec / Matrices::norm(vec);
        this->camera_position += vec * speed * delta_time;
    }
}

void FreeCamera::updateRotation(InputState state, float delta_time)
{
    float speed = 0.15f;
    float mouse_delta_x = state.mouse_x - old_mouse_x;
    float mouse_delta_y = state.mouse_y - old_mouse_y;

    this->g_CameraTheta += mouse_delta_x * speed * delta_time;
    this->g_CameraPhi += -mouse_delta_y * speed * delta_time;

    if (this->g_CameraPhi > 3.1415f / 2.0f)
    {
        this->g_CameraPhi = 3.1415f / 2.0f;
    }
    else if (this->g_CameraPhi < -3.1415f / 2.0f)
    {
        this->g_CameraPhi = -3.1415f / 2.0f;
    }

    old_mouse_x = state.mouse_x;
    old_mouse_y = state.mouse_y;

    float r = this->g_CameraDistance;
    float y = r * sin(this->g_CameraPhi);
    float z = r * cos(this->g_CameraPhi) * cos(this->g_CameraTheta);
    float x = -r * cos(this->g_CameraPhi) * sin(this->g_CameraTheta);

    this->camera_view_vector = glm::vec4(x, y, z, 0.0f);
}