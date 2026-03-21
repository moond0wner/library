#ifndef LIBRARY_H
#define LIBRARY_H

#include <vector>
#include <ctime>
#include <string>
#include "Book.h"
#include "Reader.h"

class Library {
private:
	std::vector<Book> books; // список книг
	std::vector<Reader> readers; // список читателей
	int nextBookId; // следующий ID для книги
	int nextReaderId; // следующий ID для читателя

public:
	// лимит книг на человека
	static const int BORROW_LIMIT = 5;

	// управление книгами
	void addBook(std::string title, std::string author, int year); // добавить книгу
	bool removeBook(int bookId); // удалить книгу (если не выдана)
	Book* findBookById(int bookId); // найти книгу по ID (возвращается указатель)
	std::vector<Book> findBooksByTitle(std::string title); // найти книги по названию
	std::vector<Book> findBooksByAuthor(std::string author); // найти книги по автору

	// геттеры
	std::vector<Book> getAllBooks() const;
	std::vector<Book> getAvailableBooks() const;
	std::vector<Book> getBorrowedBooks() const;
	std::vector<Reader> getAllReaders() const;

	// управление читателями
	void addReader(std::string name, std::string contact); // добавить читателя
	bool removeReader(int readerId); // удалить читателя (только если нет книг)
	Reader* findReaderById(int readerId); // найти читателя по ID
	std::vector<Reader> findReadersByName(std::string name); // найти читателей по имени

	// операции выдачи
	bool borrowBook(int bookId, int readerId, time_t date); // выдать книгу
	bool returnBook(int bookId, int readerId); // вернуть книгу
	std::vector<std::pair<Book, Reader>> getOverdue(time_t currentDate); // список должников

	// отчеты
	void printAllBooks(); // вывести все книги
	void printAvailableBooks(); // вывести доступные книги
	void printBorrowedBooks(); // вывести выданные
	void printAllReaders(); // вывести всех читателей
	void printOverdue(time_t currentDate); // вывести должников

};

#endif