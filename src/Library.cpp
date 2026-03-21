#include <iostream>
#include "Library.h"
#include "Book.h"
#include "Reader.h"
#include "Utils.h"

using namespace std;

vector<Book> Library::getAllBooks() const { return books; }
vector<Reader> Library::getAllReaders() const { return readers; }
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
		if (book.getTitle().find(title) != string::npos) {
			foundBooks.push_back(book);
		}
	}
	return foundBooks;
}

vector<Book> Library::findBooksByAuthor(string author) {
	vector<Book> foundBooks;
	for (auto& book : books) {
		if (book.getAuthor().find(author) != string::npos) {
			foundBooks.push_back(book);
		}
	}
	return foundBooks;
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
		if (reader.getNameReader().find(name) != string::npos) {
			foundReaders.push_back(reader);
		}
	}
	return foundReaders;
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
		if (!book.getIsAvailable()) {
			cout << "\t" << book.getId() << ": " << book.getTitle() << " - " << book.getAuthor()
				<< " (выдана читателю " << dateToString(book.getBorrowDate()) << ")" << endl;
		}
		else {
			cout << "\t" << book.getId() << ": " << book.getTitle() << " - " << book.getAuthor() << endl;
		}
	}
}

void Library::printAvailableBooks() {
	for (auto& book: books) {
		if (book.getIsAvailable()) {
			cout << "\t" << book.getId() << ": " << book.getTitle() << " - " << book.getAuthor() << endl;
		}
	}
}

void Library::printBorrowedBooks() {
	for (auto& book : books) {
		if (!book.getIsAvailable()) {
			cout << "\t" << book.getId() << ": " << book.getTitle() << " - " << book.getAuthor()
				<< " (выдана читателю " << dateToString(book.getBorrowDate()) << ")" << endl;
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