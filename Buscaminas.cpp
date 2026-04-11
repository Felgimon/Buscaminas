// Buscaminas.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <random>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <map>

const int rows = 16;
const int columns = 16;
const int cantMinas = 40;
bool start = false;
bool gameEnded = false;
std::string finalMessage = "";
int tablero[rows][columns] = { };
int tableroVisual[rows][columns] = { };
int selectedRow = 0;
int selectedColumn = 0;
enum denominations {
	CERO,
	UNO,
	DOS,
	TRES,
	CUATRO,
	CINCO,
	SEIS,
	SIETE,
	OCHO,
	PREGUNTA,
	BANDERA,
	DESCONOCIDO,
	MINA
};

std::string values[13];

void initValues() {
	values[PREGUNTA] = " ? "; 
	values[BANDERA] = u8" ⚑ "; 
	values[DESCONOCIDO] = u8" ■ ";
	values[MINA] = u8" ※ "; /*mina roja*/
	values[CERO] = u8" □ ";
	values[UNO] = " 1 ";
	values[DOS] = " 2 ";
	values[TRES] = " 3 ";
	values[CUATRO] = " 4 ";
	values[CINCO] = " 5 ";
	values[SEIS] = " 6 ";
	values[SIETE] = " 7 ";
	values[OCHO] = " 8 ";
}

std::string colors[13];

void initColors() {
	colors[PREGUNTA] = "";
	colors[BANDERA] = "\033[31m";/*roja*/
	colors[DESCONOCIDO] = "";
	colors[MINA] = "\033[31m"; /*roja*/
	colors[CERO] = "";
	colors[UNO] = "\033[38;5;75m"; /*azul claro*/
	colors[DOS] = "\033[92m"; /*verde claro*/
	colors[TRES] = "\033[38;5;208m"; /*naranja*/
	colors[CUATRO] = "\033[34m"; /*azul*/
	colors[CINCO] = "\033[38;5;88m"; /*rojo bordó*/
	colors[SEIS] = "\033[36m";/*turquesa cyan*/
	colors[SIETE] = "\033[35m"; /*magenta*/
	colors[OCHO] = "\033[90m"; /*gris*/
}

void initTableroVisual() {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			tableroVisual[i][j] = DESCONOCIDO;
		}
	}
}

void printMinas() {
	std::cout << "Minas: " << std::endl;
	std::cout << "\n" << "                      -----------------------------------------------------------------" << "\n";
	for (int i = 0; i < rows; i++) {
		std::cout << "                      |";
		std::string color;
		for (int j = 0; j < columns; j++) {
			color = colors[tablero[i][j]];
			std::cout << color << values[tablero[i][j]] << "\033[0m" << "|";
		}
		std::cout << "\n" << "                      -----------------------------------------------------------------" << "\n";
	}
}

void printTablero() {
	std::cout << "\n\n";
	std::cout << "\n" << "                      -----------------------------------------------------------------" << "\n";
	for (int i = 0; i < rows; i++) {
		std::cout << "                      |";
		std::string color;
		for (int j = 0; j < columns; j++) {
			if (i == selectedRow && j == selectedColumn) color = "\033[38;5;213m"; /*Rosa siempre cuando esta seleccionado*/
			else {
				color = colors[tableroVisual[i][j]];
			}
			std::cout << color << values[tableroVisual[i][j]] << "\033[0m" << "|";
		}
		std::cout << "\n" << "                      -----------------------------------------------------------------" << "\n";
	}
	std::cout << finalMessage;
}

void calculateIndicators() { // Calcula los numeros que indican cuantas minas hay alrededor
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			if (tablero[i][j] == MINA) continue;
			int surroundingMinas = 0;
			for (int r = i-1; r <= i+1; r++) {
				if (r < 0 || r >= rows) continue; /*Si estoy chequeando una fila negativa o más allá del límite de filas, q avance a la siguiente fila*/
				for (int c = j-1; c <= j+1; c++) {
					if (r == i && c == j) continue; /*En el caso de que sea en el slot q estoy evaluando*/
					if (c < 0 || c >= columns) continue; /*Si estoy chequeando una columna negativa o más allá del límite de columnas, q avance a la siguiente columna*/
					if (tablero[r][c] == MINA) surroundingMinas++;
				}
			}
			tablero[i][j] = surroundingMinas;
		}
	}
}

