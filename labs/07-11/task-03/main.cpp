
#include <iostream>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <random>
#include <cmath>
#include <algorithm>



// Okno aplikacji
const float pi = 3.14159265f;
int windowWidth = 600, windowHeight = 600;
const char *windowTitle = "Press space to switch scenes";
float rotation_alpha_z = 0.0f;
float scale_B1 = 0.9f;
constexpr int vertices_number_A = 16;
constexpr int vertices_number_B = 26;
float colors_A[3 * vertices_number_A];
float colors_B[3 * vertices_number_B];

// Osobne macierze dla każdej figury.
GLfloat Matrix_T_A[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
GLfloat Matrix_Scale_A[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
GLfloat Matrix_OZ_A[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
GLfloat Matrix_T_B1[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
GLfloat Matrix_Scale_B1[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
GLfloat Matrix_OZ_B1[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
GLfloat Matrix_T_B2[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
GLfloat Matrix_Scale_B2[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
GLfloat Matrix_OZ_B2[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

// Funkcje pomocnicze
#include "utilities.hpp"
// Plik ze współrzędnymi wierzchołków
#include "obiektA.h"
#include "obiektB.h"

// ---------------------------------------
// Identyfikatory obiektow OpenGL
GLuint idProgram;		// programu shaderowego
GLuint idVAO_A;			// VAO dla obiektu A
GLuint idVAO_B;			// VAO dla obiektu B
GLuint idVBO_coord_A;	// bufora na współrzędne kwadratów A
GLuint idVBO_color_A;	// bufora na kolory wierzchołków  A
GLuint idEBO_A;			// bufora na indeksy wierzchołków A
GLuint idVBO_coord_B;	// bufora na współrzędne kwadratów B
GLuint idVBO_color_B;	// bufora na kolory wierzchołków B
GLuint idEBO_B;			// bufora na indeksy wierzchołków B
GLint idTranslate;		// bufor na macierz przesunięcia
GLint idScale;			// bufor macierz skalowania
GLint idRotate_z;		// bufor macierz obrotu względem osi Z

// Generator kolorów z gradientem
void GradientColorsGenerator(float array[], int array_size) {
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_real_distribution<> random(0.0, 1.0);
	for(int i = 0; i < 3*(array_size); i++) {
		float randomValue = random(generator);
		array[i] = randomValue;
	}
}


void Scale(GLfloat matrix[4][4], float x, float y, float z) {
    matrix[0][0] = x;
    matrix[1][1] = y;
    matrix[2][2] = z;
}

void Translation(GLfloat matrix[4][4], float x, float y, float z) {
    matrix[0][3] = x;
    matrix[1][3] = y;
    matrix[2][3] = z;
}

void Rotation(GLfloat matrix[4][4], float angle) {
    matrix[0][0] = cos(angle);
    matrix[0][1] = -sin(angle);
    matrix[1][0] = sin(angle);
    matrix[1][1] = cos(angle);
}

void InitializeScene() {
	// Tworzymy VAO obiektu A
	glGenVertexArrays( 1, &idVAO_A);
	glBindVertexArray( idVAO_A);
	// Tworzymy bufor na współrzędne wierzchołków
	glGenBuffers( 1, &idVBO_coord_A );
	glBindBuffer( GL_ARRAY_BUFFER, idVBO_coord_A );
	glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat )*coordA.size(), coordA.data(), GL_STATIC_DRAW );
	// Ustawienie atrybutu o numerze 0 dla współrzędnych
	// ten sam numer ustawiony w vertex shaderze
	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 0 );

	// Tworzymy buffor na indeksy wierzchołków
	glGenBuffers( 1, &idEBO_A );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, idEBO_A );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*idxA.size(), idxA.data(), GL_STATIC_DRAW );

	// Tworzymy bufor na kolory wierzchołków bez gradientu
	glGenBuffers( 1, &idVBO_color_A);
	glBindBuffer( GL_ARRAY_BUFFER, idVBO_color_A );
	glBufferData( GL_ARRAY_BUFFER, sizeof(colors_A), colors_A, GL_STATIC_DRAW );
	// Ustawienie atrybutu o numerze 1 dla kolorów
	// ten sam numer ustawiony w vertex shaderze
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 1 );
	glBindVertexArray( 0 );

	glGenVertexArrays(1, &idVAO_B);
	glBindVertexArray( idVAO_B);

	glGenBuffers(1, &idVBO_coord_B);
	glBindBuffer(GL_ARRAY_BUFFER, idVBO_coord_B);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*coordB.size(), coordB.data(), GL_STATIC_DRAW );
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,NULL);
	glEnableVertexAttribArray(0);
	// Tworzymy buffor na indeksy wierzchołków
	glGenBuffers( 1, &idEBO_B );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, idEBO_B );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*idxB.size(), idxB.data(), GL_STATIC_DRAW );

	// Tworzymy bufor na kolory wierzchołków bez gradientu
	glGenBuffers( 1, &idVBO_color_B);
	glBindBuffer( GL_ARRAY_BUFFER, idVBO_color_B );
	glBufferData( GL_ARRAY_BUFFER, sizeof(colors_B), colors_B, GL_STATIC_DRAW );
	// Ustawienie atrybutu o numerze 1 dla kolorów
	// ten sam numer ustawiony w vertex shaderze
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 1 );
	glBindVertexArray( 0 );

}



