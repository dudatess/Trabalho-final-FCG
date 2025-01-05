#include "player.h"
#include "matrices.h"
#include "bezier_curve.h"

Player::Player()
{
    this->camera_type = CameraType::LOOK_AT_CAMERA; 
    this->free_camera = FreeCamera();
    this->look_at_camera = LookAtCamera();
    this->position = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
    this->velocity = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
      
    glm::vec4 p1 = position; 
    glm::vec4 p2 = p1 + glm::vec4(10.0f, 5.0f, 0.0f, 0.0f); // Ponto de controle 1
    glm::vec4 p3 = p1 + glm::vec4(20.0f, 5.0f, 0.0f, 0.0f); // Ponto de controle 2
    glm::vec4 p4 = p1 + glm::vec4(30.0f, 0.0f, 0.0f, 0.0f); // Ponto final
    this->bezier_curve = new BezierCurve(5.0f, p1, p2, p3, p4);

}


void Player::update(InputState state, float delta_time)
{
    if (isMoving) 
    {
        this->bezier_curve->Update(delta_time); // Atualiza a posição com a curva de Bezier
        position = this->bezier_curve->GetPoint(); // Atualiza a posição do jogador

        if (this->bezier_curve->HasFinished()) {
            isMoving = false; // Para a movimentação quando a curva terminar
        }
    }
    // Se o movimento terminar, o jogador está em uma posição inicial
    if (!isMoving) {
        
        updatePosition();
        if(camera_type == CameraType::FREE_CAMERA)
        {
            this->free_camera.updateCameraPosition(this->position);
            this->free_camera.updateCameraRotation(state, delta_time);
        }
        else
        {
            this->look_at_camera.updateCameraPosition(this->position);
            this->look_at_camera.updateCameraRotation(state, delta_time);
        }
    }
    

}

void Player::updatePosition()
{
    this->position += this->velocity;
}

void Player::updateVelocity(InputState state, float delta_time)
{
    glm::vec4 camera_view_vector;
    glm::vec4 camera_up_vector;

    if(camera_type == CameraType::FREE_CAMERA)
    {
        camera_view_vector = this->free_camera.getCameraViewVector();
        camera_up_vector = this->free_camera.getCameraUpVector();
    }
    else
    {
        camera_view_vector = this->look_at_camera.getCameraViewVector();
        camera_up_vector = this->look_at_camera.getCameraUpVector();
    }
    

    float speed = 5.0f;

    if (state.is_running)
    {
        speed = 20.0f;
    }

    this->velocity = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

    if(camera_type == CameraType::FREE_CAMERA)
    {
        if (state.move_forward)
        {
            glm::vec4 vec = camera_view_vector;
            vec.y = 0.0f;
            vec = vec / Matrices::norm(vec);
            this->velocity += vec * speed * delta_time;
        }
        if (state.move_backward)
        {
            glm::vec4 vec = -camera_view_vector;
            vec.y = 0.0f;
            vec = vec / Matrices::norm(vec);
            this->velocity += vec * speed * delta_time;
        }
        if (state.move_left)
        {
            glm::vec4 vec = Matrices::crossproduct(camera_up_vector, camera_view_vector);
            vec.y = 0.0f;
            vec = vec / Matrices::norm(vec);
            this->velocity += vec * speed * delta_time;
        }
        if (state.move_right)
        {
            glm::vec4 vec = Matrices::crossproduct(camera_up_vector, -camera_view_vector);
            vec.y = 0.0f;
            vec = vec / Matrices::norm(vec);
            this->velocity += vec * speed * delta_time;
        }

    }
    else{

        if (state.move_forward)
        {
            glm::vec4 vec = glm::normalize(glm::vec4(camera_view_vector.x, 0.0f, camera_view_vector.z, 0.0f));
            this->velocity += vec * speed * delta_time;
        }
        if (state.move_backward)
        {
            glm::vec4 vec = glm::normalize(glm::vec4(-camera_view_vector.x, 0.0f, -camera_view_vector.z, 0.0f));
            this->velocity += vec * speed * delta_time;
        }

    }

}

FreeCamera Player::getFreeCamera()
{
    return this->free_camera;
}

LookAtCamera Player::getLookAtCamera()
{
    return this->look_at_camera;
}

glm::vec4 Player::getPosition()
{
    return this->position;
}

void Player::setPosition(glm::vec4 position)
{
    this->position = position;

    if(camera_type == CameraType::FREE_CAMERA) {
        this->free_camera.updateCameraPosition(this->position);
    }
    else {
        this->look_at_camera.updateCameraPosition(this->position);
    }
}

void Player::resetPosition() {
    this->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); // Define a posição inicial
}

void Player::printPlayerPosition()
{
    std::cout << "Player position: " << this->position.x << " " << this->position.y << " " << this->position.z << std::endl;
}

void Player::updateCamera(bool isOpening) {
    if (isOpening) {
        this->camera_type = CameraType::LOOK_AT_CAMERA; // Define a câmera como look-at
    } else {
        this->camera_type = CameraType::FREE_CAMERA; // Define a câmera como free
    }
}
