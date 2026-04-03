// Buscaminas.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <random>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <map>

const int rows = 10;
const int columns = 10;
const int cantMinas = 18;
bool start = false;
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
	values[BANDERA] = u8" ⚐ "; 
	values[DESCONOCIDO] = u8" ■ ";
	values[MINA] = u8"\033[31m ※ \033[0m"; /*mina roja*/
	values[CERO] = " 0 ";
	values[UNO] = " 1 ";
	values[DOS] = " 2 ";
	values[TRES] = " 3 ";
	values[CUATRO] = " 4 ";
	values[CINCO] = " 5 ";
	values[SEIS] = " 6 ";
	values[SIETE] = " 7 ";
	values[OCHO] = " 8 ";
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
	for (int i = 0; i < rows; i++) {
		std::cout << "|";
		for (int j = 0; j < columns; j++) {
			std::cout << values[tablero[i][j]] << "|";
		}
		std::cout << "\n" << "-----------------------------------------" << "\n";
	}
}

void printTablero() {
	std::cout << "Tablero: " << std::endl;
	for (int i = 0; i < rows; i++) {
		std::cout << "|";
		for (int j = 0; j < columns; j++) {
			if (i == selectedRow && j == selectedColumn) {
				std::cout << "\033[34m" << values[tableroVisual[i][j]] << "\033[0m" << "|"; /*Azul siempre cuando esta seleccionado*/
			}
			else {
				std::cout << values[tableroVisual[i][j]] << "|";
			}
		}
		std::cout << "\n" << "-----------------------------------------" << "\n";
	}
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
		if (selectedColumn + 1 <= 9) selectedColumn++;
	} 
	else if (command == "LEFT") {
		if (selectedColumn - 1 >= 0) selectedColumn--;
	}
	else if (command == "DOWN") {
		if (selectedRow + 1 <= 9) selectedRow++;
	}
}

void colocarBandera() {
	tableroVisual[selectedRow][selectedColumn] = BANDERA;
}

void colocarPregunta() {
	tableroVisual[selectedRow][selectedColumn] = PREGUNTA;
}

void showStartingIndicators() { /*Hay q hacer una funcion q muestre desde el row y column seleccionado todos los q sean indicador 0 hasta los q tengan un indicador q no sea 0.*/
	std::cout << "Mostrar indicadores";
}

void interact() {
	if (!start) {
		createMinas();
		showStartingIndicators();
		start = true;
	}
	else {
	}
}

void inputManager() {
	bool listening = true;
	while (listening) {
		if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(0x57)) { // Arriba
			Sleep(100);
			selectionManager("UP");
			listening = false;
		}
		else if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(0x44)) { // Derecha
			Sleep(100);
			selectionManager("RIGHT");
			listening = false;
		}
		else if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(0x41)) { // Izquierda
			Sleep(100);
			selectionManager("LEFT");
			listening = false;
		}
		else if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(0x53)) { // Abajo
			Sleep(100);
			selectionManager("DOWN");
			listening = false;
		}
		else if (GetAsyncKeyState(0x47)) { /*Tecla G = interactuar*/
			Sleep(100);
			interact();
			listening = false;
		}
		else if (GetAsyncKeyState(0x48)) { /*Tecla H = bandera*/
			Sleep(100);
			colocarBandera();
			listening = false;
		}
		else if (GetAsyncKeyState(0x4A)) { /*Tecla J = pregunta*/
			Sleep(100);
			colocarPregunta();
			listening = false;
		}
	}
}


void buscaminas() {
	initValues();
	initTableroVisual();
	while (true) {
		printMinas();
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

