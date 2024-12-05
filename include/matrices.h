#ifndef _MATRICES_H
#define _MATRICES_H

#include <cstdio>
#include <cstdlib>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Matrices
{
    public:
        static glm::mat4 Matrix(
            float m00, float m01, float m02, float m03, // LINHA 1
            float m10, float m11, float m12, float m13, // LINHA 2
            float m20, float m21, float m22, float m23, // LINHA 3
            float m30, float m31, float m32, float m33  // LINHA 4
        );
        static glm::mat4 Matrix_Identity();
        static glm::mat4 Matrix_Translate(float tx, float ty, float tz);
        static glm::mat4 Matrix_Scale(float sx, float sy, float sz);
        static glm::mat4 Matrix_Rotate_X(float angle);
        static glm::mat4 Matrix_Rotate_Y(float angle);
        static glm::mat4 Matrix_Rotate_Z(float angle);
        static glm::mat4 Matrix_Rotate(float angle, glm::vec4 axis);
        static glm::vec4 crossproduct(glm::vec4 u, glm::vec4 v);
        static float norm(glm::vec4 v);
        static float dotproduct(glm::vec4 u, glm::vec4 v);
        static glm::mat4 Matrix_Camera_View(glm::vec4 position_c, glm::vec4 view_vector, glm::vec4 up_vector);
        static glm::mat4 Matrix_Orthographic(float l, float r, float b, float t, float n, float f);
        static glm::mat4 Matrix_Perspective(float field_of_view, float aspect, float n, float f);
        static void PrintMatrix(glm::mat4 M);
        static void PrintVector(glm::vec4 v);
        static void PrintMatrixVectorProduct(GLFWwindow* window, glm::mat4 M, glm::vec4 v, float x, float y, float scale);
        static void PrintMatrixVectorProductDivW(GLFWwindow* window, glm::mat4 M, glm::vec4 v, float x, float y, float scale);

};


#endif //_MATRICES_H
