
#include <iostream>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <random>



// Okno aplikacji
const float pi = 3.14159265f;
int windowWidth = 600, windowHeight = 600;
const char *windowTitle = "Press space to switch scenes";
constexpr int N = 200; // Liczba losowo generowanych kwadratów
int scene = 0; // Obecna scena
float offset_x = 0.0 , offset_y = 0.0; // Offset do zmiany położenia domku
float rotation_alpha_x = 0;
float rotation_alpha_y = 0;
float rotation_alpha_z = 0;

GLfloat Matrix_Rotation [4][4] = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

GLfloat* ptrMatrix_Rotation = &Matrix_Rotation[0][0];


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

GLfloat Matrix_OX [4][4] = {
	1, 0, 0, 0,
	0, cos(rotation_alpha_x), -sin(rotation_alpha_x), 0,
	0, sin(rotation_alpha_x), cos(rotation_alpha_x), 0,
	0, 0, 0, 1
};

GLfloat Matrix_OY [4][4] = {
	cos(rotation_alpha_y), 0, sin(rotation_alpha_y), 0,
	0, 1, 0, 0,
	-sin(rotation_alpha_y), 0, cos(rotation_alpha_y), 0,
	0, 0, 0, 1
};

GLfloat Matrix_OZ [4][4] = {
	cos(rotation_alpha_z), -sin(rotation_alpha_z), 0, 0,
	sin(rotation_alpha_z), cos(rotation_alpha_z), 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};


// Generator współrzędnych wierzchołków kwadratów
void VerticesGenerator(float array[]){
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
	glUniformMatrix4fv( glGetUniformLocation(idProgramHouse, "Matrix_Rotation"), 1,GL_TRUE, ptrMatrix_Rotation);
	glDrawArrays(GL_TRIANGLES, 0, NUMBER_OF_VERTICES);
	glBindVertexArray( 0 );
}

void Rotation(char rotation_operator, float rotation_alpha) {
	switch (rotation_operator) {
		case 'x':
			Matrix_OX[1][1] = cos(rotation_alpha);
			Matrix_OX[1][2] = -sin(rotation_alpha);
			Matrix_OX[2][1] = sin(rotation_alpha);
			Matrix_OX[2][2] = cos(rotation_alpha);
			break;
		case 'y':
			Matrix_OY[0][0] = cos(rotation_alpha);
			Matrix_OY[0][2] = sin(rotation_alpha);
			Matrix_OY[2][0] = -sin(rotation_alpha);
			Matrix_OY[2][2] = cos(rotation_alpha);
			break;
		case 'z':
			Matrix_OZ[0][0] = cos(rotation_alpha);
			Matrix_OZ[0][1] = -sin(rotation_alpha);
			Matrix_OZ[1][0] = sin(rotation_alpha);
			Matrix_OZ[1][1] = cos(rotation_alpha);
			break;
	}
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
			rotation_alpha_x += pi/6 ;
			Rotation('x', rotation_alpha_x);
			ptrMatrix_Rotation = &Matrix_OX[0][0];
			break;
		case GLFW_KEY_S:
			rotation_alpha_x -= pi/6;
			Rotation('x', rotation_alpha_x);
			ptrMatrix_Rotation = &Matrix_OX[0][0];
			break;
		case GLFW_KEY_D:
			rotation_alpha_y += pi/6 ;
			Rotation('y', rotation_alpha_y);
			ptrMatrix_Rotation = &Matrix_OY[0][0];
			break;
		case GLFW_KEY_A:
			rotation_alpha_y -= pi/6;
			Rotation('y', rotation_alpha_y);
			ptrMatrix_Rotation = &Matrix_OY[0][0];
			break;
		case GLFW_KEY_Q:
			rotation_alpha_z += pi/6 ;
			Rotation('z', rotation_alpha_z);
			ptrMatrix_Rotation = &Matrix_OZ[0][0];
			break;
		case GLFW_KEY_E:
			rotation_alpha_z -= pi/6;
			Rotation('z', rotation_alpha_z);
			ptrMatrix_Rotation = &Matrix_OZ[0][0];
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
	VerticesGenerator(triangles);
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
