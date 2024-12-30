#include "gpuFunctions.h"
#include <iostream>

GpuFunctions::GpuFunctions(GLuint g_GpuProgramID)
{
    // Buscamos o endereço das variáveis definidas dentro do Vertex Shader.
    // Utilizaremos estas variáveis para enviar dados para a placa de vídeo
    // (GPU)! Veja arquivo "shader_vertex.glsl" e "shader_fragment.glsl".
    this->g_model_uniform      = glGetUniformLocation(g_GpuProgramID, "model"); // Variável da matriz "model"
    this->g_view_uniform       = glGetUniformLocation(g_GpuProgramID, "view"); // Variável da matriz "view" em shader_vertex.glsl
    this->g_projection_uniform = glGetUniformLocation(g_GpuProgramID, "projection"); // Variável da matriz "projection" em shader_vertex.glsl
    this->g_texture_type       = glGetUniformLocation(g_GpuProgramID, "texture_type");
    this->g_light_type         = glGetUniformLocation(g_GpuProgramID, "light_type");
    this->g_texture_id         = glGetUniformLocation(g_GpuProgramID, "texture_id");
}

void GpuFunctions::gpuDraw(GLuint VAO_id, glm::mat4 model, TextureType texture_type, LightType light_type, GLint texture_id, size_t num_indices, size_t first_index)
{
    glBindVertexArray(VAO_id);

    // Envia apenas a matriz de transformação do modelo
    glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
    
    // Define identificadores
    glUniform1i(g_texture_type, texture_type);
    glUniform1i(g_light_type, light_type);
    glUniform1i(g_texture_id, texture_id);

    // Desenha os elementos (usando triângulos)
    glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, (void *)(first_index * sizeof(GLuint)));
   //Desativa o VAO
    glBindVertexArray(0);
}

void GpuFunctions::updateCameraMatrices(FreeCamera camera)
{
    glm::mat4 view_matrix = camera.getViewMatrix();
    glm::mat4 projection_matrix = camera.getProjectionMatrix();

    // Atualiza as matrizes de câmera e projeção na GPU
    glUniformMatrix4fv(GetViewUniform(), 1, GL_FALSE, glm::value_ptr(view_matrix));
    glUniformMatrix4fv(GetProjectionUniform(), 1, GL_FALSE, glm::value_ptr(projection_matrix));
}