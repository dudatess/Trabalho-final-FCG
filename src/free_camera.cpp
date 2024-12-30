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

void FreeCamera::updateCameraPosition(glm::vec4 position)
{
    this->camera_position = position;
}

void FreeCamera::updateCameraRotation(InputState state, float delta_time)
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

glm::mat4 FreeCamera::getViewMatrix()
{
    return Matrices::Matrix_Camera_View(this->camera_position, this->camera_view_vector, this->camera_up_vector);
}

glm::mat4 FreeCamera::getProjectionMatrix()
{
    //TODO: Nao sei se o 1.0f ali sempre da certo, no codigo do sor tava g_ScreenRatio (que era width / height da janela)
    return Matrices::Matrix_Perspective(field_of_view, 1.0f, near_plane, far_plane);
}