void DrawScene() {
    glUseProgram(idProgram);

    glBindVertexArray(idVAO_A);
    glUniformMatrix4fv(idTranslate, 1, GL_TRUE, &Matrix_T_A[0][0]);
    glUniformMatrix4fv(idScale, 1, GL_TRUE, &Matrix_Scale_A[0][0]);
    glUniformMatrix4fv(idRotate_z, 1, GL_TRUE, &Matrix_OZ_A[0][0]);
    glDrawElements(GL_TRIANGLES, idxA.size(), GL_UNSIGNED_INT, NULL);

    glBindVertexArray(idVAO_B);
    glUniformMatrix4fv(idTranslate, 1, GL_TRUE, &Matrix_T_B1[0][0]);
    glUniformMatrix4fv(idScale, 1, GL_TRUE, &Matrix_Scale_B1[0][0]);
    glUniformMatrix4fv(idRotate_z, 1, GL_TRUE, &Matrix_OZ_B1[0][0]);
    glDrawElements(GL_TRIANGLES, idxB.size(), GL_UNSIGNED_INT, NULL);

    glUniformMatrix4fv(idTranslate, 1, GL_TRUE, &Matrix_T_B2[0][0]);
    glUniformMatrix4fv(idScale, 1, GL_TRUE, &Matrix_Scale_B2[0][0]);
    glUniformMatrix4fv(idRotate_z, 1, GL_TRUE, &Matrix_OZ_B2[0][0]);
    glDrawElements(GL_TRIANGLES, idxB.size(), GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}

void DisplayScene()
{
	glClear( GL_COLOR_BUFFER_BIT );
	DrawScene();
	glUseProgram( 0 );
}

// ---------------------------------------
void Initialize()
{
    Translation(Matrix_T_B1, -0.1f, 0.55f, 0.0f);
    Scale(Matrix_Scale_B1, scale_B1, scale_B1, 1.0f);
    Translation(Matrix_T_B2, -0.55f, -0.55f, 0.0f);
    Scale(Matrix_Scale_B2, 0.5f, 0.5f, 1.0f);
    InitializeScene();

	idProgram = glCreateProgram();
		glAttachShader( idProgram, LoadShader(GL_VERTEX_SHADER, "vertex.glsl"));
		glAttachShader( idProgram, LoadShader(GL_FRAGMENT_SHADER, "fragment.glsl"));
	glLinkProgram( idProgram );

	idTranslate = glGetUniformLocation(idProgram, "translate");
	idScale = glGetUniformLocation(idProgram, "scale");
	idRotate_z = glGetUniformLocation(idProgram, "rotate");

	glBindVertexArray(0);
	glClearColor( 0.7f, 0.7f, 0.7f, 1.0f );

}


// funkcja zwrotna do obsługi klawiatury
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch(key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
        case GLFW_KEY_W:
            scale_B1 += 0.1f;
            Scale(Matrix_Scale_B1, scale_B1, scale_B1, 1.0f);
            break;
        case GLFW_KEY_S:
            scale_B1 -= 0.1f;
            Scale(Matrix_Scale_B1, scale_B1, scale_B1, 1.0f);
            break;
        case GLFW_KEY_D:
            rotation_alpha_z += pi / 6;
            Rotation(Matrix_OZ_A, rotation_alpha_z);
            break;
        case GLFW_KEY_A:
            rotation_alpha_z -= pi / 6;
            Rotation(Matrix_OZ_A, rotation_alpha_z);
            break;
		}
	}
}



// funkcja zwrotna do obsługi zmiany rozmiaru framebuffera
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	windowWidth = width;
	windowHeight = height;

	// Ustawienie viewportu
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
}



// ---------------------------------------------------
int main( int argc, char *argv[] )
{
	// Generowanie danych i utworzenie okna aplikacji
	GradientColorsGenerator(colors_A, vertices_number_A);
	GradientColorsGenerator(colors_B, vertices_number_B);
	GLFWwindow* window = nullptr;
	Initialize_GLFW(window);


	// Inicjalizacja wszystkich scen
	Initialize();


	// Główna pętla
	while (!glfwWindowShouldClose(window))
	{
		// Obsługa zdarzeń
		glfwPollEvents();

		// Renderowanie sceny
		DisplayScene();

		// Podmiana buforów
		// wyświetlenie wyniku
		glfwSwapBuffers(window);
	}


	// Zwalnianie zasobów
	glDeleteProgram( idProgram );
	glDeleteBuffers(1, &idVBO_coord_A);
	glDeleteBuffers(1, &idVBO_color_A);
	glDeleteBuffers(1, &idEBO_A);
	glDeleteVertexArrays(1, &idVAO_A);
    glDeleteBuffers(1, &idVBO_coord_B);
    glDeleteBuffers(1, &idVBO_color_B);
    glDeleteBuffers(1, &idEBO_B);
    glDeleteVertexArrays(1, &idVAO_B);
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}
