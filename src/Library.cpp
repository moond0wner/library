#include <iostream>
#include "Library.h"
#include "Book.h"
#include "Reader.h"
#include "Utils.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
const std::string Library::FILE_NAME = "library.json";

using namespace std;

const vector<Book>& Library::getAllBooks() const { return books; }
const vector<Reader>& Library::getAllReaders() const { return readers; }
vector<Book> Library::getAvailableBooks() const {
	vector<Book> availableBooks;
	for (auto& book : books) {
		if (book.getIsAvailable()) availableBooks.push_back(book);
	}
	return availableBooks;
}

vector<Book> Library::getBorrowedBooks() const {
	vector<Book> borrowedBooks;
	for (auto& book : books) {
		if (!book.getIsAvailable()) borrowedBooks.push_back(book);
	}
	return borrowedBooks;
}

void Library::addBook(string title, string author, int year) {
	Book book(title, author, year);
	books.push_back(book);
}

bool Library::removeBook(int bookId) {
	for (size_t i = 0; i < books.size(); i++) {
		if (books[i].getId() == bookId) {
			if (!books[i].getIsAvailable()) return false; // выданную книгу нельзя убрать из библиотеки
			books.erase(books.begin() + i);
			return true;
		}
	}
	return false;
}

Book* Library::findBookById(int bookId) {
	for (auto& book : books) {
		if (book.getId() == bookId) {
			return &book;
		}
	}
	return nullptr;
}

vector<Book> Library::findBooksByTitle(string title) {
	vector<Book> foundBooks;
	for (auto& book : books) {
		if (toLower(book.getTitle()).find(title) != string::npos) {
			foundBooks.push_back(book);
		}
	}
	return foundBooks;
}

vector<Book> Library::findBooksByAuthor(string author) {
	vector<Book> foundBooks;
	for (auto& book : books) {
		if (toLower(book.getAuthor()).find(author) != string::npos) {
			foundBooks.push_back(book);
		}
	}
	return foundBooks;
}

void Library::clearAllBooks() {
	books.clear();
}

void Library::addReader(string name, string contact) {
	Reader reader(name, contact);
	readers.push_back(reader);
}

bool Library::removeReader(int readerId) {
	for (size_t i = 0; i < readers.size(); i++) {
		if (readers[i].getId() == readerId) {
			if (readers[i].borrowedCount() > 0) {
				vector<int> borrowed = readers[i].getBorrowedBooks();
				for (int bookId : borrowed) {
					returnBook(bookId, readerId);
				}
			}
			readers.erase(readers.begin() + i);
			return true;
		}
	}
	return false;
}


Reader* Library::findReaderById(int readerId) {
	for (auto& reader : readers) {
		if (reader.getId() == readerId) {
			return &reader;
		}
	}
	return nullptr;
}

vector<Reader> Library::findReadersByName(string name) {
	vector<Reader> foundReaders;
	for (auto& reader : readers) {
		if (toLower(reader.getNameReader()).find(name) != string::npos) {
			foundReaders.push_back(reader);
		}
	}
	return foundReaders;
}

void Library::clearAllReaders() {
	readers.clear();
}

bool Library::borrowBook(int bookId, int readerId, time_t date) {
	Book* book = findBookById(bookId);
	Reader* reader = findReaderById(readerId);

	if (!book || !reader) return false;
	if (!book->getIsAvailable()) return false;
	if (reader->borrowedCount() >= BORROW_LIMIT) return false;
	
	book->borrow(readerId, date);
	reader->borrowBook(bookId);
	return true;
}

bool Library::returnBook(int bookId, int readerId) {
	Book* book = findBookById(bookId);
	Reader* reader = findReaderById(readerId);

	if (!book || !reader) return false;
	if (book->getIsAvailable()) return false;
	book->returnBook();
	reader->returnBook(bookId);
	return true;
}

vector<pair<Book, Reader>> Library::getOverdue(time_t currentDate) {
	vector<pair<Book, Reader>> overdue;
	for (auto& book : books) {
		if (!book.getIsAvailable() && book.isOverdue(currentDate)) {
			Reader* reader = findReaderById(book.getBorrowedBy());
			if (reader) {
				overdue.push_back({ book, *reader });
			}
		}
	}
	return overdue;
}

void Library::printAllBooks() {
	for (auto& book : books) {
		cout << "\t" << book.getId() << ": " << book.getTitle() << " - " << book.getAuthor() << " - " << book.getYear();
		if (!book.getIsAvailable()) {
			Reader* reader = findReaderById(book.getBorrowedBy());
			string readerName = reader ? reader->getNameReader() : "неизвестно";
			cout << " (выдана читателю " << readerName << " - " << dateToString(book.getBorrowDate()) << ")";
		}
		cout << endl;
	}
}

