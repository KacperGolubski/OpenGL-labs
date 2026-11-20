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

#include <filesystem>
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
CMesh rock;
CMesh palm;
CMesh ground;
CMesh bark;
CMesh grass;

// Macierze transformacji i rzutowania
glm::mat4 matProj;
glm::mat4 matView;
glm::mat4 matModelBase;	// wspolna dla wszystkich obiektow

// Pozycje trawy w scenie
std::vector<glm::vec3> GrassPositions = {
	{-1.60f, 0.0f, -1.30f},
	{-0.70f, 0.0f, -1.80f},
	{ 0.30f, 0.0f, -1.40f},
	{ 1.50f, 0.0f, -1.10f},
	{ 1.80f, 0.0f,  0.10f},
	{ 0.90f, 0.0f,  1.10f},
	{-0.20f, 0.0f,  1.60f},
	{-1.30f, 0.0f,  0.90f},
	{-0.60f, 0.0f,  0.10f},
	{ 0.50f, 0.0f, -0.30f},
	{ 1.20f, 0.0f,  0.40f},
	{ 2.20f, 0.0f,  0.90f},
	{ 2.70f, 0.0f,  1.90f},
	{ 1.60f, 0.0f,  2.40f},
	{ 0.50f, 0.0f,  2.70f},
	{-0.60f, 0.0f,  2.20f},
	{-1.70f, 0.0f,  2.50f},
	{-2.60f, 0.0f,  1.60f},
	{-2.30f, 0.0f,  0.40f},
	{-2.90f, 0.0f, -0.70f},
	{-2.20f, 0.0f, -1.70f},
	{-1.10f, 0.0f, -2.50f},
	{ 0.20f, 0.0f, -2.80f},
	{ 1.40f, 0.0f, -2.40f},
	{ 2.40f, 0.0f, -1.70f},
	{ 3.20f, 0.0f, -0.70f},
	{ 3.70f, 0.0f,  0.60f},
	{ 3.35f, 0.0f,  1.70f},
	{ 3.90f, 0.0f,  2.80f},
	{ 2.65f, 0.0f,  3.40f},
	{ 1.40f, 0.0f,  3.15f},
	{ 0.30f, 0.0f,  3.80f},
	{-0.95f, 0.0f,  3.45f},
	{-2.10f, 0.0f,  3.65f},
	{-3.15f, 0.0f,  3.00f},
	{-3.80f, 0.0f,  1.90f},
	{-3.45f, 0.0f,  0.75f},
	{-4.05f, 0.0f, -0.40f},
	{-3.55f, 0.0f, -1.60f},
	{-3.20f, 0.0f, -2.80f},
	{-2.10f, 0.0f, -3.55f},
	{-0.90f, 0.0f, -3.20f},
	{ 0.35f, 0.0f, -3.85f},
	{ 1.55f, 0.0f, -3.40f},
	{ 2.70f, 0.0f, -3.65f}
};

