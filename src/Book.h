#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <ctime>

class Book {
private:
	int id; // уникальный номер
	std::string title; // название книги
	std::string author; // автор книги
	int year; // год издания
	bool isAvailable; // доступна или выдана
	int borrowedBy; // ID читателя, если выдана
	time_t borrowDate; // дата выдачи (если выдана)

public:
	Book(std::string title, std::string author, int year); // конструктор

	// Геттеры
	int getId() const;
	std::string getTitle() const;
	std::string getAuthor() const;
	int getYear() const;
	bool getIsAvailable() const;
	int getBorrowedBy() const;
	time_t getBorrowDate() const;

	// методы
	void borrow(int readerId, time_t date); // выдать книгу
	void returnBook(); // вернуть книгу
	bool isOverdue(time_t currentDate); // просрочена ли книга
};

#endif