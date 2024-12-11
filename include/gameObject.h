#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include <string>
#include <glad/glad.h>
#include <glm/mat4x4.hpp>

#include "gpuFunctions.h"
#include "object.h"
#include "transform.h"

class GameObject
{

protected:
    GpuFunctions *gpu_functions;
    //Adicionar LightInterpolationModel
    Object *object;
    glm::mat4 model;

public:

    GameObject(GpuFunctions *gpuController, Object *object);
    Transform transform;
    //Adicionar Colision colision;

    //Esses métodos devem ser abstratos depois
    //void Update(float deltaTime);
    void Render();

    void UpdateModel();
    
    virtual ~GameObject() = default;

};


#endif // _GAMEOBJECT_H