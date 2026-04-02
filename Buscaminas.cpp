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
const int cantMinas = 20;
int tablero[rows][columns] = { };
enum denominations tableroVisual[rows][columns] = { };
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
	values[BANDERA] = u8"🚩 ";
	values[DESCONOCIDO] = u8" ■ ";
	values[MINA] = u8"💥 ";
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
				std::cout << "\033[34m" << values[tableroVisual[i][j]] <<"\033[0m" << "|";
			}
			else {
				std::cout << values[tableroVisual[i][j]] << "|";
			}
		}
		std::cout << "\n" << "-----------------------------------------" << "\n";
	}
}

void calculateIndicators() { // Calcula los numeros que indican cuantas minas hay alrededor
//	for (int i = 0; i < cantCasillas; i++) {
//		if (tablero[i] == MINA) {
//			continue;
//		}
//		int surroundingMinas = 0;
//		std::vector<int> casillasToCalculate = {};
//		if (i % 10 == 0) { // Columna de la izquierda del todo <<
//			if (i != 90) { // Para la esquina izquierda de arriba o todas las demas en la columna
//				casillasToCalculate.push_back(tablero[i + 10]); // bottomCenter
//				casillasToCalculate.push_back(tablero[i + 11]); // bottomRight
//			}
//			else if (i == 90 || i > 0) { // Para la esquina izquierda de abajo o todas las demas en la columna
//				casillasToCalculate.push_back(tablero[i - 10]); // topCenter
//				casillasToCalculate.push_back(tablero[i - 9]); // topRight
//			}
//			casillasToCalculate.push_back(tablero[i+1]); // rightCenter
//		}
//		else if (i % 10 == 9) { // Columna de la derecha del todo <<
//			if (i != 99) { // Para la esquina derecha de arriba o todas las demas en la columna
//				casillasToCalculate.push_back(tablero[i + 10]); // bottomCenter
//				casillasToCalculate.push_back(tablero[i + 9]); // bottomLeft
//				// Caso de no en las esquinas
//			}
//			else if (i == 99 || i > 9) { // Para la esquina derecha de abajo o todas las demas en la columna
//				casillasToCalculate.push_back(tablero[i - 10]); // topCenter
//				casillasToCalculate.push_back(tablero[i - 11]); // topLeft
//			}
//			casillasToCalculate.push_back(tablero[i-1]); // leftCenter
//		}
//		else if (i > 0 && i < 9) { //Primera fila de arriba sin bordes
//			casillasToCalculate.push_back(tablero[i-1]); // leftCenter
//			casillasToCalculate.push_back(tablero[i+1]); // rightCenter
//			casillasToCalculate.push_back(tablero[i + 10]); // bottomCenter
//			casillasToCalculate.push_back(tablero[i + 9]); // bottomLeft
//			casillasToCalculate.push_back(tablero[i + 11]); // bottomRight
//		} else if (i > 90 && i < 99) { // Ultima fila de abajo sin bordes
//			casillasToCalculate.push_back(tablero[i-1]); // leftCenter
//			casillasToCalculate.push_back(tablero[i+1]); // rightCenter
//			casillasToCalculate.push_back(tablero[i - 10]); // topCenter
//			casillasToCalculate.push_back(tablero[i - 9]); // topRight
//			casillasToCalculate.push_back(tablero[i - 11]); // topLeft
//		}
//		else { // Todas las casillas de dentro
//			casillasToCalculate.push_back(tablero[i-1]); // leftCenter
//			casillasToCalculate.push_back(tablero[i+1]); // rightCenter
//			casillasToCalculate.push_back(tablero[i - 10]); // topCenter
//			casillasToCalculate.push_back(tablero[i - 9]); // topRight
//			casillasToCalculate.push_back(tablero[i - 11]); // topLeft
//			casillasToCalculate.push_back(tablero[i + 10]); // bottomCenter
//			casillasToCalculate.push_back(tablero[i + 9]); //bottomLeft
//			casillasToCalculate.push_back(tablero[i + 11]); //bottomLeft
//		}
//		for (int casilla : casillasToCalculate) {
//			if (casilla == MINA) {
//				surroundingMinas += std::abs(casilla);
//			}
//		}
//		tablero[i] = surroundingMinas;
//	}
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
		while (tablero[ranRow][ranColumn] == MINA) { /*Meter un "&&" para cuando quiero chequear la ubicacion de donde se inicio el juego*/
			ranColumn = distrib(gen);
			ranRow = distribTwo(gen);
		}
		tablero[ranRow][ranColumn] = MINA;
	}
	calculateIndicators();
}

void SelectionManager(std::string command) {
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
	std::cout << "\nRow: " << selectedRow;
	std::cout << "\Column: " << selectedColumn;
}

void InputManager() {
	bool listening = true;
	while (listening) {
		if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(0x57)) { // Arriba
			Sleep(100);
			SelectionManager("UP");
			listening = false;
		}
		else if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(0x44)) { // Derecha
			Sleep(100);
			SelectionManager("RIGHT");
			listening = false;
		}
		else if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(0x41)) { // Izquierda
			Sleep(100);
			SelectionManager("LEFT");
			listening = false;
		}
		else if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(0x53)) { // Abajo
			Sleep(100);
			SelectionManager("DOWN");
			listening = false;
		}
	}
}


void Buscaminas() {
	initValues();
	createMinas();
	initTableroVisual();
	while (true) {
		printMinas();
		printTablero();
		InputManager();
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
	Buscaminas();

	return 0;
}

