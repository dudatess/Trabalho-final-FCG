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
    StaticGameObject(
        GpuFunctions *gpu_controller, Object *object, TextureType texture_type, int texture_id, 
        LightType light_type, std::string object_id = "");

    void Update(float deltaTime) override;
    void Render() override;
    void setHitbox(glm::vec4 min_point, glm::vec4 max_point);
    void setHitsphere(glm::vec4 center, float radius);
};

#endif 