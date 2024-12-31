#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include <string>
#include <glad/glad.h>
#include <glm/mat4x4.hpp>

#include "gpuFunctions.h"
#include "object.h"
#include "transform.h"
#include "lightType.h"
#include "texture.h"
#include "hitbox.h"

class GameObject
{
private:

protected:
    GpuFunctions *gpu_functions;
    TextureType texture_type;
    LightType light_type;
    Object *object;
    glm::mat4 model;
    //Adicionar colisão;
    HitBox hitbox;

public:

    GameObject(GpuFunctions *gpuController, glm::mat4 model, Object *object, TextureType texture_type, LightType light_type);

    Transform transform;

    //void Update(float deltaTime);
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;

    void UpdateModel();
    
    virtual ~GameObject() = default;

    HitBox getHitbox() const;
};


#endif // _GAMEOBJECT_H