// Buscaminas.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <random>
#include <Windows.h>
#include <iostream>
#include <conio.h>

const int cantCasillas = 100;
int cantMinas = 25;
bool minas[cantCasillas] = { };
int selectedIndex = 0;

void printBehaviour(std::string c, bool selected) {
	std::cout << "|";
	if (selected) std::cout << "\033[34m" << c;
	else std::cout << c;
}

void printMinas() {
	std::cout << "Minas: " << std::endl;
	for (int i = 0; i < cantCasillas; i++) {
		std::string symbol;
		if (minas[i] == true) {
			symbol = u8"💣";
		}
		else {
			symbol = u8"  ";

		}
		std::cout << "|" << symbol;
		if ((i + 1) % 10 == 0 && i != 0) {
			std::cout << "|" << "\n";
			std::cout << "-------------------------------" << "\n";
		}
	}
	std::cout << "\n";
}

void printTablero() {
	std::cout << "Tablero: " << std::endl;
	for (int i = 0; i < cantCasillas; i++) {
		std::string symbol;
		symbol = u8"  ";
		printBehaviour(symbol, i == selectedIndex);
		std::cout << " ";
		if ((i + 1) % 10 == 0 && i != 0) {
			std::cout << "|" << "\n";
			std::cout << "-------------------------------" << "\n";
		}
	}
}

void createMinas() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distrib(0, cantCasillas - 1);
	for (int i = 0; i < cantMinas; i++) {
		int ran = distrib(gen);
		while (minas[ran] == true) {
			ran = distrib(gen);
		}
		minas[ran] = true;
	}
}

void Buscaminas() {
	createMinas();
	bool listening;
	while (true) {
		printMinas();
		printTablero();
		listening = true;
		while (listening) {
			if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(0x57)) { // Arriba
				Sleep(100);
				if (selectedIndex - 10 >= 0) {
					selectedIndex = selectedIndex - 10;
				}
				listening = false;
			}
			else if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(0x44)) { // Derecha
				Sleep(100);
				if (selectedIndex + 1 < cantCasillas) {
					selectedIndex = selectedIndex + 1;
				}
				listening = false;
			}
			else if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(0x41)) { // Izquierda
				Sleep(100);
				if (selectedIndex - 1 >= 0) {
					selectedIndex = selectedIndex - 1;
				}
				listening = false;
			}
			else if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(0x53)) { // Abajo
				Sleep(100);
				if (selectedIndex + 10 < cantCasillas) {
					selectedIndex = selectedIndex + 10;
				}
				listening = false;
			}
		}
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

