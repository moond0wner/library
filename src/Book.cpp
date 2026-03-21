#include "Book.h"
#include "Utils.h"

using namespace std;

Book::Book(string title, string author, int year) {
	static int nextId = 1;
	this->id = nextId++;

	this->title = title;
	this->author = author;
	this->year = year;
	this->isAvailable = true;
	this->borrowedBy = -1; // -1 означает не выдана
	this->borrowDate = 0;
}

int Book::getId() const { return id;  }
string Book::getTitle() const { return title; }
string Book::getAuthor() const { return author; }
int Book::getYear() const { return year; }
bool Book::getIsAvailable() const { return isAvailable; }
int Book::getBorrowedBy() const { return borrowedBy; }
time_t Book::getBorrowDate() const { return borrowDate; }

void Book::borrow(int readerId, time_t date) {
	if (!isAvailable) return;
	isAvailable = false;
	borrowedBy = readerId;
	borrowDate = date;
}

void Book::returnBook() {
	if (isAvailable) return;
	isAvailable = true;
	borrowedBy = -1;
}

bool Book::isOverdue(time_t currentDate) {
	if (isAvailable) return false;

	// Срок возврата - 14 дней с момента выдачи
	return isDateOverdue(borrowDate, currentDate, 14);
}