// Zmienne do kontroli obrotu obiektu
float angleY = -1.0f;
float angleX = 0.5f;
float distance = -10.0f;


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
	glm::mat4 GroundPosition_1 = glm::translate(matModelBase, glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 RockPosition_1 = glm::translate(matModelBase, glm::vec3(-1.0f, 0.5f, 3.0f));
	glm::mat4 RockPosition_2 = glm::translate(matModelBase, glm::vec3(-5.0f, 0.5f, -3.0f));
	glm::mat4 PalmPosition_1 = glm::translate(matModelBase, glm::vec3(3.0f, 0.5f, 3.0f));
	glm::mat4 BarkPosition_1 = glm::translate(matModelBase, glm::vec3(3.0f, 0.2f, -3.0f));
	glm::mat4 GrassPosition_1 = glm::translate(matModelBase, glm::vec3(0.0f, 0.0f, 0.0f));

	// Macierze skalowania obiektow w scenie
	glm::mat4 GroundScale_1 = glm::scale(GroundPosition_1, glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 RockScale_1 = glm::scale(RockPosition_1, glm::vec3(0.5f, 0.5f, 0.5f));
	glm::mat4 RockScale_2 = glm::scale(RockPosition_2, glm::vec3(0.8f, 0.8f, 0.8f));
	glm::mat4 PalmScale_1 = glm::scale(PalmPosition_1, glm::vec3(2.0f, 2.0f, 2.0f));
	glm::mat4 BarkScale_1 = glm::scale(BarkPosition_1, glm::vec3(0.3f, 0.3f, 0.3f));
	glm::mat4 GrassScale = glm::scale(GrassPosition_1, glm::vec3(1.0f, 1.0f, 1.0f));

	// Macierz obrotu kamienia_1 w scenie
	glm::mat4 RockAnimation_1 = glm::rotate(RockScale_1, time_anim, glm::vec3(0.0f, 1.0f, 0.0f));

	// Macierz obrotu kamienia_2 w scenie
	glm::mat4 RockAnimation_2 = glm::rotate(RockScale_2, time_anim, glm::vec3(1.0f, 0.0f, 0.0f));

	// Pozycje obiektow wzgledem srodka sceny
	
	
	palm.SetMatrix(PalmPosition_1);
	bark.SetMatrix(BarkPosition_1);

	// Skalowanie obiektow
	
	rock.SetMatrix(RockAnimation_2);
	palm.SetMatrix(PalmScale_1);
	bark.SetMatrix(BarkScale_1);
	grass.SetMatrix(GrassScale);


	// Wlaczenie potoku
	program.UseProgram();

		// Przekazujemy macierze modelu, widoku i rzutowania do potoku
		program.SetMat4("matProj", matProj);
		program.SetMat4("matView", matView);

		// Uruchamianie potoku (rendering)
		ground.SetMatrix(GroundScale_1);
		program.SetMat4("matModel", ground.GetPositionMatrix());
		program.SetVec3("uniColor", glm::vec3(0.35f, 0.45f, 0.15f));
		ground.DrawMesh();
		rock.SetMatrix(RockAnimation_1);
		program.SetMat4("matModel", rock.GetPositionMatrix());
		program.SetVec3("uniColor", glm::vec3(0.60f, 0.58f, 0.52f));
		rock.DrawMesh();
		rock.SetMatrix(RockAnimation_2);
		program.SetMat4("matModel", rock.GetPositionMatrix());
		program.SetVec3("uniColor", glm::vec3(0.60f, 0.58f, 0.52f));
		rock.DrawMesh();
		palm.SetMatrix(PalmScale_1);
		program.SetMat4("matModel", palm.GetPositionMatrix());
		program.SetVec3("uniColor", glm::vec3(0.10f, 0.65f, 0.20f));
		palm.DrawMesh();
		bark.SetMatrix(BarkScale_1);
		program.SetMat4("matModel", bark.GetPositionMatrix());
		program.SetVec3("uniColor", glm::vec3(0.48f, 0.30f, 0.14f));
		bark.DrawMesh();
		grass.SetMatrix(GrassScale);
		for (const glm::vec3& position : GrassPositions) {
			grass.SetMatrix(glm::translate(matModelBase, position));
			program.SetMat4("matModel", grass.GetPositionMatrix());
			program.SetVec3("uniColor", glm::vec3(0.0f, 1.0f, 0.0f));
			grass.DrawMesh();
		}
		glBindVertexArray( 0 );


	// Wylaczanie potoku
	program.UnuseProgram();
}

// ---------------------------------------
void Initialize(const std::filesystem::path& assetDirectory)
{

	// Ustawianie koloru, ktorym bedzie czyszczony bufor koloru
	glClearColor( 0.5f, 0.5f, 0.5f, 1.0f );
	glEnable(GL_DEPTH_TEST);

	// Obliczanie macierzy rzutowania perspektywicznego
	// za pierwszym razem (po utworzeniu okna aplikacji)
	if (windowHeight != 0)
		matProj = glm::perspective(glm::radians(80.0f), windowWidth/(float)windowHeight, 0.1f, 100.0f);



	// Wczytywanie danych z plikow OBJ

	if(!ground.LoadMesh((assetDirectory / "terrain.obj").string()))
	{
		printf("File not loaded!\n");
	}
	
	if(!rock.LoadMesh((assetDirectory / "rock.obj").string()))
	{
		printf("File not loaded!\n");
	}

	if(!palm.LoadMesh((assetDirectory / "palm.obj").string()))
	{
		printf("File not loaded!\n");
	}

	if(!bark.LoadMesh((assetDirectory / "bark.obj").string()))
	{
		printf("File not loaded!\n");
	}

	if(!grass.LoadMesh((assetDirectory / "grass.obj").string()))
	{
		printf("File not loaded!\n");
	}

	// Potok
	program.CreateProgram();
	program.AttachShaderVertexShader((assetDirectory / "vertex.glsl").string());
	program.AttachShaderFragmentShader((assetDirectory / "fragment.glsl").string());


	// Buffory dla kamienia
	ground.CoordBuffer();
	ground.ColorBuffer(ground.GetNormals());
	program.LinkProgram();
	
	// Buffory dla palmy
	palm.CoordBuffer();
	palm.ColorBuffer(palm.GetNormals());
	program.LinkProgram();
	
	// Buffory dla podłoza
	rock.CoordBuffer();
	rock.ColorBuffer(rock.GetNormals());
	program.LinkProgram();

	// Buffory dla suchego drzewa
	bark.CoordBuffer();
	bark.ColorBuffer(bark.GetNormals());
	program.LinkProgram();

	// Buffory dla trawy
	grass.CoordBuffer();
	grass.ColorBuffer(grass.GetNormals());
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
	program.DeleteProgram();
	rock.DeleteBuffers();
	palm.DeleteBuffers();
	bark.DeleteBuffers();
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
