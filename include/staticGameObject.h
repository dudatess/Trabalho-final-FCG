#ifndef _STATICGAMEOBJECT_H
#define _STATICGAMEOBJECT_H

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <tiny_obj_loader.h>

#include <cassert>
#include <string>
#include "gpuFunctions.h"
#include "gameObject.h"
#include "matrices.h"
#include "object.h"
#include "texture.h"

class StaticGameObject : public GameObject
{
private:
    int texture_id;

public:
    StaticGameObject(GpuFunctions *gpu_controller, Object *object, TextureType texture_type, int texture_id, LightType light_type, glm::mat4 model = Matrices::Matrix_Identity());
    void Update(float deltaTime) override;
    void Render() override;
};

#endif 