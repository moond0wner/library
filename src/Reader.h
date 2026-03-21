#ifndef READER_H
#define READER_H

#include <string>
#include <vector>
#include <ctime>

class Reader {
private:
	int id; // уникальный номер
	std::string name; // имя
	std::string contact; // телефон/email
	std::vector<int> borrowedBooks; // ID взятых книг
	std::vector<int> history; // история ввсех взятых книг (ID)
	time_t registrationDate; // дата регистрации

public:
	Reader(std::string name, std::string contact); // конструктор

	// геттеры
	int getId() const;
	std::string getNameReader() const;
	std::string getContactReader() const;
	std::vector<int> getBorrowedBooks() const;
	std::vector<int> getHistory() const;
	time_t getRegistrationDate() const;

	// методы
	void borrowBook(int bookId); // добавить книгу в список взятых
	void returnBook(int bookId); // убрать из списка взятых, добавить в историю
	bool canBorrow() const; // можно ли взять еще книгу (проверка лимита и долгов)
	int borrowedCount() const; // сколько книг сейчас на руках

};

#endif