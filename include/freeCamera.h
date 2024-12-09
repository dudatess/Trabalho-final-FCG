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

    public:

        FreeCamera();
        glm::vec4 getCameraPosition() { return camera_position; }
        glm::vec4 getCameraLookAt() { return camera_lookat; }
        glm::vec4 getCameraViewVector() { return camera_view_vector; }
        glm::vec4 getCameraUpVector() { return camera_up_vector; }

        void update(InputState state, float delta_time);
        void updatePosition(InputState state, float delta_time);
        void updateRotation(InputState state, float delta_time);
};

#endif //_FREECAMERA_H