void Library::printAvailableBooks() {
	for (auto& book: books) {
		if (book.getIsAvailable()) {
			cout << "\t" << book.getId() << ": " << book.getTitle() << " - " << book.getAuthor() << " - " << book.getYear() << endl;
		}
	}
}

void Library::printBorrowedBooks() {
	for (auto& book : books) {
		if (!book.getIsAvailable()) {
			Reader* reader = findReaderById(book.getBorrowedBy());
			string readerName = reader ? reader->getNameReader() : "неизвестно";
			cout << "\t" << book.getId() << ": " << book.getTitle()
				<< " - " << book.getAuthor() << " - " << book.getYear()
				<< " (выдана читателю " << readerName
				<< " - " << dateToString(book.getBorrowDate()) << ")" << endl;
		}
	}
}

void Library::printAllReaders() {
	for (const auto& reader : readers) {
		cout << "\t" << reader.getId() << ": " << reader.getNameReader()
			<< " - " << reader.getContactReader()
			<< " (книг на руках: " << reader.borrowedCount() << ")" << endl;
	}
}

void Library::printOverdue(time_t currentDate) {
	auto overdue = getOverdue(currentDate);
	if (overdue.empty()) {
		cout << "\tНет должников" << endl;
		return;
	}
	for (const auto& [book, reader] : overdue) {
		cout << "\tКнига \"" << book.getTitle() << "\" у читателя "
			<< reader.getNameReader() << " (ID: " << reader.getId() << ")" << endl;
	}
}

void Library::saveToFile(const string& filename) const {
	try {
		json j;

		// книги
		j["books"] = json::array();
		for (const auto& book : books) {
			j["books"].push_back({
				{"id", book.getId()},
				{"title", book.getTitle()},
				{"author", book.getAuthor()},
				{"year", book.getYear()},
				{"isAvailable", book.getIsAvailable()},
				{"borrowedBy", book.getBorrowedBy()},
				{"borrowDate", book.getBorrowDate()}
				});
		}

		// читатели
		j["readers"] = json::array();
		for (const auto& reader : readers) {
			j["readers"].push_back({
				{"id", reader.getId()},
				{"name", reader.getNameReader()},
				{"contact", reader.getContactReader()},
				{"borrowedBooks", reader.getBorrowedBooks()},
				{"history", reader.getHistory()},
				{"registrationDate", reader.getRegistrationDate()}
				});
		}

		ofstream file(filename);
		if (!file.is_open()) {
			cerr << "Ошибка: не удалось открыть файл для записи: " << filename << endl;
			return;
		}
		file << j.dump(4);
		file.close();
		cout << "Сохранено в " << filename << endl;
	}
	catch (const exception& e) {
		cerr << "Ошибка при сохранении: " << e.what() << endl;
	}
}
void Library::loadFromFile(const string& filename) {
	ifstream file(filename);
	if (!file.is_open()) {
		cout << "Файл " << filename << " не найден. Будет создан новый." << endl;
		return;
	}

	// Проверка, что файл не пустой
	if (file.peek() == ifstream::traits_type::eof()) {
		cout << "Файл пуст. Будет создан новый." << endl;
		return;
	}

	try {
		json j;
		file >> j;

		books.clear();
		if (j.contains("books")) {
			for (const auto& item : j["books"]) {
				Book book(
					item["title"].get<string>(),
					item["author"].get<string>(),
					item["year"].get<int>()
				);
				book.setId(item["id"].get<int>());
				book.setIsAvailable(item["isAvailable"].get<bool>());
				book.setBorrowedBy(item["borrowedBy"].get<int>());
				book.setBorrowDate(item["borrowDate"].get<time_t>());
				books.push_back(book);
			}
		}

		readers.clear();
		if (j.contains("readers")) {
			for (const auto& item : j["readers"]) {
				Reader reader(
					item["name"].get<string>(),
					item["contact"].get<string>()
				);
				reader.setId(item["id"].get<int>());
				reader.setBorrowedBooks(item["borrowedBooks"].get<vector<int>>());
				reader.setHistory(item["history"].get<vector<int>>());
				reader.setRegistrationDate(item["registrationDate"].get<time_t>());
				readers.push_back(reader);
			}
		}

		cout << "Загружено из " << filename << endl;
	}
	catch (const exception& e) {
		cerr << "Ошибка при загрузке: " << e.what() << endl;
	}
}