#pragma once

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>

class CProgram {
    private:
        GLuint idProgram;

    public:
        explicit CProgram();
        void CreateProgram();
        void UseProgram();
        void UnuseProgram();
        void AttachShaderVertexShader(std::string filename);
        void AttachShaderFragmentShader(std::string filename);
        void DeleteProgram();
        GLuint GetIdProgram();
        void LinkProgram();
        void SetFloat(const char *Str, float value);
        void SetInt(const char *Str, int value);
        void SetMat4(const char *Str, glm::mat4 matrix);
        void SetVec3(const char *Str, glm::vec3 vector);
        
};