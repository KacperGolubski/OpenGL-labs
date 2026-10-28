
#include <iostream>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <random>
// Okno aplikacji
int windowWidth = 600, windowHeight = 600;
const char *windowTitle = "What is this?";


// Funkcje pomocnicze
#include "utilities.hpp"
#include "co-to-jest.h"


// ---------------------------------------
// Identyfikatory obiektow

GLuint idProgram;
GLuint idVBO_coord;
GLuint idVBO_color;
GLuint idVBO_indices;
GLuint idVAO;


GLfloat color[97*3];

void ColorGenerator(GLfloat array[], int n) {
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_real_distribution<float> distribution(0.0, 1.0);

	for(int i = 0; i < n; i++) {
		float random_Value = distribution(generator);
		array[i] = random_Value;
	}
}

// ---------------------------------------
void DisplayScene()
{
	glClear( GL_COLOR_BUFFER_BIT );

	glUseProgram( idProgram );
	glBindVertexArray( idVAO );
	glDrawElements(GL_TRIANGLES, 137*3, GL_UNSIGNED_INT, NULL);

	glBindVertexArray( 0 );
	glUseProgram( 0 );
}


// ---------------------------------------
void Initialize()
{

	glClearColor( 0.6f, 0.6f, 0.6f, 1.0f );


	// 1. Program i shadery
	idProgram = glCreateProgram();
	glAttachShader( idProgram, LoadShader(GL_VERTEX_SHADER, "vertex.glsl"));
	glAttachShader( idProgram, LoadShader(GL_FRAGMENT_SHADER, "fragment.glsl"));
	glLinkProgram( idProgram );


	// 2. Vertex arrays
	glGenVertexArrays( 1, &idVAO );
	glBindVertexArray( idVAO );

		// a) BUFOR: Wspolrzedne wierzcholkow
		glGenBuffers( 1, &idVBO_coord );
		glBindBuffer( GL_ARRAY_BUFFER, idVBO_coord );
		glBufferData( GL_ARRAY_BUFFER, sizeof( coord ), coord, GL_STATIC_DRAW );
		glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, NULL );
		glEnableVertexAttribArray( 0 );

		// b) BUFOR: Kolor wierzcholkow
		glGenBuffers( 1, &idVBO_color );
		glBindBuffer( GL_ARRAY_BUFFER, idVBO_color  );
		glBufferData( GL_ARRAY_BUFFER, sizeof( color ), color, GL_STATIC_DRAW );
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL );
		glEnableVertexAttribArray( 1 );

		// ---------------------------------------
		// NOWE: Nowy bufor w ramach VAO potrzebny
		// do przechowywania tablicy indeksow
		// ---------------------------------------
		// c) BUFOR: Indeksy wierzcholkow
		glGenBuffers( 1, &idVBO_indices );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, idVBO_indices );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

	glBindVertexArray( 0 );

}

// ---------------------------------------------------
// funkcja zwrotna do obslugi klawiatury
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch(key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;

		default:
			printf("Nacisnieto klawisz %d \n", key);
			break;
		}
	}
}


// ---------------------------------------------------
// funkcja zwrotna do obslugi zmiany rozmiaru framebuffera
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
	ColorGenerator(color, std::size(color));

	// Kontekst i okno aplikacji
	GLFWwindow* window = nullptr;
	Initialize_GLFW(window);


	// Inicjalizacja naszej sceny
	Initialize();


	// Glowna petla
	while (!glfwWindowShouldClose(window))
	{
		// Obsluga zdarzen
		glfwPollEvents();

		// Render sceny
		DisplayScene();

		glfwSwapBuffers(window);
	}

	// Cleaning
	glDeleteProgram( idProgram );
	glDeleteBuffers( 1, &idVBO_coord);
	glDeleteBuffers( 1, &idVBO_color );
	glDeleteBuffers( 1, &idVBO_indices );
	glDeleteVertexArrays( 1, &idVAO );

	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}