//Hacer que createMinas() se ejecute una vez que se debloquee el slot

void createMinas() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distrib(0, columns - 1);
	std::uniform_int_distribution<int> distribTwo(0, rows - 1);
	for (int i = 0; i < cantMinas; i++) {
		int ranColumn = distrib(gen);
		int ranRow = distribTwo(gen);
		while (tablero[ranRow][ranColumn] == MINA || (ranRow == selectedRow-1 && ranColumn == selectedColumn-1) || (ranRow == selectedRow-1 && ranColumn == selectedColumn) || (ranRow == selectedRow - 1 && ranColumn == selectedColumn+1) || (ranRow == selectedRow && ranColumn == selectedColumn - 1) || (ranRow == selectedRow && ranColumn == selectedColumn) || (ranRow == selectedRow && ranColumn == selectedColumn+1) || (ranRow == selectedRow+1 && ranColumn == selectedColumn -1) || (ranRow == selectedRow + 1 && ranColumn == selectedColumn) || (ranRow == selectedRow + 1 && ranColumn == selectedColumn+1)){ /*No puede colocar una mina donde ya hay una mina, ni colocar una mina donde el jugador selecciono ni sus alrededores para empezar el juego*/
			ranRow = distribTwo(gen);
			ranColumn = distribTwo(gen);
		}
		tablero[ranRow][ranColumn] = MINA;
	}
	calculateIndicators();
}

void selectionManager(std::string command) {
	if (command == "UP") {
		if (selectedRow - 1 >= 0) selectedRow--;
	}
	else if (command == "RIGHT") {
		if (selectedColumn + 1 < columns) selectedColumn++;
	} 
	else if (command == "LEFT") {
		if (selectedColumn - 1 >= 0) selectedColumn--;
	}
	else if (command == "DOWN") {
		if (selectedRow + 1 < rows) selectedRow++;
	}
}

//Que si descubris un 0 interactuando en un slot vacio q funcione como en startingIndicators y busque todos los 0 y sus alrededores. Check
//Que si interactuas con una casilla suelta y q no sea una mina se descubra, y si es una mina q pierdas.
//Los unos de las esquinas no descubren nada. Check
//Los numeros q interactues q estan en las esquinas para desbloquear zonas sin minas descubren los de la columna - 1, fuera del limite de columns. Check
//Cambiar de que cuando interactuas sobre un indicador no corrobore en base a las flags puestas correctamente. Sino que deje poner esas banderas de manera incorrecta y que pierdas si interactuas con el indicador.


void colocarBandera() {
	if (tableroVisual[selectedRow][selectedColumn] == DESCONOCIDO || tableroVisual[selectedRow][selectedColumn] == PREGUNTA) tableroVisual[selectedRow][selectedColumn] = BANDERA;
	else if (tableroVisual[selectedRow][selectedColumn] == BANDERA) tableroVisual[selectedRow][selectedColumn] = DESCONOCIDO; /*Te permite sacar la bandera*/
}

void colocarPregunta() {
	if (tableroVisual[selectedRow][selectedColumn] == DESCONOCIDO || tableroVisual[selectedRow][selectedColumn] == BANDERA) tableroVisual[selectedRow][selectedColumn] = PREGUNTA;
	else if (tableroVisual[selectedRow][selectedColumn] == PREGUNTA) tableroVisual[selectedRow][selectedColumn] = DESCONOCIDO; /*Te permite sacar la bandera*/
}

void showIndicators(int evaluatedRow, int evaluatedColumn) { /*Cambiar nombre */
	for (int r = evaluatedRow - 1; r <= evaluatedRow + 1; r++) {
		for (int c = evaluatedColumn - 1; c <= evaluatedColumn + 1; c++) {
			if (r < 0 || r >= rows || c < 0 || c >= columns) continue;

			if (tableroVisual[r][c] != DESCONOCIDO) continue; /*No se vuelve a evaluar slots q ya fueron evauados*/

			tableroVisual[r][c] = tablero[r][c];

			if (tablero[r][c] == CERO) {
				showIndicators(r, c);
			}
		}
	}
}

