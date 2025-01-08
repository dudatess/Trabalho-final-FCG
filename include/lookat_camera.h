#ifndef _LOOKATCAMERA_H
#define _LOOKATCAMERA_H


#include <glm/glm.hpp>
#include "input_handler.h"

class LookAtCamera
{
private:

    bool is_static_camera;
    glm::vec4 static_position; // Posição fixa da câmera
    glm::vec4 static_lookat;   // Ponto fixo para onde a câmera olha


    glm::vec4 camera_position; // Ponto "c", centro da câmera
    glm::vec4 camera_lookat;// Ponto "l", para onde a câmera (look-at) estará sempre olhando
    glm::vec4 camera_view_vector; // Vetor "view", sentido para onde a câmera está virada
    glm::vec4 camera_up_vector; // Vetor "up" fixado para apontar para o "céu" (eito Y global)

    float field_of_view = 3.141592 / 3.0f;
    float g_ScreenRatio = 1.0f;
    float nearplane = -0.1f;  // Posição do "near plane"
    float farplane  = -150.0f; // Posição do "far plane"

    float g_CameraPhi;   // Angle between ZX plane and Z axis
    float g_CameraTheta; // Angle with respect to the Y axis
    float g_CameraDistance; // Distance to look at point

    double old_mouse_x = 0.0;
    double old_mouse_y = 0.0;


public:

        LookAtCamera();
        void setStaticCamera(glm::vec4 position, glm::vec4 lookat, glm::vec4 up_vector);
        void disableStaticCamera();

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

#endif // _LOOKATCAMERA_H