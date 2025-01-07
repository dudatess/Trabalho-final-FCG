#ifndef _GPUFUNCTIONS_H
#define _GPUFUNCTIONS_H

#include <glm/mat4x4.hpp>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "texture.h"
#include "lightType.h"
#include "free_camera.h"
#include "lookat_camera.h"  

class GpuFunctions
{
    private:
        GLint g_model_uniform;
        GLint g_view_uniform;
        GLint g_projection_uniform;
        //GLint g_object_id_uniform;
        GLint g_texture_type;
        GLint g_light_type;
        GLint g_texture_id;

    public:
        GpuFunctions(GLuint g_GpuProgramID);
        void gpuDraw(GLuint VAO_id, glm::mat4 model, TextureType texture_type, LightType light_type, GLint texture_id, size_t num_indices, size_t first_index);
        void updateFreeCameraMatrices(FreeCamera camera);
        void updateLookAtCameraMatrices(LookAtCamera camera);

        GLuint GetModelUniform() { return g_model_uniform; }
        GLuint GetViewUniform() { return g_view_uniform; }
        GLuint GetProjectionUniform() { return g_projection_uniform; }
        //GLuint GetObjectIdUniform() { return g_object_id_uniform; }
};


#endif //_GPUFUNCTIONS_H