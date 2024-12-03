#ifndef _GPUFUNCTIONS_H
#define _GPUFUNCTIONS_H

#include <glm/mat4x4.hpp>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

class GpuFunctions
{
    private:
        GLint g_model_uniform;
        GLint g_view_uniform;
        GLint g_projection_uniform;
        GLint g_object_id_uniform;

    public:
        GpuFunctions(GLuint g_GpuProgramID);
        void gpuDraw(GLuint VAO_id, glm::mat4 model, size_t num_indices, size_t first_index, int object_number);

        GLuint GetModelUniform() { return g_model_uniform; }
        GLuint GetViewUniform() { return g_view_uniform; }
        GLuint GetProjectionUniform() { return g_projection_uniform; }
        GLuint GetObjectIdUniform() { return g_object_id_uniform; }
};


#endif //_GPUFUNCTIONS_H