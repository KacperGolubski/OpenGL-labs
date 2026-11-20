#include "CProgram.h"
GLuint LoadShader(GLuint mode, const char* filename);
void LinkAndValidateProgram(GLuint program);
#include <glm/gtc/type_ptr.hpp>
#include <vector>

CProgram::CProgram() {
    idProgram = 0;
}

void CProgram::CreateProgram() {
    idProgram = glCreateProgram();
}

void CProgram::UseProgram()
{
    glUseProgram(idProgram);
}

void CProgram::UnuseProgram()
{
    glUseProgram(0);
}

void CProgram::AttachShaderVertexShader(std::string filename)
{
    glAttachShader(idProgram, LoadShader(GL_VERTEX_SHADER, filename.c_str()));
}

void CProgram::AttachShaderFragmentShader(std::string filename)
{
    glAttachShader(idProgram, LoadShader(GL_FRAGMENT_SHADER, filename.c_str()));
}

void CProgram::DeleteProgram()
{
    glDeleteProgram(idProgram);
}

GLuint CProgram::GetIdProgram()
{
    return idProgram;
}

void CProgram::LinkProgram()
{
    LinkAndValidateProgram(idProgram);
}

void CProgram::SetFloat(const char *Str, float value)
{
    glUniform1f(glGetUniformLocation(idProgram, Str), value);
}
void CProgram::SetInt(const char *Str, int value)
{
    glUniform1i(glGetUniformLocation(idProgram, Str), value);
}
void CProgram::SetMat4(const char *Str, glm::mat4 matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(idProgram, Str), 1, GL_FALSE, glm::value_ptr(matrix));
}

void CProgram::SetVec3(const char *Str, glm::vec3 vector)
{
    glUniform3fv(glGetUniformLocation(idProgram, Str), 1, glm::value_ptr(vector));
}