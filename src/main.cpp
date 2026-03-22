#include <iostream>
#include <windows.h>
#include <string>
#include "Library.h"
#include "Book.h"
#include "Utils.h"
#include "Interface.h"

using namespace std;

int main() {
	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);
	setlocale(LC_ALL, ".UTF8");

	try {
		Library lib;
		lib.loadFromFile(Library::FILE_NAME);

		int choice;
		do {
			mainInterface();
			choice = inputInt("Выберите действие: ");

			switch (choice) {
			case 1: menuBooks(lib); break;
			case 2: readerMenu(lib); break;
			case 3: lib.printOverdue(getCurrentDate()); break;
			}
		} while (choice != 4);

		cout << "Данные сохраняются. Программа завершена." << endl;
		lib.saveToFile(Library::FILE_NAME);

	}
	catch (const exception& e) {
		cerr << "Критическая ошибка: " << e.what() << endl;
		return 1;
	}
	return 0;
}
