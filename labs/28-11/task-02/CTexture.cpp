#include "CTexture.h"
#include <glad/glad.h>
#include <string>
#include <cstdio>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


CTexture::CTexture(){
    idVBO_uv = 0;
    idTexture = 0;
    tex_data = nullptr;
    tex_width = 0;
    tex_height = 0;
    tex_n = 0;
}

bool CTexture::LoadTexture(std::string filename){
    tex_data = stbi_load(filename.c_str(), &tex_width, &tex_height, &tex_n, STBI_rgb_alpha);
	if (tex_data == nullptr) {
		printf ("Image cannot be loaded!\n");
        return false;
	}
    return true;
}

void CTexture::ActiveTexture(){
    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, idTexture);
}

void CTexture::CreateTextureObject(GLenum enum_opengl){
    glGenTextures(1, &idTexture);
    glBindTexture(GL_TEXTURE_2D, idTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, enum_opengl, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void CTexture::TextureBuffer(std::vector<glm::vec2> vertices_uv){
    glGenBuffers( 1, &idVBO_uv );
	glBindBuffer( GL_ARRAY_BUFFER, idVBO_uv );
	glBufferData( GL_ARRAY_BUFFER, vertices_uv.size() * sizeof(glm::vec2), &vertices_uv[0], GL_STATIC_DRAW );
	glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 2 );
};

void CTexture::DeleteBuffers(){
    glDeleteBuffers(1, &idVBO_uv);
    glDeleteTextures(1, &idTexture);
    stbi_image_free(tex_data);

    idVBO_uv = 0;
    idTexture = 0;
    tex_data = nullptr;

    tex_width = 0;
    tex_height = 0;
    tex_n = 0;
}
