// -------------------------------------------------
// Programowanie grafiki 3D w OpenGL / UG
// -------------------------------------------------
// Przyklad programu, w ktorym wczytujemy dane
// modelu 3D z pliku w formacie OBJ.
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
// -------------------------------------------------
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

// Bibliteka GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Okno aplikacji
int windowWidth = 600, windowHeight = 600;
const char *windowTitle = "OpenGL w GLFW (obrot klawiszami WSAD oraz mysza)";

// Funkcje pomocnicze
#include "utilities.hpp"

// Klasa CProgram do obslugi potoku OpenGL
#include "CProgram.h"

// Klasa CMesh do obslugi obiektow 3D wczytanych z plikow OBJ
#include "CMesh.h"

// Identyfikator obiektow programu
CProgram program;

// Identyfikatory obiektow VAO i VBO dla obiektu 3D wczytanego z pliku OBJ
CMesh Monkey;
CMesh Cone;

// Macierze transformacji i rzutowania
glm::mat4 matProj;
glm::mat4 matView;
glm::mat4 matModelBase;	// wspolna dla wszystkich obiektow


// Zmienne do kontroli obrotu obiektu
const float pi = 3.14;
float angleY = 0.0f;
float angleX = 0.0f;
float distance = -10.0f;
float cone_1_angle = 0.0f;
float cone_2_angle = pi/2;
float cone_3_angle = pi;
float cone_4_angle = 3*pi/2;

// Katy obrotu stozkow wzgledem swojej osi
std::vector <float> cone_rotation_angles = {
	0.0f, pi/2, pi, 3*pi/2
};

// Pozycje stozkow w scenie
std::vector<glm::vec3> ConePositions = {
	glm::vec3(0.0f, 3.0f, 0.0f),
	glm::vec3(-3.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, -3.0f, 0.0f),
	glm::vec3(3.0f, 0.0f, 0.0f)
};


