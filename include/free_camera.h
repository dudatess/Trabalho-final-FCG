#ifndef _FREECAMERA_H
#define _FREECAMERA_H

#include <glm/glm.hpp>
#include "input_handler.h"

class FreeCamera
{
    private:
    
        glm::vec4 camera_position;
        glm::vec4 camera_lookat;
        glm::vec4 camera_view_vector;
        glm::vec4 camera_up_vector;

        float g_CameraDistance;
        float g_CameraPhi;
        float g_CameraTheta;

        double old_mouse_x = 0.0;
        double old_mouse_y = 0.0;

        float near_plane = -0.1f;  // Posição do "near plane"
        float far_plane  = -150.0f; // Posição do "far plane"
        float field_of_view = 3.141592 / 3.0f;
        
    public:

        FreeCamera();
        glm::vec4 getCameraPosition() { return camera_position; }
        glm::vec4 getCameraLookAt() { return camera_lookat; }
        glm::vec4 getCameraViewVector() { return camera_view_vector; }
        glm::vec4 getCameraUpVector() { return camera_up_vector; }

        glm::mat4 getViewMatrix();
        glm::mat4 getProjectionMatrix();

        void updateCameraPosition(glm::vec4 position);
        void updateCameraRotation(InputState state, float delta_time);
        void setPosition(glm::vec4 position) { camera_position = position; }
};

#endif //_FREECAMERA_H