void Lose(int evaluatedRow, int evaluatedColumn, bool showOnlySelectedMina) {
	if (showOnlySelectedMina) {
		if (tablero[selectedRow][selectedColumn] == MINA && tableroVisual[selectedRow][selectedColumn] != BANDERA) {
			tableroVisual[selectedRow][selectedColumn] = MINA;
		}
	}
	else {
		for (int r = evaluatedRow - 1; r <= evaluatedRow + 1; r++) {
			for (int c = evaluatedColumn - 1; c <= evaluatedColumn + 1; c++) {
				if (r < 0 || r >= rows || c < 0 || c >= columns) continue;
				if (tablero[r][c] == MINA && tableroVisual[r][c] != BANDERA) tableroVisual[r][c] = MINA; /*Muestra la mina q no se marco*/
			}
		}
	}
	finalMessage = "                      \033[31mYOU LOSE\033[0m\n                      Please, press the escape bar to play again";
	gameEnded = true;
}

void interact() {
	if (!start) {
		createMinas();
		showIndicators(selectedRow, selectedColumn);
		start = true;
	}
	else {
		if (tableroVisual[selectedRow][selectedColumn] != BANDERA && tableroVisual[selectedRow][selectedColumn] != PREGUNTA) {
			if (tablero[selectedRow][selectedColumn] <= OCHO && tablero[selectedRow][selectedColumn] > CERO && tableroVisual[selectedRow][selectedColumn] == DESCONOCIDO) { /*si es un indicador apretado en un slot desconocido*/\
				tableroVisual[selectedRow][selectedColumn] = tablero[selectedRow][selectedColumn]; 
				if (tablero[selectedRow][selectedColumn] == MINA && tableroVisual[selectedRow][selectedColumn] != BANDERA) {
					Lose(selectedRow, selectedColumn, true);
				}
			}
			else if (tablero[selectedRow][selectedColumn] == MINA) {
				Lose(selectedRow, selectedColumn, true);
			}
			else {
				int validFlags = 0;
				for (int r = selectedRow - 1; r <= selectedRow + 1; r++) {
					for (int c = selectedColumn - 1; c <= selectedColumn + 1; c++) {
						if (r < 0 || r >= rows || c < 0 || c >= columns) continue; /*Para que no se pase del limite*/
						if (tablero[r][c] == MINA && tableroVisual[r][c] == BANDERA) validFlags++; /*Tiene que ser una bandera puesta si o si arriba de una mina*/
					}
				}
				if (validFlags == tableroVisual[selectedRow][selectedColumn] || tablero[selectedRow][selectedColumn] == CERO) { /*En el caso de que esten bien puestas las banderas*/
					showIndicators(selectedRow, selectedColumn);
				}
				else {
					Lose(selectedRow, selectedColumn, false);
				}
			}
		}
	}
}

void inputManager() {
	bool listening = true;
	while (listening) {
		if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(0x57)) { // Arriba
			Sleep(150);
			selectionManager("UP");
			listening = false;
		}
		else if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(0x44)) { // Derecha
			Sleep(150);
			selectionManager("RIGHT");
			listening = false;
		}
		else if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(0x41)) { // Izquierda
			Sleep(150);
			selectionManager("LEFT");
			listening = false;
		}
		else if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(0x53)) { // Abajo
			Sleep(150);
			selectionManager("DOWN");
			listening = false;
		}
		if (gameEnded) { /*Si el juego termino solo se puede hacer esta accion y las de arriba*/
			if (GetAsyncKeyState(0x20)) { /*Barrra espaciadora = cuando perdes*/
				Sleep(150);
				std::cout << "\n                      We are working on it"; //Aca deberia ir buscaminas() pero el identifier me tira error, porque buscaminas() deberia estar arriba de esta funcion, pero no es posible porque entonces no podria llamar a inputManager()
				listening = false;
			}
		}
		else {
			if (GetAsyncKeyState(0x47)) { /*Tecla G = interactuar*/
				Sleep(150);
				interact();
				listening = false;
			}
			else if (GetAsyncKeyState(0x48)) { /*Tecla H = bandera*/
				Sleep(150);
				colocarBandera();
				listening = false;
			}
			else if (GetAsyncKeyState(0x4A)) { /*Tecla J = pregunta*/
				Sleep(150);
				colocarPregunta();
				listening = false;
			}
		} 
	}
}

void buscaminas() {
	initValues();
	initColors();
	initTableroVisual();
	while (true) {
		//printMinas();
		printTablero();
		inputManager();
		#if defined(_WIN32) || defined(_WIN64)
				std::system("cls");
		#else
				std::system("clear");
		#endif
	}
}

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	buscaminas();

	return 0;
}

