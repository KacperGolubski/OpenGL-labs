// -------------------------------------------------
// Programowanie grafiki 3D w OpenGL / UG
// -------------------------------------------------
// Przyklad wczytania pliku graficznego, utworzenia
// biektu tekstury w OpenGL oraz uzycia jej w
// potoku graficznym.
// -------------------------------------------------
// Szablon aplikacji w OpenGL korzystajacej
// z GLFW zamiast freeGLUT
//
// Kompilacja pod windowsem
// g++ plik.cpp glad.c -lglfw3 -lopengl32 -lgdi32
//
// Wymagana jest bibliteka glad
// https://glad.dav1d.de/
//
// Loader GLAD i aplikacja korzystaja z OpenGL 3.3 Core.
// ------------------------------------------------
#include <filesystem>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// -----------------------------------------
// NOWE: plik do obslugi plikow graficznych
// -----------------------------------------
# define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Okno aplikacji
int windowWidth = 600, windowHeight = 600;
const char *windowTitle = "OpenGL w GLFW (Tekstura)";

// Lokalne pliki naglowkowe
#include "utilities.hpp"


// Identyfikatory obiektow
GLuint idProgram;
GLuint idVAO;
GLuint idVBO_coord;
GLuint idVBO_color;
GLuint idVBO_uv;

// Macierze transformacji i rzutowania
glm::mat4 matProj = glm::mat4(1.0);
glm::mat4 matView = glm::mat4(1.0);
glm::mat4 matModel = glm::mat4(1.0);


// ---------------------------------------
// NOWE: identyfikatory obiektow tekstur
// ---------------------------------------
GLuint idTextures[3];

// ---------------------------------------
// Dane kwadratu
// ---------------------------------------
GLfloat vertices_coord[] =
{
	-1.0f, -1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f,
	1.0f,  -1.0f, 0.0f,
	1.0f, 1.0f, 0.0f

};


GLfloat vertices_color[] =
{
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
};


GLfloat vertices_uv[] =
{
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f

};




// ---------------------------------------
void DisplayScene()
{
	// Czyszczenie bufora koloru i glebokosci
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Obliczanie macierzy widoku
	matView = UpdateViewMatrix();

	// Wlaczenie programu
	glUseProgram( idProgram );

		// Przekazanie macierzy
		glUniformMatrix4fv( glGetUniformLocation(idProgram, "matProj"), 1, GL_FALSE, glm::value_ptr(matProj));
		glUniformMatrix4fv( glGetUniformLocation(idProgram, "matView"), 1, GL_FALSE, glm::value_ptr(matView));
		glUniformMatrix4fv( glGetUniformLocation(idProgram, "matModel"), 1, GL_FALSE, glm::value_ptr(matModel));

		// -----------------------------------------------
		// NOWE: Aktywowanie jednostki teksturujacej nr 0,
		// przypisanie do niej obiektu tekstury oraz
		// polaczenie j.t. z uchwytem tekstury w shaderze
		// -----------------------------------------------
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, idTextures[0]);
		glUniform1i(glGetUniformLocation(idProgram, "tex0"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, idTextures[1]);
		glUniform1i(glGetUniformLocation(idProgram, "tex1"), 1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, idTextures[2]);
		glUniform1i(glGetUniformLocation(idProgram, "tex2"), 2);



		// Rendering
		glBindVertexArray( idVAO );
		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
		glBindVertexArray( 0 );


	// Wylaczanie
	glUseProgram( 0 );
}

