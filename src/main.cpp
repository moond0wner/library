/*TODO:
* 1. функционал 3 и 4 пункта главного меню поместить в 1 пункт
* 2. добавить валидацию на номер телефона/email (перед этим узнать что будет вводится)
* 3. дописать в выводе всех книг их год создания
* 4. подробнее написать кому принадлежит какая книга
* 5. подробнее написать какие книги есть у каждого читателя
* 6. к дате написать время
*/ 

#include <iostream>
#include <Windows.h>
#include <string>
#include "Library.h"
#include "Book.h"
#include "Utils.h"

using namespace std;

void printHeader(const string& title) {
	cout << "\n" << string(50, '=') << endl;
	cout << "          " << title << endl;
	cout << string(50, '=') << endl;
}

void mainInterface() {
	printHeader("БИБЛИОТЕКА");
	cout << "1. Управление книгами" << endl;
	cout << "2. Управление читателями" << endl;
	cout << "3. Выдать книгу" << endl;
	cout << "4. Вернуть книгу" << endl;
	cout << "5. Показать должников" << endl;
	cout << "6. Выход" << endl;
	cout << "----------------------------------------" << endl;
	cout << "Выберите действие: ";
}

void bookInterface() {
	cout << "--- Управление книгами ---" << endl;
	cout << "1. Добавить книгу" << endl;
	cout << "2. Удалить книгу" << endl;
	cout << "3. Найти по названию" << endl;
	cout << "4. Найти по автору" << endl;
	cout << "5. Показать все книги" << endl;
	cout << "6. Показать выданные" << endl;
	cout << "7. Назад" << endl;
	cout << "----------------------------------------" << endl;
	cout << "Выберите действие: ";
}

void menuBooks(Library& lib) {
	int choice;
	do {
		bookInterface();
		cin >> choice;
		switch (choice) {
		case 1: {
			string title, author;
			int year;
			
			cin.ignore();
			cout << "Введите название книги: ";
			getline(cin, title);

			cout << "Введите автора данной книги: ";
			getline(cin, author);

			cout << "Введите дату издания книги: ";
			cin >> year;
			cin.ignore();

			lib.addBook(title, author, year);
			cout << "Книга успешно добавлена в библиотеку и уже ждет своего читателя!" << endl;
			break;
		}
		case 2: {
			vector<Book> books = lib.getAvailableBooks();
			if (books.empty()) {
				cout << "Свободных книг нет, либо библиотека пустая" << endl;
				break;
			}
			int choice;
			bool valid = false;
			while (!valid) {
				cout << "Представлены свободные книги:" << endl;
				lib.printAvailableBooks();
				cout << "Введите номер книги (ID) которую нужно убрать из библиотеки: ";
				cin >> choice;

				Book* book = lib.findBookById(choice);

				if (book && book->getIsAvailable()) {
					valid = true;
				}
				else {
					cout << "Ошибка: книга с таким ID не найдена или уже выдана. Попробуйте снова." << endl;
				}
			}
			if (lib.removeBook(choice)) {
				cout << "Книга удалена!" << endl;
			}
			else {
				cout << "Не удалось удалить книгу" << endl;
			}
			cout << endl;
			break;

		}
		case 3: {
			string targetNameBook;
			cin.ignore();
			cout << "Введите название книги: ";
			getline(cin, targetNameBook);

			vector<Book> foundBooks = lib.findBooksByTitle(targetNameBook);
			if (foundBooks.empty()) {
				cout << "По вашему запросу не найдена ни одна книга" << endl;
				break;
			}
			cout << "Найдены следующие книги:" << endl;
			for (auto& book : foundBooks) {
				cout << "\t" << book.getId() << ": " << book.getTitle() << " - " << book.getAuthor() << endl;
			}
			break;
		}
		case 4: {
			string targetAuthorBook;
			cin.ignore();
			cout << "Введите автора: ";
			getline(cin, targetAuthorBook);

			vector <Book> foundBooks = lib.findBooksByAuthor(targetAuthorBook);
			if (foundBooks.empty()) {
				cout << "По вашему запросу не найдена ни одна книга" << endl;
				break;
			}
			cout << "Найдены следующие книги:" << endl;
			for (auto& book : foundBooks) {
				cout << "\t" << book.getId() << ": " << book.getTitle() << " - " << book.getAuthor() << endl;
			}
			break;
		}
		case 5: {
			if (lib.getAllBooks().empty()) {
				cout << "Библиотека пустая" << endl;
				break;
			}
			cout << "Все книги:" << endl;
			lib.printAllBooks();
			break;
		}
		case 6: {
			vector<Book> borrowedBooks = lib.getBorrowedBooks();
			if (borrowedBooks.empty()) {
				cout << "Ни одну книгу не заняли, либо библиотека пустая" << endl;
				break;
			}
			cout << "Занятые книги:" << endl;
			lib.printBorrowedBooks();
			break;
		}
		}
	} while (choice != 7);
}

void readerInterface() {
	cout << "--- Управление читателями ---" << endl;
	cout << "1. Добавить читателя" << endl;
	cout << "2. Удалить читателя" << endl;
	cout << "3. Найти по имени" << endl;
	cout << "4. Показать всех читателей" << endl;
	cout << "5. Назад" << endl;
	cout << "----------------------------------------" << endl;
	cout << "Выберите действие: ";
}

