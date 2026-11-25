#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class CTexture{
    private:
        GLuint idVBO_uv;
        GLuint idTexture;
        int tex_width;
        int tex_height;
        int tex_n;
        unsigned char *tex_data;

    public:
        explicit CTexture();
        bool LoadTexture(std::string filename);
        void ActiveTexture();
        void CreateTextureObject(GLenum enum_opengl); // GL_RGB or GL_RGBA
        void DeleteBuffers();
        void TextureBuffer(std::vector<glm::vec2> vertices_uv);
};