// ---------------------------------------
void Initialize(const std::filesystem::path& assetDirectory)
{

	// Obliczanie macierzy rzutowania perspektywicznego
	// za pierwszym razem (po utworzeniu okna aplikacji)
	if (windowHeight != 0)
		matProj = glm::perspective(glm::radians(80.0f), windowWidth/(float)windowHeight, 0.1f, 50.0f);


	// Ustawienia maszyny OpenGL
	glEnable(GL_DEPTH_TEST);
	glClearColor( 0.7f, 0.7f, 0.7f, 1.0f );


	// VAO
	glGenVertexArrays( 1, &idVAO );
	glBindVertexArray( idVAO );

		// Bufor na wspolrzedne wierzcholkow
		glGenBuffers( 1, &idVBO_coord );
		glBindBuffer( GL_ARRAY_BUFFER, idVBO_coord );
		glBufferData( GL_ARRAY_BUFFER, sizeof(vertices_coord), &vertices_coord[0], GL_STATIC_DRAW );
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
		glEnableVertexAttribArray( 0 );

		// Bufor na kolory
		glGenBuffers( 1, &idVBO_color );
		glBindBuffer( GL_ARRAY_BUFFER, idVBO_color );
		glBufferData( GL_ARRAY_BUFFER, sizeof(vertices_color), &vertices_color[0], GL_STATIC_DRAW );
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL );
		glEnableVertexAttribArray( 1 );

		// -------------------------------------
		// NOWE: Bufor na wspolrzedne tekstury
		// -------------------------------------
		glGenBuffers( 1, &idVBO_uv );
		glBindBuffer( GL_ARRAY_BUFFER, idVBO_uv );
		glBufferData( GL_ARRAY_BUFFER, sizeof(vertices_uv), &vertices_uv[0], GL_STATIC_DRAW );
		glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, NULL );
		glEnableVertexAttribArray( 2 );

	// ------------------------------------
	// NOWE: Wczytanie pliku tekstury
	// ------------------------------------
	int tex_width[3], tex_height[3], tex_n[3];
	unsigned char *tex_data[3];

	// tylko raz w calym programie
	stbi_set_flip_vertically_on_load(true);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// wczytywanie pliku graficznego
	tex_data[0] = stbi_load ((assetDirectory / "uvtemplate.jpg").string().c_str(), &tex_width[0] , &tex_height[0] , &tex_n[0], 0);
	if ( tex_data[0] == NULL) {
		printf ("Image cannot be loaded!\n");
	}

	tex_data[1] = stbi_load ((assetDirectory / "pikachu.png").string().c_str(), &tex_width[1], &tex_height[1], &tex_n[1], 4);
	if ( tex_data[1] == NULL) {
		printf ("Image cannot be loaded!\n");
	}

	tex_data[2] = stbi_load ((assetDirectory / "pokemon.png").string().c_str(), &tex_width[2], &tex_height[2], &tex_n[2], 4);
	if ( tex_data[2] == NULL) {
		printf ("Image cannot be loaded!\n");
	}

	

	// ------------------------------------
	// NOWE: Utworzenie obiektu tekstury
	// ------------------------------------
	glGenTextures(3, idTextures);
	glBindTexture(GL_TEXTURE_2D, idTextures[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width[0], tex_height[0], 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, idTextures[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width[1], tex_height[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, idTextures[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width[2], tex_height[2], 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);




	// Potok
	idProgram = glCreateProgram();
	glAttachShader( idProgram, LoadShader(GL_VERTEX_SHADER, (assetDirectory / "vertex.glsl").string().c_str()));
	glAttachShader( idProgram, LoadShader(GL_FRAGMENT_SHADER, (assetDirectory / "fragment.glsl").string().c_str()));
	LinkAndValidateProgram( idProgram );

	glBindVertexArray( 0 );
}


// ---------------------------------------------------
// funkcja zwrotna do obslugi zmiany rozmiaru framebuffera
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	windowWidth = width;
	windowHeight = height;

	// Viewport
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);

	// Macierz rzutowania perspektywicznego
	matProj = glm::perspective(glm::radians(70.0f), width/(float)height, 0.1f, 20.0f );
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
int main( int argc, char *argv[] )
{
	// Kontekst i okno aplikacji
    GLFWwindow* window = nullptr;
	Initialize_GLFW(window);


	// Inicjalizacja
	Initialize(std::filesystem::canonical(argv[0]).parent_path());


	// Glowna petla
	while (!glfwWindowShouldClose(window))
	{
		// Obsluga zdarzen
		glfwPollEvents();

		// Render Sceny
		DisplayScene();

		glfwSwapBuffers(window);
	}

	// Czyszczenie
	glDeleteProgram( idProgram );
	glDeleteVertexArrays( 1, &idVBO_coord );
	glDeleteVertexArrays( 1, &idVBO_color );
	glDeleteVertexArrays( 1, &idVBO_uv );
	glDeleteVertexArrays( 1, &idVAO );
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
