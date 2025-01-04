#include "lookat_camera.h"
#include "matrices.h"

LookAtCamera::LookAtCamera()
{
    this->g_CameraTheta = 0.0f;
    this->g_CameraPhi = 0.0f;
    this->g_CameraDistance = 3.5f;

    // Computamos a posição da câmera utilizando coordenadas esféricas.  As
    // variáveis g_CameraDistance, g_CameraPhi, e g_CameraTheta são
    // controladas pelo mouse do usuário. Veja as funções CursorPosCallback()
    // e ScrollCallback().
    float r = g_CameraDistance;
    float y = r*sin(g_CameraPhi);
    float z = r*cos(g_CameraPhi)*cos(g_CameraTheta);
    float x = r*cos(g_CameraPhi)*sin(g_CameraTheta);

    this->camera_position  = glm::vec4(x,y,z,1.0f); // Ponto "c", centro da câmera
    this->camera_lookat   = glm::vec4(0.0f,0.0f,0.0f,1.0f); // Ponto "l", para onde a câmera (look-at) estará sempre olhando
    this->camera_view_vector = camera_lookat - camera_position; // Vetor "view", sentido para onde a câmera está virada
    this->camera_up_vector   = glm::vec4(0.0f,1.0f,0.0f,0.0f); // Vetor "up" fixado para apontar para o "céu" (eito Y global)

}

glm::mat4 LookAtCamera::getViewMatrix()
{
    // Computamos a matriz "View" utilizando os parâmetros da câmera para
        // definir o sistema de coordenadas da câmera.  Veja slides 2-14, 184-190 e 236-242 do documento Aula_08_Sistemas_de_Coordenadas.pdf.
    return Matrices::Matrix_Camera_View(this->camera_position, this->camera_view_vector, this->camera_up_vector);
}

glm::mat4 LookAtCamera::getProjectionMatrix()
{
    // Para definição do field of view (FOV), veja slides 205-215 do documento Aula_09_Projecoes.pdf.
    return Matrices::Matrix_Perspective(field_of_view, g_ScreenRatio, nearplane, farplane);
}

void LookAtCamera::updateCameraRotation(InputState state, float delta_time)
{
    /*
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

    //this->camera_view_vector = glm::vec4(x, y, z, 0.0f);

    //NEW
    this->camera_lookat = this->camera_position + glm::vec4(x, y, z, 0.0f);
    this->camera_view_vector = glm::normalize(this->camera_lookat - this->camera_position);*/

    // Se o look_at for fixo, não atualizamos rotação baseada no mouse.
    // Apenas recalculamos view_vector caso tenha sido modificado externamente.
    this->camera_view_vector = glm::normalize(this->camera_lookat - this->camera_position);
}


void LookAtCamera::updateCameraPosition(glm::vec4 position)
{
    this->camera_position = position;

    //NEW
    this->camera_position = position;

    // Define o look_at como fixo no centro do mundo
    this->camera_lookat = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    // Atualiza o vetor de visão
    this->camera_view_vector = glm::normalize(this->camera_lookat - this->camera_position);
}
