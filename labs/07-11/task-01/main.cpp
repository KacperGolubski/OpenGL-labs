// -------------------------------------------------
// Programowanie grafiki 3D w OpenGL / UG
// -------------------------------------------------
// Przyklad programu, ktory renderuje dane
// przy uzyciu tablicy indeksow.
// Nacisnij SPACE aby zmienic tryb.
// -------------------------------------------------
#include <iostream>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Okno aplikacji
int windowWidth = 600, windowHeight = 600;
const char *windowTitle = "OpenGL (nacisnij spacje) [WSAD]";


// Funkcje pomocnicze
#include "utilities.hpp"


// ---------------------------------------
// Identyfikatory obiektow

GLuint idProgram;
GLuint idVBO_coord;
GLuint idVBO_color;
GLuint idVBO_indices;
GLuint idVAO;

// ---------------------------------------
// Wspolrzedne wierzchokow
GLfloat triangles_coord[2*6] =
{
	-0.5f, -1.0f,
	 0.5f, -1.0f,
	 1.0f,  0.0f,
	 0.5f,  1.0f,
	-0.5f,  1.0f,
	-1.0f,  0.0f
};

// ---------------------------------------
// Kolory wierzcholkow
GLfloat triangles_color[3*6] =
{
	1.f, 0.f, 0.f,
	0.f, 1.f, 0.f,
	0.f, 0.f, 1.f,
	1.f, 1.f, 0.f,
	0.f, 1.f, 1.f,
	1.f, 0.f, 1.f
};

// ---------------------------------------
// Indeksy wierzcholkow
GLuint triangles_indices[3*4] =
{
	0, 1, 2,
	0, 2, 3,
	0, 3, 4,
	0, 4, 5
};

// ---------------------------------------
// NOWE :
// Zmiena do kontroli trybu renderingu:
// za pomoca tablicy indeksow badz nie (po staremu)
bool indeksowanieWierzcholkow = false;

// ---------------------------------------
void DisplayScene()
{
	glClear( GL_COLOR_BUFFER_BIT );

	glUseProgram( idProgram );
	glBindVertexArray( idVAO );


		// Bez tablicy wierzcholkow (po staremu)
		if (indeksowanieWierzcholkow == false)
		{
			glDrawArrays( GL_TRIANGLES, 0, 6 );
		}
		// Z tablica wierzcholkow
		else
		{
			glDrawElements( GL_TRIANGLES, 12, GL_UNSIGNED_INT, NULL );
		}


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
	LinkAndValidateProgram( idProgram );


	// 2. Vertex arrays
	glGenVertexArrays( 1, &idVAO );
	glBindVertexArray( idVAO );

		// a) BUFOR: Wspolrzedne wierzcholkow
		glGenBuffers( 1, &idVBO_coord );
		glBindBuffer( GL_ARRAY_BUFFER, idVBO_coord );
		glBufferData( GL_ARRAY_BUFFER, sizeof( triangles_coord ), triangles_coord, GL_STATIC_DRAW );
		glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, NULL );
		glEnableVertexAttribArray( 0 );

		// b) BUFOR: Kolor wierzcholkow
		glGenBuffers( 1, &idVBO_color );
		glBindBuffer( GL_ARRAY_BUFFER, idVBO_color  );
		glBufferData( GL_ARRAY_BUFFER, sizeof( triangles_color ), triangles_color, GL_STATIC_DRAW );
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL );
		glEnableVertexAttribArray( 1 );

		// ---------------------------------------
		// NOWE: Nowy bufor w ramach VAO potrzebny
		// do przechowywania tablicy indeksow
		// ---------------------------------------
		// c) BUFOR: Indeksy wierzcholkow
		glGenBuffers( 1, &idVBO_indices );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, idVBO_indices );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( triangles_indices ), triangles_indices, GL_STATIC_DRAW );

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

		case GLFW_KEY_SPACE:
			indeksowanieWierzcholkow = !indeksowanieWierzcholkow;
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
