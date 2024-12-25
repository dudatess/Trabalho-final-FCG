#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <glad/glad.h>
#include <string>
#include <unordered_map>
#include <iostream>
#include "stb_image.h"

enum TextureType
{
    SPHERE,
    PLANE,
    OBJ_FILE
};

class Texture
{
private:
    GLuint number_of_loaded_textures;
    std::unordered_map<std::string, unsigned int> texture_ids;

public:
    Texture();
    void LoadTextureImage(const char *filename, std::string name);
    unsigned int GetTexture(std::string name);
};

#endif // _TEXTURELOADER_H