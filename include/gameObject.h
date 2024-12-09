#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include <string>
#include <glad/glad.h>
#include <glm/mat4x4.hpp>

#include "gpuFunctions.h"
#include "object.h"
#include "transform.h"
#include "lightType.h"

class GameObject
{

protected:
    GpuFunctions *gpu_functions;
    LightType light_type;
    Object *object;
    glm::mat4 model;

public:

    GameObject(GpuFunctions *gpuController, Object *object, LightType light_type);

    Transform transform;
    //Adicionar Colision colision;

    //Esses métodos devem ser abstratos depois
    //void Update(float deltaTime);
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;

    void UpdateModel();
    
    virtual ~GameObject() = default;

};


#endif // _GAMEOBJECT_H