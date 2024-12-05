#include "freeCamera.h"
#include <GLFW/glfw3.h>
#include <iostream>

FreeCamera::FreeCamera()
{
    this->g_CameraDistance = 3.5f;
    this->g_CameraPhi = 0.0f;
    this->g_CameraTheta = 0.0f;


    float r = g_CameraDistance;
    float y = r*sin(g_CameraPhi);
    float z = r*cos(g_CameraPhi)*cos(g_CameraTheta);
    float x = r*cos(g_CameraPhi)*sin(g_CameraTheta);



    this->camera_position  = glm::vec4(x,y,z,1.0f);
    this->camera_lookat    = glm::vec4(0.0f,0.0f,0.0f,1.0f);
    this->camera_view_vector = camera_lookat - camera_position; 
    this->camera_up_vector   = glm::vec4(0.0f,1.0f,0.0f,0.0f);
}

void FreeCamera::handleKeyboardInput(int key)
{
    if (key == GLFW_KEY_W)
    {
        this->camera_position.z -= 0.3f;
    }
    if (key == GLFW_KEY_S)
    {
        this->camera_position.z += 0.3f;
    }
    if (key == GLFW_KEY_A)
    {
        this->camera_position.x -= 0.3f;
    }
    if (key == GLFW_KEY_D)
    {
        this->camera_position.x += 0.3f;
    }
}

void FreeCamera::updateCamera()
{
    float r = this->g_CameraDistance;
    float y = r*sin(this->g_CameraPhi);
    float z = r*cos(this->g_CameraPhi)*cos(this->g_CameraTheta);
    float x = r*cos(this->g_CameraPhi)*sin(this->g_CameraTheta);

    this->camera_view_vector = glm::vec4(x, y, z, 0.0f);
}