void readerMenu(Library& lib) {
	int choice;
	do {
		readerInterface();
		cin >> choice;
		switch (choice) {
		case 1: {
			string name, contact;

			cin.ignore();
			cout << "Введите имя читателя: ";
			getline(cin, name);

			cout << "Введите контакт читателя (номер телефона/email): ";
			getline(cin, contact);

			lib.addReader(name, contact);
			cout << "Читатель успешно добавлен!" << endl;
			break;
		} 
		case 2: {
			if (lib.getAllReaders().empty()) {
				cout << "В данный момент в библиотеке нет ни одного читателя!" << endl;
				break;
			}
			int choice;
			bool valid = false;

			while (!valid) {
				cout << "Представлен список читателей:" << endl;
				lib.printAllReaders();

				cout << "Введите номер (ID) читателя которого нужно удалить из библиотеки: ";
				cin >> choice;
				Reader* reader = lib.findReaderById(choice);
				if (reader) {
					if (lib.removeReader(reader->getId())) {
						cout << "Пользователь удален!" << endl;
						valid = true;
					}
					else cout << "Не удалось удалить читателя" << endl;
				}
				else {
					cout << "Читатель не найден, попробуйте снова!" << endl;
				}
			}
			break;
		}
		case 3: {
			if (lib.getAllReaders().empty()) {
				cout << "В данный момент в библиотеке нет ни одного читателя!" << endl;
				break;
			}
			string targetNameReader;
			cin.ignore();
			cout << "Введите имя читателя: ";
			getline(cin, targetNameReader);
			vector<Reader> foundReaders = lib.findReadersByName(targetNameReader);
			if (foundReaders.empty()) {
				cout << "По вашему запросу ничего не найдено" << endl;
				break;
			}
			cout << "Найдены следующие читатели:" << endl;
			for (auto& reader : foundReaders) {
				cout << "\t" << reader.getId() << ": " << reader.getNameReader() << " - " << reader.getContactReader() << endl;
			}
			break;
		}
		case 4: {
			if (lib.getAllReaders().empty()) {
				cout << "В данный момент в библиотеке нет ни одного читателя!" << endl;
				break;
			}
			cout << "Представлен список читателей:" << endl;
			lib.printAllReaders();
			break;
		}
		}
	} while (choice != 5);
}
void borrowBookInterface(Library& lib) {
	if (lib.getAllBooks().empty() || lib.getAllReaders().empty()) {
		cout << "Нет ни одного читателя или книги" << endl;
		return;
	}

	cout << "--- Выдача книги ---" << endl;

	int bookId, readerId;
	bool validBook = false, validReader = false;

	while (!validBook) {
		cout << "ID книги: ";
		cin >> bookId;
		Book* book = lib.findBookById(bookId);
		if (book && book->getIsAvailable()) {
			validBook = true;
		}
		else {
			cout << "Книга с таким ID не найдена или уже выдана. Попробуйте снова." << endl;
		}
	}

	while (!validReader) {
		cout << "ID читателя: ";
		cin >> readerId;
		Reader* reader = lib.findReaderById(readerId);
		if (reader) {
			validReader = true;
		}
		else {
			cout << "Читатель с таким ID не найден. Попробуйте снова." << endl;
		}
	}

	if (lib.borrowBook(bookId, readerId, getCurrentDate())) {
		cout << "Книга выдана!" << endl;
	}
	else {
		cout << "Ошибка выдачи!" << endl;
	}
}

void returnBookInterface(Library& lib) {
	if (lib.getAllBooks().empty() || lib.getAllReaders().empty()) {
		cout << "Нет ни одного читателя или книги" << endl;
		return;
	}

	cout << "--- Возврат книги ---" << endl;

	int bookId, readerId;
	bool validBook = false, validReader = false;


	while (!validBook) {
		cout << "ID книги: ";
		cin >> bookId;
		Book* book = lib.findBookById(bookId);
		if (book && !book->getIsAvailable()) {
			validBook = true;
		}
		else {
			cout << "Книга с таким ID не найдена или не выдана. Попробуйте снова." << endl;
		}
	}

	while (!validReader) {
		cout << "ID читателя: ";
		cin >> readerId;
		Reader* reader = lib.findReaderById(readerId);
		if (reader) {
			validReader = true;
		}
		else {
			cout << "Читатель с таким ID не найден. Попробуйте снова." << endl;
		}
	}

	if (lib.returnBook(bookId, readerId)) {
		cout << "Книга вернулась!" << endl;
	}
	else {
		cout << "Ошибка возврата!" << endl;
	}
}

void overdueInterface(Library& lib) {
	cout << "--- Должники ---" << endl;
	lib.printOverdue(getCurrentDate());
}

int main() {
	system("chcp 1251 > nul");  // Windows-1251
	setlocale(LC_ALL, "Rus");

	Library lib;
	int choice;

	do {
		mainInterface();
		cin >> choice;
		switch (choice) {
		case 1: menuBooks(lib); break;
		case 2: readerMenu(lib); break;
		case 3: borrowBookInterface(lib); break;
		case 4: returnBookInterface(lib); break;
		case 5: overdueInterface(lib); break;
	}

	} while (choice != 6);

	cout << "Программа завершена" << endl;

	return 0;
}
