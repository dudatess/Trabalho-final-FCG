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
    this->g_object_id_uniform  = glGetUniformLocation(g_GpuProgramID, "object_id"); // Variável "object_id" em shader_fragment.glsl

}

void GpuFunctions::gpuDraw(GLuint VAO_id, glm::mat4 model, size_t num_indices, size_t first_index, int object_number)
{
    glBindVertexArray(VAO_id); // Ativa o VAO do objeto a ser desenhado

    // Envia apenas a matriz de transformação do modelo
    glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));

    // Define um identificador para o objeto (opcional, se necessário no shader)
    glUniform1i(g_object_id_uniform, object_number);

    // Desenha os elementos (usando triângulos neste exemplo)
    glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, (void *)(first_index * sizeof(GLuint)));

    // Desativa o VAO
    glBindVertexArray(0);
}

void GpuFunctions::updateCameraMatrices(glm::mat4 view_matrix, glm::mat4 projection_matrix)
{
    // Atualiza as matrizes de câmera e projeção na GPU
    glUniformMatrix4fv(GetViewUniform(), 1, GL_FALSE, glm::value_ptr(view_matrix));
    glUniformMatrix4fv(GetProjectionUniform(), 1, GL_FALSE, glm::value_ptr(projection_matrix));
}