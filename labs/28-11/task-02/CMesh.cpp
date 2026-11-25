#include "CMesh.h"
#include "objloader.hpp"
#include <glad/glad.h>



CMesh::CMesh(){
    idVAO = 0;
    idVBO_coord = 0;
    idVBO_color = 0;
    Matrix = glm::mat4(1.0f);
}

bool CMesh::LoadMesh(std::string filename){
    if(!loadOBJ(filename.c_str(), vertices, uvs, normals))
    {
        std::cerr << "Failed to load OBJ file: " << filename << std::endl;
        return false;
    }
    if (vertices.empty()) {
        std::cerr << "No vertices loaded from OBJ file: " << filename << std::endl;
        return false;
    }
    if (normals.empty()) {
        std::cerr << "No normals loaded from OBJ file: " << filename << std::endl;
        return false;
    }
    glGenVertexArrays(1, &idVAO);
    glGenBuffers( 1, &idVBO_coord );
    glGenBuffers( 1, &idVBO_color );
    return true;
}



void CMesh::DeleteBuffers() {
    glDeleteVertexArrays(1, &idVAO);
    glDeleteBuffers(1, &idVBO_coord);
    glDeleteBuffers(1, &idVBO_color);
}

void CMesh::DrawMesh() {
    glBindVertexArray(idVAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}

void CMesh::CoordBuffer(){
    glBindVertexArray(idVAO);
	glBindBuffer( GL_ARRAY_BUFFER, idVBO_coord );
	glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec3 ) * vertices.size(), &vertices[0], GL_STATIC_DRAW );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 0 );
}

void CMesh::ColorBuffer(std::vector <glm::vec3> color){
    glBindVertexArray(idVAO);
    glBindBuffer( GL_ARRAY_BUFFER, idVBO_color );
    glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec3 ) * color.size(), &color[0], GL_STATIC_DRAW );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL );
    glEnableVertexAttribArray( 1 );
}

void CMesh::SetMatrix(glm::mat4 matrix) {
    Matrix = matrix;
}

glm::mat4 CMesh::GetPositionMatrix() {
    return Matrix;
}

std::vector<glm::vec3> CMesh::GetVertices() {
    return vertices;
}

std::vector<glm::vec2> CMesh::GetUvs() {
    return uvs;
}

std::vector<glm::vec3> CMesh::GetNormals() {
    return normals;
}

