#ifndef _FREECAMERA_H
#define _FREECAMERA_H

#include <glm/glm.hpp>

class FreeCamera
{
    private:
        glm::vec4 camera_position;
        glm::vec4 camera_lookat;
        glm::vec4 camera_view_vector;
        glm::vec4 camera_up_vector;

    public:

        FreeCamera();
        glm::vec4 getCameraPosition() { return camera_position; }
        glm::vec4 getCameraLookAt() { return camera_lookat; }
        glm::vec4 getCameraViewVector() { return camera_view_vector; }
        glm::vec4 getCameraUpVector() { return camera_up_vector; }
        void handleKeyboardInput(int key);
        void updateCamera();

        float g_CameraDistance;
        float g_CameraPhi;
        float g_CameraTheta;
};

#endif //_FREECAMERA_H