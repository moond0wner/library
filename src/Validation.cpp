#include "Validation.h"
#include "Utils.h"
#include <iostream>

using namespace std;

int inputValidBookId(Library& lib, bool forBorrow) {
	while (true) {
		lib.printAllBooks();
		int id = inputInt("ID книги: ");
		Book* book = lib.findBookById(id);

		if (forBorrow) {
			if (book && book->getIsAvailable()) return id;
			cout << "Книга не найдена или уже выдана. Попробуйте снова." << endl;
		}
		else {
			if (book && !book->getIsAvailable()) return id;
			cout << "Книга не найдена или не выдана. Попробуйте снова." << endl;
		}
	}
}

int inputValidReaderId(Library& lib) {
	while (true) {
		lib.printAllReaders();
		int id = inputInt("ID читателя: ");
		Reader* reader = lib.findReaderById(id);
		if (reader) return id;
		cout << "Читатель не найден. Попробуйте снова." << endl;
	}
}