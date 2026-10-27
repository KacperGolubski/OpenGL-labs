
#include <iostream>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <random>



// Okno aplikacji
int windowWidth = 600, windowHeight = 600;
const char *windowTitle = "Press space to switch scenes";
constexpr int N = 200; // Liczba losowo generowanych kwadratów
int scene = 0; // Obecna scena
float offset_x = 0.0 , offset_y = 0.0; // Offset do zmiany położenia domku


// Funkcje pomocnicze
#include "utilities.hpp"
// Plik ze współrzędnymi wierzchołków domku
#include "house.h"


// ---------------------------------------
// Identyfikatory obiektow OpenGL
GLuint idProgram;	// programu shaderowego dla kwadratów
GLuint idProgramHouse; // programu shaderowego dla domku
GLuint idVAO_solid;		// VAO kwadratów bez gradientu
GLuint idVAO_gradient;	// VAO kwadratów z gradientem
GLuint idVAO_house;		// VAO domku
GLuint idVBO_coord;		// bufora na współrzędne kwadratów
GLuint idVBO_house_coord; // bufora na współrzędne domku
GLuint idVBO_solid_color; // bufora na kolory wierzchołków bez gradientu
GLuint idVBO_gradient_color; // bufora na kolory wierzchołków z gradientem


GLfloat triangles[8*N]; // tablica współrzędnych wierzchołków kwadratów
GLfloat solid_colors[12*N]; // tablica kolorów wierzchołków bez gradientu
GLfloat gradient_colors[12*N]; // tablica kolorów wierzchołków z gradientem


// Generator współrzędnych wierzchołków kwadratów
void VerticiesGenerator(float array[]){
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_real_distribution<> random_XY(-0.9, 0.9);
	std::uniform_real_distribution<> random_size(0.01, 0.1);
	for(int i = 0; i < 8*N; i+=8){
		float randomX = random_XY(generator);
		float randomY = random_XY(generator);
		float randomSize = random_size(generator);
		array[i + 0] = -randomSize + randomX;
		array[i + 1] = -randomSize + randomY;
		array[i + 2] = -randomSize + randomX;
		array[i + 3] =  randomSize + randomY;
		array[i + 4] =  randomSize + randomX;
		array[i + 5] = -randomSize + randomY;
		array[i + 6] =  randomSize + randomX;
		array[i + 7] =  randomSize + randomY;
	}
}

// Generator kolorów bez gradientu
void SolidColorsGenerator(float array[]){
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_real_distribution<> random(0.0, 1.0);
	for(int i = 0; i < 12*N; i+=12){
		float randomR = random(generator);
		float randomG = random(generator);
		float randomB = random(generator);
		array[i + 0] = randomR;
		array[i + 1] = randomG;
		array[i + 2] = randomB;
		array[i + 3] = randomR;
		array[i + 4] = randomG;
		array[i + 5] = randomB;
		array[i + 6] = randomR;
		array[i + 7] = randomG;
		array[i + 8] = randomB;
		array[i + 9] = randomR;
		array[i + 10] = randomG;
		array[i + 11] = randomB;
	}
}

// Generator kolorów z gradientem
void GradientColorsGenerator(float array[]) {
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_real_distribution<> random(0.0, 1.0);
	for(int i = 0; i < 12*N; i++) {
		float randomValue = random(generator);
		array[i] = randomValue;
	}
}

void InitializeScene1() {
	// Tworzymy VAO kwadratów bez gradientu
	glGenVertexArrays( 1, &idVAO_solid );
	glBindVertexArray( idVAO_solid );
	// Tworzymy bufor na współrzędne wierzchołków
	glGenBuffers( 1, &idVBO_coord );
	glBindBuffer( GL_ARRAY_BUFFER, idVBO_coord );
	glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * 8 *N, triangles, GL_STATIC_DRAW );
	// Ustawienie atrybutu o numerze 0 dla współrzędnych
	// ten sam numer ustawiony w vertex shaderze
	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 0 );

	// Tworzymy bufor na kolory wierzchołków bez gradientu
	glGenBuffers( 1, &idVBO_solid_color );
	glBindBuffer( GL_ARRAY_BUFFER, idVBO_solid_color );
	glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * 4*3 * N, solid_colors, GL_STATIC_DRAW );
	// Ustawienie atrybutu o numerze 1 dla kolorów
	// ten sam numer ustawiony w vertex shaderze
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 1 );

	glBindVertexArray( 0 );
}