// ---------------------------------------
void DisplayScene()
{
	// Zmienna czasu
	float time_anim = static_cast<float>(glfwGetTime());

	// Czyszczenie bufora koloru
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


	// Obliczanie macierzy widoku (pozycja kamery)
	matView = glm::mat4(1.0);
	matView = glm::translate(matView, glm::vec3(0.0, 0.0, -2.5));

	// Wspolne przesuniecie i obrot sceny
	matModelBase = glm::mat4(1.0f);
	matModelBase = glm::translate(matModelBase, glm::vec3(0.0f, 0.0f, distance));
	matModelBase = glm::rotate(matModelBase, angleX, glm::vec3(1.0f, 0.0f, 0.0f));
	matModelBase = glm::rotate(matModelBase, angleY, glm::vec3(0.0f, 1.0f, 0.0f));
	
	// Macierze ustawienia pozycji obiektow w scenie
	glm::mat4 Monkey_position = glm::translate(matModelBase, glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 Cone_position = glm::translate(matModelBase, glm::vec3(0.0f, 0.0f, 0.0f));

	// Macierze skalowania obiektow w scenie
	glm::mat4 MonkeyScale = glm::scale(Monkey_position, glm::vec3(1.0f, 1.0f, 1.0f));

	// Macierz obrotu malpki w scenie
	glm::mat4 MonkeyAnimation = glm::rotate(MonkeyScale, 2.0f * time_anim, glm::vec3(0.0f, 1.0f, 0.0f));




	// Wlaczenie potoku
	program.UseProgram();

		// Przekazujemy macierze modelu, widoku i rzutowania do potoku
		program.SetMat4("matProj", matProj);
		program.SetMat4("matView", matView);

		// Uruchamianie potoku (rendering)
		Monkey.SetMatrix(MonkeyAnimation);
		program.SetMat4("matModel", Monkey.GetPositionMatrix());
		Monkey.DrawMesh();
		

		for(int i = 0; i < 4; i++){
			glm::mat4 ConeMatrix = glm::rotate(matModelBase, 0.5f * time_anim, glm::vec3(0.0f, 0.0f, 1.0f));
			ConeMatrix = glm::translate(ConeMatrix, ConePositions[i]);
			ConeMatrix = glm::rotate(ConeMatrix, cone_rotation_angles[i], glm::vec3(0.0f, 0.0f, 1.0f));
			ConeMatrix = glm::scale(ConeMatrix, glm::vec3(1.0f, 2.0f, 1.0f));
			Cone.SetMatrix(ConeMatrix);
			program.SetMat4("matModel", Cone.GetPositionMatrix());
			Cone.DrawMesh();
		}

		glBindVertexArray( 0 );


	// Wylaczanie potoku
	program.UnuseProgram();
}

// ---------------------------------------
void Initialize()
{

	// Ustawianie koloru, ktorym bedzie czyszczony bufor koloru
	glClearColor( 0.5f, 0.5f, 0.5f, 1.0f );
	glEnable(GL_DEPTH_TEST);

	// Obliczanie macierzy rzutowania perspektywicznego
	// za pierwszym razem (po utworzeniu okna aplikacji)
	if (windowHeight != 0)
		matProj = glm::perspective(glm::radians(80.0f), windowWidth/(float)windowHeight, 0.1f, 100.0f);



	// Wczytywanie danych z plikow OBJ

	if (!Monkey.LoadMesh("monkey.obj")) {
		std::cerr << "Failed to load monkey.obj" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (!Cone.LoadMesh("cone.obj")) {
		std::cerr << "Failed to load cone.obj" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Potok
	program.CreateProgram();
	program.AttachShaderVertexShader("vertex.glsl");
	program.AttachShaderFragmentShader("fragment.glsl");

	// Buffory dla malpki
	Monkey.CoordBuffer();
	Monkey.ColorBuffer(Monkey.GetNormals());
	program.LinkProgram();

	// Buffory dla stozka
	Cone.CoordBuffer();
	Cone.ColorBuffer(Cone.GetNormals());
	program.LinkProgram();

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

    // -------------------------------------------------------
	// NOWE! Obliczanie macierzy rzutowania perspektywicznego
	// Dlaczego akurat tutaj?
	// -------------------------------------------------------
	matProj = glm::perspective(glm::radians(70.0f), width/(float)height, 0.1f, 100.0f );

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

		case GLFW_KEY_W:
			angleX += 0.1f;
			break;

		case GLFW_KEY_S:
			angleX -= 0.1f;
			break;

		case GLFW_KEY_D:
			angleY += 0.1f;
			break;

		case GLFW_KEY_A:
			angleY -= 0.1f;
			break;
		case GLFW_KEY_EQUAL:
			distance += 0.1f;
			break;
		case GLFW_KEY_MINUS:
			distance -= 0.1f;
			break;
		default:
			printf("Nacisnieto klawisz %d \n", key);
			break;
		}
	}
}

// ---------------------------------------------------
// funkcja zwrotna do obslugi scrolla myszy
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	 distance += yoffset;
}

// --------------------------------------------------------------
// funkcja zwrotna do obslugi ruchu kursora myszy
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (__mouse_press && __mouse_button == GLFW_MOUSE_BUTTON_LEFT)
	{
		angleX += (ypos - __mouse_buttonY)/(float)windowWidth;
		angleY += (xpos - __mouse_buttonX)/(float)windowHeight;
        __mouse_buttonX = xpos;
        __mouse_buttonY = ypos;
	}
}


// ---------------------------------------------------
int main( int argc, char *argv[] )
{
	// Kontekst i okno aplikacji
    GLFWwindow* window = nullptr;
	Initialize_GLFW(window);


	// Inicjalizacja sceny
	Initialize();


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
	program.DeleteProgram();
	Monkey.DeleteBuffers();
	Cone.DeleteBuffers();
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
