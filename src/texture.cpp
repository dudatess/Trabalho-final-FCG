#include "texture.h"

Texture::Texture()
{
    this->number_of_loaded_textures = 0;
}

void Texture::LoadTextureImage(const char *filename, std::string name)
{
    if (texture_ids.find(name) == texture_ids.end())
    {
        texture_ids[name] = number_of_loaded_textures;
    }
    else
    {
        std::cerr << "[ERROR] LoadTexture: a texture of name " << name << " already exists in the game" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    printf("Carregando imagem \"%s\"... ", filename);

    stbi_set_flip_vertically_on_load(true);
    int width;
    int height;
    int channels;
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 3);

    if (data == NULL)
    {
        fprintf(stderr, "ERROR: Cannot open image file \"%s\".\n", filename);
        std::exit(EXIT_FAILURE);
    }

    printf("OK (%dx%d).\n", width, height);

    GLuint texture_id;
    GLuint sampler_id;
    glGenTextures(1, &texture_id);
    glGenSamplers(1, &sampler_id);

    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glSamplerParameteri(sampler_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glSamplerParameteri(sampler_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

    GLuint textureunit = this->number_of_loaded_textures;
    glActiveTexture(GL_TEXTURE0 + textureunit);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindSampler(textureunit, sampler_id);

    stbi_image_free(data);

    this->number_of_loaded_textures += 1;
}

unsigned int Texture::GetTexture(std::string name)
{
    auto iter = texture_ids.find(name);

    if (iter == texture_ids.end())
    {
        std::cerr << "[ERROR] GetTexture: a texture of name " << name << " doesn't exist" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    else
    {
        return iter->second;
    }
}