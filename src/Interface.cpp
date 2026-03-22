#include "Interface.h"
#include "Utils.h"
#include "Validation.h"
#include <string>
#include <vector>
#include <iostream>

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
	cout << "3. Показать должников" << endl;
	cout << "4. Выход" << endl;
	cout << "----------------------------------------" << endl;
}

void bookInterface() {
	cout << "--- Управление книгами ---" << endl;
	cout << "1. Добавить книгу" << endl;
	cout << "2. Удалить книгу" << endl;
	cout << "3. Выдать книгу" << endl;
	cout << "4. Вернуть книгу" << endl;
	cout << "5. Найти по названию" << endl;
	cout << "6. Найти по автору" << endl;
	cout << "7. Показать все книги" << endl;
	cout << "8. Показать выданные" << endl;
	cout << "9. Удалить все книги" << endl;
	cout << "10. Назад" << endl;
	cout << "----------------------------------------" << endl;
}

void readerInterface() {
	cout << "--- Управление читателями ---" << endl;
	cout << "1. Добавить читателя" << endl;
	cout << "2. Удалить читателя" << endl;
	cout << "3. Найти по имени" << endl;
	cout << "4. Показать всех читателей" << endl;
	cout << "5. Удалить всех читателей" << endl;
	cout << "6. Назад" << endl;
	cout << "----------------------------------------" << endl;
	cout << "Выберите действие: ";
}

void menuBooks(Library& lib) {
	int choice;
	do {
		bookInterface();
		choice = inputInt("Выберите действие: ");
		switch (choice) {
		case 1: {
			string title, author;
			int year;

			title = inputString("Введите название книги: ");
			author = inputString("Введите автора данной книги: ");
			year = inputInt("Введите дату издания книги: ");

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
			int targetID;
			bool valid = false;
			while (!valid) {
				cout << "Представлены свободные книги:" << endl;
				lib.printAvailableBooks();
				targetID = inputInt("Введите номер книги (ID) которую нужно убрать из библиотеки: ");

				Book* book = lib.findBookById(targetID);

				if (book && book->getIsAvailable()) {
					valid = true;
				}
				else {
					cout << "Ошибка: книга с таким ID не найдена или уже выдана. Попробуйте снова." << endl;
				}
			}
			if (lib.removeBook(targetID)) {
				cout << "Книга удалена!" << endl;
			}
			else {
				cout << "Не удалось удалить книгу" << endl;
			}
			cout << endl;
			break;

		}
		case 3:	borrowBookInterface(lib); break;
		case 4: returnBookInterface(lib); break;
		case 5: {
			string targetNameBook = inputString("Введите название книги: ");

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
		case 6: {
			string targetAuthorBook = inputString("Введите автора: ");;

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
		case 7: {
			if (lib.getAllBooks().empty()) {
				cout << "Библиотека пустая" << endl;
				break;
			}
			cout << "Все книги:" << endl;
			lib.printAllBooks();
			break;
		}
		case 8: {
			vector<Book> borrowedBooks = lib.getBorrowedBooks();
			if (borrowedBooks.empty()) {
				cout << "Ни одну книгу не заняли, либо библиотека пустая" << endl;
				break;
			}
			cout << "Занятые книги:" << endl;
			lib.printBorrowedBooks();
			break;
		}
		case 9: lib.clearAllBooks(); cout << "Все книги удалены из списка библиотеки!" << endl; break;
		}
	} while (choice != 10);
}


void readerMenu(Library& lib) {
	int choice;
	do {
		readerInterface();
		cin >> choice;
		switch (choice) {
		case 1: {
			string name, contact;
			name = inputString("Введите имя читателя: ");

			int contactType;
			cout << "Тип контакта:" << endl;
			cout << "1. Номер телефона" << endl;
			cout << "2. Электронная почта" << endl;
			contactType = inputInt("Выберите: ");

			bool isValid = false;
			while (!isValid) {
				if (contactType == 1) {
					contact = inputString("Введите номер телефона: ");
					if (auto err = validateNumberPhoneContact(contact)) {
						cout << *err << endl;
					}
					else {
						isValid = true;
					}
				}
				else if (contactType == 2) {
					contact = inputString("Введите email: ");
					if (auto err = validateEmailContact(contact)) {
						cout << *err << endl;
					}
					else {
						isValid = true;
					}
				}
			}

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
				choice = inputInt("");
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
			string targetNameReader = inputString("Введите имя читателя: ");
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
		case 5: lib.clearAllReaders(); cout << "Все читатели удалены из библиотеки!" << endl; break;
		}
	} while (choice != 6);
}


void borrowBookInterface(Library& lib) {
	if (hasNoData(lib)) return;

	cout << "--- Выдача книги ---" << endl;
	int bookId = inputValidBookId(lib, true);
	int readerId = inputValidReaderId(lib);

	if (lib.borrowBook(bookId, readerId, getCurrentDate())) {
		cout << "Книга выдана!" << endl;
	}
	else {
		cout << "Ошибка выдачи!" << endl;
	}
}

void returnBookInterface(Library& lib) {
	if (hasNoData(lib)) return;

	cout << "--- Возврат книги ---" << endl;
	int bookId = inputValidBookId(lib, false);
	int readerId = inputValidReaderId(lib);

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

bool hasNoData(const Library& lib) {
	if (lib.getAllBooks().empty() || lib.getAllReaders().empty()) {
		cout << "Нет ни одного читателя или книги" << endl;
		return true;
	}
	return false;
}
