#pragma once

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <vector>

class CMesh{
    private:
        GLuint idVAO;
        GLuint idVBO_coord;
        GLuint idVBO_color;
        std::string filename;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;
        glm::mat4 Matrix;
    public:
        explicit CMesh();
        bool LoadMesh(std::string filename);
        void CoordBuffer();
        void ColorBuffer(std::vector <glm::vec3> color);
        void DrawMesh();
        void DeleteBuffers();
        void SetMatrix(glm::mat4 matrix);
        glm::mat4 GetPositionMatrix();
        std::vector<glm::vec3> GetVertices();
        std::vector<glm::vec2> GetUvs();
        std::vector<glm::vec3> GetNormals();
};
