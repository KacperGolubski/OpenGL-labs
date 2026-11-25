// --------------------------------------------------------------
// Wskazówki/mini wzorce projektowe programowania w OpenGL
// --------------------------------------------------------------
// UWAGA!
// --------------------------------------------------------------
// Niniejsze propozycje co do nazewnictwa klas/metod sa tylko
// propozycjami i nalezy je dostosowac do wlasnych potrzeb.
// --------------------------------------------------------------

// Zacznijmy...

// Mamy dwa glowne obiekty w OpenGL:
// (a) potok graficzny
// (b) obiekty 3D trzymane w VAO + VBO 

// Stworzmy wiec dwie podstawowe klasy


// --------------------------------------------------------------
// 1. Klasa do obslugi potoku
// --------------------------------------------------------------
class CProgram
{

public:

	// Glowna metoda tworzaca obiekt programu
	// korzystajaca m.in. z glCreateProgram()
	Init();
	
	// Jak juz tworzymy cos to warto posprzatac
	// uzywajac funkcji glDeleteProgram
	Clean(); 
    
	
	// UWAGA: Metody Init() i Clean() mozna 
	// z powodzeniem zastapic konstruktorami
	// i dekstruktorem.
	
	
	// Metody ladujace shadery:
	LoadVertexShader(const char *Filename);
	LoadFragmentShader(const char *Filename);

	// lub w jednej metodzie
	LoadShaders(VertexFilename, FragmentFilename);

	// UWAGA: A moze zamiast dwoch funkcji Init()
	// oraz LoadShaders, zrobic z nich jedna?
	

	// Koniecznie dodajemy metode aktywujaca potok 
	// przy uzyciu glUseProgram()
	Use();  
	
	// A jak aktywujemy to na potrzeby nauki i
	// latwiejszego wykrywania bledow dodajemy
	// metode dezaktywujaca potok 
	UnUse();

	// Metoda ustawiajace wartosc zmiennych uniform 
	SetFloat(const char *Str, float value);
	SetInt(const char *Str, int value);
	SetMat4(const char *Str, glm::mat4 matrix);
	// szczegolnie ta ostatnia przyda sie 
	// podczas przesylania do potoku macierzy
	// rzutowania i transformacji
	
	
	
	// Albo inne settery i gettery
	getId();
	
	
private:

	// Jakie skladowe umiescimy w klasie CProgram?
	// Z pewnoscia identyfikator potoku
	
	GLuint idProgram;
	
}

// --------------------------------------------------------------
// 2. Oraz druga klasa do obslugi obiektow 3D
// --------------------------------------------------------------
class CMesh
{

public:
	
	// Metody tworzaca VAO i VBO z pliku OBJ
	CreateFromOBJ(Filename);
	
	// Metoda czyszczaca, ktora posprzata
	// usunie pamiec, obiekty VAO itd.
	Clean();
	
	
	// Oczywiscie metoda generujaca obraz na ekranie
	// To ona wywola metode glDrawArrays()
	Draw();
	
	// Moze cos jeszcze? Na przyklad metody
	// do obslugi macierzy modelu obiektu?
	SetPosition(glm::vec3 Position);
	SetRotation(glm::vec3 Angles);
	

private:

	// Skladowe
	
	// Identyfikator VAO
	GLuint idVAO;
	
	// Identyfikatory buforow na potrzeby VAO
	GLuint idVBO_coords;
	GLuint idVBO_uvs; // i inne..
	
	
	// Macierz modelu, to w niej bedzie
	// przechowana pozycja i orientacja obiektu
	// na scenie
	glm::mat4 matModel;
}


// --------------------------------------------------------------
// PRZYKLAD UZYCIA
// --------------------------------------------------------------

// Majac takie klasy bedzie nam wygodnie tworzyc obiekty potokow
// oraz obiekty. 

// Przykladowo:  majac obiekt potoku oraz obiekty 3D
// jako zmienne globalne inicjalizujemy je w Initialize
// a nastepnie uzywamy w DisplayScene.


CProgram glownyProgram;
CMesh kwiatek3D, domek3D;


// ---------------------------------------
void DisplayScene()
{
	// Czyszczenie ramki
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


	// Geometria sceny i obliczanie macierzy rzutowania
	matPV = matProj * matView;


	// Aktywujemy potok
	glownyProgram.Use();

		// Przekazujemy zmienne jednorodne
		// na przykald macierze projection i view
		// tutaj: jako jedna (wymnozona juz)
		glownyProgram.SetMat4("matPV", matPV);

		// Wywolujemy rendering obiektow
		kwiatek3D.Draw();
		domek3D.Draw();


	// Dezaktywujemy potok
	glownyProgram.UnUse();



	glutSwapBuffers();
}

// ---------------------------------------------------
void Initialize()
{

	// Tworzenie obiektu potoku
	// UWAGA: Chociaz zamiast dwoch metod, moze wygodniej
	// byloby stworzyc tylko jedna?
	glownyProgram.Init();
	glownyProgram.LoadShaders("vertex.glsl", "fragment.glsl");

	// Tworzenie obiektow 3D z plikow OBJ
	kwiatek3D.CreateFromOBJ("flower.obj");
	kwiatek3D.SetPosition(glm::vec3(1.0, 0.0, 1.0));

	domek3D.CreateFromOBJ("house.obj");
	kwiatek3D.SetPosition(glm::vec3(3.0, 0.0, -1.0));

	// itd...

}


// Majac takie klasy bedzie nam duzo wygodniej w przyszlosci.
// O szczegoly pytaj prowadzacego i kolegow/kolezanki.