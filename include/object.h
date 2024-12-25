#ifndef _OBJECT_H
#define _OBJECT_H

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <tiny_obj_loader.h>
#include <stdexcept>

#include <cassert>
#include <string>
#include "matrices.h"

class Object
{
private:
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    void ComputeNormals();
    void BuildObject();

public:
    Object(const char *filename, const char* basepath = NULL, bool triangulate = true);
    GLuint vertex_array_object_id;
    size_t first_index;
    size_t num_indices;

    void PrintObjModelInfo();
    // getters
    tinyobj::attrib_t GetAttrib();
    std::vector<tinyobj::shape_t> GetShapes();
    std::vector<tinyobj::material_t> GetMaterials();

    //Para a textura
    glm::vec3 bbox_min;
    glm::vec3 bbox_max;
};

#endif //_OBJECT_H