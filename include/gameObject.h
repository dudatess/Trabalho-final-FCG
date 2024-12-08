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
    GpuFunctions *gpuController;
    //Adicionar LightInterpolationModel
    Object *object;
    glm::mat4 model;

    GameObject(GpuFunctions *gpuController, Object *object)
    {
        this->gpuController = gpuController;
        this->object = object;
    }
    
public:
    Transform transform;
    //Adicionar Colision colision;

    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;

    void UpdateModel();
    
    virtual ~GameObject() = default;

};


#endif // _GAMEOBJECT_H