void InitializeScene2() {
	// Tworzymy VAO kwadratów z gradientem
	glGenVertexArrays( 1, &idVAO_gradient );
	glBindVertexArray( idVAO_gradient );
	// Wykorzystujemy bufor współrzędnych z pierwszej sceny
	glBindBuffer( GL_ARRAY_BUFFER, idVBO_coord );
	// Ustawienie atrybutu o numerze 0 dla współrzędnych
	// ten sam numer ustawiony w vertex shaderze
	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 0 );

	// Tworzymy bufor na kolory wierzchołków z gradientem
	glGenBuffers( 1, &idVBO_gradient_color );
	glBindBuffer( GL_ARRAY_BUFFER, idVBO_gradient_color );
	glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * 4*3 * N, gradient_colors, GL_STATIC_DRAW );
	// Ustawienie atrybutu o numerze 1 dla kolorów
	// ten sam numer ustawiony w vertex shaderze
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 1 );

	glBindVertexArray( 0 );
}


void InitializeScene3() {
	// Tworzymy VAO domku
	glGenVertexArrays(1, &idVAO_house );
	glBindVertexArray( idVAO_house );
	// Tworzymy bufor na współrzędne wierzchołków domku
	glGenBuffers(1, &idVBO_house_coord);
	glBindBuffer( GL_ARRAY_BUFFER, idVBO_house_coord );
	glBufferData(GL_ARRAY_BUFFER, sizeof( Mesh_Vertices ), Mesh_Vertices, GL_STATIC_DRAW );
	// Ustawienie atrybutu o numerze 0 dla współrzędnych
	// ten sam numer ustawiony w vertex shaderze
	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 0 );
	glBindVertexArray( 0 );
}

void DrawScene1() {
	glUseProgram( idProgram );
	glBindVertexArray( idVAO_solid );
	for (int i = 0; i < N; i++) {
		glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
	}
	glBindVertexArray( 0 );
}

void DrawScene2() {
	glUseProgram( idProgram );
	glBindVertexArray( idVAO_gradient );
	for (int i = 0; i < N; i++) {
		glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
	}
	glBindVertexArray( 0 );
}

void DrawScene3() {
	glUseProgram( idProgramHouse);
	glBindVertexArray( idVAO_house);
	glUniform1f( glGetUniformLocation(idProgramHouse, "offset_x"), offset_x );
	glUniform1f( glGetUniformLocation(idProgramHouse, "offset_y"), offset_y );
	glDrawArrays(GL_TRIANGLES, 0, NUMBER_OF_VERTICES);
	glBindVertexArray( 0 );
}

void DisplayScene()
{
	glClear( GL_COLOR_BUFFER_BIT );
	switch(scene) {
		case 0:
			DrawScene1();
			break;
		case 1:
			DrawScene2();
			break;
		case 2:
			DrawScene3();
			break;
	}

	glUseProgram( 0 );
}

// ---------------------------------------
void Initialize()
{
	InitializeScene1();
	InitializeScene2();
	InitializeScene3();

	idProgram = glCreateProgram();
		glAttachShader( idProgram, LoadShader(GL_VERTEX_SHADER, "vertex.glsl"));
		glAttachShader( idProgram, LoadShader(GL_FRAGMENT_SHADER, "fragment.glsl"));
	glLinkProgram( idProgram );


	idProgramHouse = glCreateProgram();
		glAttachShader( idProgramHouse, LoadShader(GL_VERTEX_SHADER, "vertex_house.glsl"));
		glAttachShader( idProgramHouse, LoadShader(GL_FRAGMENT_SHADER, "fragment_house.glsl"));
	glLinkProgram( idProgramHouse );


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

		case GLFW_KEY_SPACE:
			scene++;
			scene = scene % 3;
			break;
		case GLFW_KEY_W:
			offset_y += 0.1f;
			break;
		case GLFW_KEY_S:
			offset_y -= 0.1f;
			break;
		case GLFW_KEY_A:
			offset_x -= 0.1f;
			break;
		case GLFW_KEY_D:
			offset_x += 0.1f;
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
	VerticiesGenerator(triangles);
	SolidColorsGenerator(solid_colors);
	GradientColorsGenerator(gradient_colors);
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
	glDeleteProgram( idProgramHouse );
	glDeleteBuffers(1, &idVBO_coord);
	glDeleteBuffers(1, &idVBO_house_coord);
	glDeleteBuffers(1, &idVBO_solid_color);
	glDeleteBuffers(1, &idVBO_gradient_color);
	glDeleteVertexArrays(1, &idVAO_house);
	glDeleteVertexArrays(1, &idVAO_solid);
	glDeleteVertexArrays(1, &idVAO_gradient);
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}
